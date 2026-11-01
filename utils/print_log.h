#ifndef UTILS_PRINT_LOG_H_
#define UTILS_PRINT_LOG_H_

#include <zephyr/logging/log.h>

/* Header dùng chung cho các module/application Zephyr.
 * Mỗi module đăng ký LOG_MODULE_REGISTER() tại file .c sở hữu module.
 * fmt phải là string literal; không thêm newline vào cuối message.
 * Giữ LOG_* native để Zephyr lọc level ngay lúc compile.
 */
#if defined(CONFIG_MCXN_LOG_SOURCE_LOCATION)
#if defined(__FILE_NAME__)
#define PRINT_LOG_FILE __FILE_NAME__
#else
#define PRINT_LOG_FILE __FILE__
#endif

#define PRINT_LOG_IMPL(log_fn, fmt, ...) \
	log_fn("%s:%u %s(): " fmt, PRINT_LOG_FILE, (unsigned int)__LINE__, \
	       __func__, ##__VA_ARGS__)
#else
#define PRINT_LOG_IMPL(log_fn, fmt, ...) log_fn(fmt, ##__VA_ARGS__)
#endif

#define printLog_E(fmt, ...) PRINT_LOG_IMPL(LOG_ERR, fmt, ##__VA_ARGS__)
#define printLog_W(fmt, ...) PRINT_LOG_IMPL(LOG_WRN, fmt, ##__VA_ARGS__)
#define printLog_I(fmt, ...) PRINT_LOG_IMPL(LOG_INF, fmt, ##__VA_ARGS__)
#define printLog_D(fmt, ...) PRINT_LOG_IMPL(LOG_DBG, fmt, ##__VA_ARGS__)

#endif /* UTILS_PRINT_LOG_H_ */
