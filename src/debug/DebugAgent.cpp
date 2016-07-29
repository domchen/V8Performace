
#include "DebugAgent.h"
#include <locale>
#include <codecvt>
#include <fstream>


inline static int VSNPrintF(char* buffer, int size, const char* format, va_list args) {
    int n = vsnprintf(buffer, static_cast<size_t>(size), format, args);
    if (n < 0 || n >= size) {
        // If the length is zero, the assignment fails.
        if (size > 0)
            buffer[size - 1] = '\0';
        return -1;
    } else {
        return n;
    }
}

inline static int SNPrintF(char* buffer, int size, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = VSNPrintF(buffer, size, format, args);
    va_end(args);
    return result;
}

static const char* ContentLength = "Content-Length";

// Receive the full buffer before returning unless an error occours.
static int ReceiveAll(Socket* conn, char* data, int len) {
    int total_received = 0;
    while (total_received < len) {
        int received = conn->receive(data + total_received, len - total_received);
        if (received == 0) {
            return total_received;
        }
        total_received += received;
    }
    return total_received;
}

static std::string ReceiveMessage(Socket* conn) {
    int received;

    // Read header.
    int content_length = 0;
    while (true) {
        const int kHeaderBufferSize = 80;
        char header_buffer[kHeaderBufferSize];
        int header_buffer_position = 0;
        char c = '\0';  // One character receive buffer.
        char prev_c = '\0';  // Previous character.

        // Read until CRLF.
        while (!(c == '\n' && prev_c == '\r')) {
            prev_c = c;
            received = conn->receive(&c, 1);
            if (received == 0) {
                return "";
            }

            // Add character to header buffer.
            if (header_buffer_position < kHeaderBufferSize) {
                header_buffer[header_buffer_position++] = c;
            }
        }

        // Check for end of header (empty header line).
        if (header_buffer_position == 2) {  // Receive buffer contains CRLF.
            break;
        }

        // Terminate header.
        ASSERT(header_buffer_position > 1);  // At least CRLF is received.
        ASSERT(header_buffer_position <= kHeaderBufferSize);
        header_buffer[header_buffer_position - 2] = '\0';

        // Split header.
        char* key = header_buffer;
        char* value = nullptr;
        for (int i = 0; header_buffer[i] != '\0'; i++) {
            if (header_buffer[i] == ':') {
                header_buffer[i] = '\0';
                value = header_buffer + i + 1;
                while (*value == ' ') {
                    value++;
                }
                break;
            }
        }

        // Check that key is Content-Length.
        if (strcmp(key, ContentLength) == 0) {
            // Get the content length value if present and within a sensible range.
            if (value == nullptr || strlen(value) > 7) {
                return std::string();
            }
            for (int i = 0; value[i] != '\0'; i++) {
                // Bail out if illegal data.
                if (value[i] < '0' || value[i] > '9') {
                    return std::string();
                }
                content_length = 10 * content_length + (value[i] - '0');
            }
        } else {
            // For now just print all other headers than Content-Length.
            LOG("%s: %s\n", key, value != nullptr ? value : "(no value)");
        }
    }

    // Return now if no body.
    if (content_length == 0) {
        return std::string();
    }

    // Read body.
    char* buffer = new char[content_length + 1];
    received = ReceiveAll(conn, buffer, content_length);
    if (received < content_length) {
        return std::string();
    }
    buffer[content_length] = '\0';

    return std::string(buffer);
}


static bool SendConnectMessage(Socket* conn,
                               const char* embedding_host) {
    static const int BUFFER_SIZE = 80;
    char buffer[BUFFER_SIZE];  // Sending buffer.
    int ok;
    int len;

    // Send the header.
    len = SNPrintF(buffer, BUFFER_SIZE,
                   "Type: connect\r\n");
    ok = conn->send(buffer, len);
    if (!ok) return false;

    len = SNPrintF(buffer, BUFFER_SIZE,
                   "V8-Version: %s\r\n", v8::V8::GetVersion());
    ok = conn->send(buffer, len);
    if (!ok) return false;

    len = SNPrintF(buffer, BUFFER_SIZE,
                   "Protocol-Version: 1\r\n");
    ok = conn->send(buffer, len);
    if (!ok) return false;

    if (embedding_host != nullptr) {
        len = SNPrintF(buffer, BUFFER_SIZE,
                       "Embedding-Host: %s\r\n", embedding_host);
        ok = conn->send(buffer, len);
        if (!ok) return false;
    }

    len = SNPrintF(buffer, BUFFER_SIZE,
                   "%s: 0\r\n", ContentLength);
    ok = conn->send(buffer, len);
    if (!ok) return false;

    // Terminate header with empty line.
    len = SNPrintF(buffer, BUFFER_SIZE, "\r\n");
    ok = conn->send(buffer, len);
    return static_cast<bool>(ok);
}

DebugAgent* DebugAgent::debugAgent = nullptr;
bool DebugAgent::initialized = false;

void DebugAgent::Initialize(std::string currentDirectory) {
    if (DebugAgent::initialized) {
        return;
    }
    DebugAgent::initialized = true;
    auto isolate = v8::Isolate::GetCurrent();
    ASSERT(isolate != nullptr);
    auto context = isolate->GetCurrentContext();
    ASSERT(!context.IsEmpty());
    v8::HandleScope handleScope(isolate);
    auto length = currentDirectory.length();
    if (currentDirectory[length - 1] != '/') {
        currentDirectory += "/";
    }
    std::string jsPath = currentDirectory + "debug-agent.js";
    std::string jsText = "var process = {pid:1,version:\"6.3.1\"};\n"
            "process.mainModule = {filename:\"";
    jsText += jsPath;
    jsText += "\"};\n";
    jsText += "process.cwd = function () {\n"
            "    return \"";
    jsText += currentDirectory;
    jsText += "\";\n"
            "};\n";

    std::ofstream jsFile;
    jsFile.open(jsPath, std::ios::trunc);
    jsFile << jsText;
    jsFile.close();

    auto source = v8::String::NewFromUtf8(isolate, jsText.c_str(),
                                          v8::NewStringType::kNormal).ToLocalChecked();
    v8::ScriptOrigin origin(
            v8::String::NewFromUtf8(isolate, jsPath.c_str(),
                                    v8::NewStringType::kNormal).ToLocalChecked());
    v8::Script::Compile(context, source, &origin).ToLocalChecked()->Run(context);
}

void DebugAgent::Enable(const std::string& hostName, int port, bool waitForConnection) {
    ASSERT(debugAgent == nullptr);
    if (debugAgent) {
        return;
    }
    auto isolate = v8::Isolate::GetCurrent();
    ASSERT(isolate != nullptr);
    debugAgent = new DebugAgent(isolate, hostName, port, waitForConnection);
}

void DebugAgent::Disable() {
    if (debugAgent) {
        delete debugAgent;
        debugAgent = nullptr;
    }
}

/**
 * Public V8 debugger API message handler function. This function just delegates to the debugger agent through it's
 * data parameter.
 */
void DebugAgentMessageHandler(const v8::Debug::Message& message) {
    DebugAgent::debugAgent->onDebugMessage(message);
}


DebugAgent::DebugAgent(v8::Isolate* isolate, const std::string& hostName, int port, bool waitForConnection)
        : isolate(isolate), hostName(hostName), port(port), server(new Socket),
          client(nullptr), terminate(false) {

    // Allow this socket to reuse port even if still in TIME_WAIT.
    server->setReuseAddress(true);

    // First bind the socket to the requested port.
    bool bound = server->bind(port);
    if (!bound) {
        LOG("Failed to open socket on port %d, ", port);
        return;
    }
    v8::Debug::SetMessageHandler(isolate, DebugAgentMessageHandler);
    if (waitForConnection) {

        if (server->listen(1)) {
            // Accept the new connection.
            client = server->accept();
            if (client) {
                v8::Debug::DebugBreak(isolate);
                // Create and start a new session.
                thread = std::thread(std::bind(&DebugAgent::runClientLoop, this));
            }
        }
    }
    else {
        thread = std::thread(std::bind(&DebugAgent::runClientLoop, this));
    }

}


DebugAgent::~DebugAgent() {
    v8::Debug::SetMessageHandler(isolate, nullptr);
    v8::Debug::CancelDebugBreak(isolate);
    terminate = true;
    delete server;
    server = nullptr;
    delete client;
    client = nullptr;
    thread.join();
}

void DebugAgent::runClientLoop() {
    while (true) {
        if (!client) {
            // Wait for connection
            if (server->listen(1));
            // Accept the new connection.
            client = server->accept();
            if (!client) {
                return;
            }
        }
        // Send the hello message.
        bool ok = SendConnectMessage(client, hostName.c_str());
        if (!ok) {
            return;
        }

        // Read data from the debugger front end.
        std::string message =
                ReceiveMessage(client);
        if (terminate) {
            return;
        }
        const char* msg = message.c_str();
        bool closed = (message.length() == 0);

        if (closed) {
            // If we lost the connection, then simulate a disconnect msg:
            msg = "{\"seq\":1,\"type\":\"request\",\"command\":\"disconnect\"}";

        } else {
            // Check if we're getting a disconnect request:
            const char* disconnectRequestStr =
                    "\"type\":\"request\",\"command\":\"disconnect\"}";
            const char* result = strstr(msg, disconnectRequestStr);
            if (result != nullptr) {
                closed = true;
            }
        }

        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        std::u16string dest = convert.from_bytes(msg);
        v8::Debug::SendCommand(isolate, reinterpret_cast<const uint16_t*>(dest.c_str()),
                               static_cast<int>(dest.length()));
        if (closed) {
            v8::Debug::CancelDebugBreak(isolate);
            delete client;
            client = nullptr;
        }
    }
}

void DebugAgent::onDebugMessage(const v8::Debug::Message& message) {
    if (client == nullptr) {
        return;
    }
    v8::HandleScope scope(message.GetIsolate());
    static const int kBufferSize = 80;
    char buffer[kBufferSize];  // Sending buffer both for header and body.

    // Convert the request to UTF-8 encoding.
    v8::String::Utf8Value utf8_request(message.GetJSON());

    // Send the header.
    int len = SNPrintF(buffer, kBufferSize,
                       "Content-Length: %d\r\n", utf8_request.length());
    if (client->send(buffer, len) < len) {
        return;
    }

    // Terminate header with empty line.
    len = SNPrintF(buffer, kBufferSize, "\r\n");
    if (client->send(buffer, len) < len) {
        return;
    }

    // Send message body as UTF-8.
    len = utf8_request.length();
    client->send(*utf8_request, len);
}