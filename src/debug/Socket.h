#ifndef V8PERFORMANCE_SOCKET_H
#define V8PERFORMANCE_SOCKET_H

#include "Log.h"

class Socket final {
public:
    Socket();

    ~Socket();

    // Server initialization.
    bool bind(int port);
    bool listen(int backlog);
    Socket* accept();

    /**
     * Client initialization.
     */
    bool connect(const char* host, const char* port);

    /**
     *  Shutdown socket for both read and write. This causes blocking send and receive calls to exit.
     *  After |shutdown()| the Socket object cannot be used for any communication.
     */
    bool shutdown();

    // Data Transmission
    // Return 0 on failure.
    int send(const char* buffer, int length);
    int receive(char* buffer, int length);

    /**
     * Set the value of the SO_REUSEADDR socket option.
     */
    bool setReuseAddress(bool reuse_address);

    bool isValid() const;

    static int getLastError();


private:
    class PlatformData;

    PlatformData* data;

    explicit Socket(PlatformData* data) : data(data) {}
};


#endif  // V8PERFORMANCE_SOCKET_H
