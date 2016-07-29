
#ifndef V8PERFORMANCE_LOG_H
#define V8PERFORMANCE_LOG_H

#include <stdlib.h>

#if !defined(V8PERFORMANCE_DEBUG) && !defined(V8PERFORMANCE_RELEASE)
#ifdef NDEBUG
#define V8PERFORMANCE_RELEASE
#else
#define V8PERFORMANCE_DEBUG
#endif
#endif


void printLog(const char format[], ...);
void printError(const char format[], ...);

#ifdef V8PERFORMANCE_DEBUG

#define LOG(...) printLog(__VA_ARGS__)
#define ASSERT(assertion) \
    if(!(assertion)) { \
        do { \
            printError("%s:%d: fatal error: \"%s\"\n", __FILE__, __LINE__, #assertion); \
            abort(); \
        } while (false); \
    }
#else

#define LOG(...)
#define ASSERT(assertion)

#endif

#endif //V8PERFORMANCE_LOG_H
