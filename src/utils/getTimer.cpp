
#include "getTimer.h"
#include <chrono>

/**
 * 返回自1970年1月1日 00:00:00 UTC到当前时间的毫秒数
 */
double getTimer() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ns = now.time_since_epoch();
    return ns.count();
}