#ifndef __UART_HANDLER_H__
#define __UART_HANDLER_H__

#include <stdint.h>

#define UART_BUF_SIZE 256
typedef enum
{
	UART_OK,
	UART_FAILED_TO_OPEN,
	UART_FAILED_TO_GET_ATTR,
	UART_FAILED_TO_SET_ATTR
}UART_ERROR_CODES;

UART_ERROR_CODES initialize_uart(const char* port_name);
UART_ERROR_CODES get_latest_uart_data(char buf[UART_BUF_SIZE]);
UART_ERROR_CODES close_uart(void);

void start_uart_thread(void);
void stop_uart_thread(void);
uint8_t new_data_available(void);
#endif //__UART_HANDLER_H__
