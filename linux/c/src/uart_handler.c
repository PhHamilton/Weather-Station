#include "uart_handler.h"
#include <stdio.h>

#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <pthread.h>
#include <unistd.h>

void *uart_thread(void);

uint8_t data_available;
uint8_t thread_is_running;
int8_t serial_port;
pthread_t thread_id;

UART_ERROR_CODES initialize_uart(const char* port_name)
{
	data_available = 0;
	thread_is_running = 0;

	serial_port = open(port_name, O_RDWR);

	if (serial_port < 0) 
	{
		//printf("Error %i from open: %s\n", errno, strerror(errno));
		return UART_FAILED_TO_OPEN;
	}
	struct termios tty;

	// Read in existing settings, and handle any error
	if(tcgetattr(serial_port, &tty) != 0) 
	{
	  printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	  return UART_FAILED_TO_GET_ATTR;
	}

	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
	tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

	tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;

	// Set in/out baud rate to be 9600
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);

	// Save tty settings, also checking for error
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) 
	{
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return UART_FAILED_TO_SET_ATTR;
	 
	}

	return UART_OK;
}

UART_ERROR_CODES close_uart(void)
{
	close(serial_port);
}

UART_ERROR_CODES get_latest_uart_data(char buf[UART_BUF_SIZE])
{
	data_available = 0;
	return UART_OK;
}

void start_uart_thread()
{
	thread_is_running = 1;
	printf("Creating uart thread.. \n");
	pthread_create(&thread_id, NULL, uart_thread, NULL);
	pthread_join(thread_id, NULL);
}

void stop_uart_thread()
{
	thread_is_running = 0;
}

uint8_t new_data_available(void)
{
	return data_available;	
}

void *uart_thread(void)
{
	printf("Entering thread");
	while(thread_is_running)
	{
		printf("123");
		sleep(1);
	}
	printf("Exiting thread");
	pthread_exit(NULL);
	return NULL;
}
