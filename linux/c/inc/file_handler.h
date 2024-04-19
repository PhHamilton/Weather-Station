#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include <stdio.h>
#include <stdint.h>

#define BUFF_SIZE 50

#define READ "rb"
#define WRITE "w"

typedef enum
{
	OK,
	FILE_NOT_FOUND,
	UNABLE_TO_OPEN_FILE,
	UNEXPECTED_EOF,
	READ_ERROR,
	WRITE_ERROR,
	UNKNOWN_ERROR
}FILE_HANDLER_ERROR_CODES;

FILE_HANDLER_ERROR_CODES read_file(const char* filePath);
char* get_file_content();
FILE_HANDLER_ERROR_CODES write_to_file(const char* filePath, const char *data);

#endif
