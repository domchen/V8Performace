
#ifndef V8PERFORMANCE_DEBUGAGENT_H
#define V8PERFORMANCE_DEBUGAGENT_H

#include <string>
#include <v8-debug.h>
#include <thread>
#include "Socket.h"

/**
 * Debug agent which starts a socket listener on the debugger port and handles connection from a remote debugger.
 */
class DebugAgent {
public:

    /**
     * Enable the V8 debug agent. The debug agent will listen on the supplied TCP/IP port for remote debugger connection.
     * @param hostName the name of the embedding application.
     * @param port the TCP/IP port to listen on.
     * @param waitForConnection whether V8 should pause on a first statement allowing remote debugger to connect before
     * anything interesting happened.
     */
    static void Enable(const std::string& hostName, int port, bool waitForConnection = false);
    /**
     * Disable the V8 debug agent. The TCP/IP connection will be closed.
     */
    static void Disable();

private:
    static DebugAgent* debugAgent;
    static bool initialized;

    DebugAgent(v8::Isolate* isolate, const std::string& hostName, int port, bool waitForConnection);
    ~DebugAgent();
    void runClientLoop();
    void onDebugMessage(const v8::Debug::Message& message);

    bool terminate;
    v8::Isolate* isolate;
    std::string hostName;
    int port;  // Port to use for the agent.
    Socket* server;  // Server socket for listen/accept.
    Socket* client;  // Client socket for send/receive.

    std::thread thread;

    friend class DebugAgentSession;

    friend void DebugAgentMessageHandler(const v8::Debug::Message& message);
};

#endif //V8PERFORMANCE_DEBUGAGENT_H
