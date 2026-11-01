#include <zephyr/kernel.h>

#include "print_log.h"

LOG_MODULE_REGISTER(app_main, CONFIG_MCXN_APP_LOG_LEVEL);

#define HEARTBEAT_INTERVAL_MS 1000

int main(void)
{
    printLog_I("MCXN236 System - firmware started");
    printLog_I("Target: FRDM-MCXN236 | UART logger ready");
    printLog_D("Debug logging enabled | heartbeat interval=%d ms",
                HEARTBEAT_INTERVAL_MS);

    /* main thread dùng stack/priority mặc định; Zephyr có logger thread riêng
     * để xử lý deferred messages. Logger stack được cấu hình trong prj.conf.
     */
    while (1) {
        printLog_I("uptime=%lld ms | heartbeat", (long long)k_uptime_get());

        /* Nhường CPU cho thread khác/idle trong lúc chờ. */
        k_msleep(HEARTBEAT_INTERVAL_MS);
    }

    return 0;
}
