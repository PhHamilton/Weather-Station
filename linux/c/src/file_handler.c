#include "file_handler.h"
#include <string.h>
#define VALUES_TO_READ 1
#define VALUES_TO_WRITE 1

FILE_HANDLER_ERROR_CODES write_content(const char *data);
FILE_HANDLER_ERROR_CODES read_content();
FILE_HANDLER_ERROR_CODES open_file(const char* filePath, const char* fileAction);
FILE_HANDLER_ERROR_CODES close_file();
FILE_HANDLER_ERROR_CODES safe_return(FILE_HANDLER_ERROR_CODES error_code);

FILE *_filePointer;
char _fileBuffer[BUFF_SIZE];

FILE_HANDLER_ERROR_CODES read_file(const char* filePath)
{
	FILE_HANDLER_ERROR_CODES rc = open_file(filePath, READ);

	if(rc != OK) return safe_return(rc); 
	
	rc = read_content();

	if(rc != OK) return safe_return(rc);

	close_file();
	return OK;
}

char * get_file_content()
{
	return _fileBuffer;
}

FILE_HANDLER_ERROR_CODES write_to_file(const char* filePath, const char* data)
{
	FILE_HANDLER_ERROR_CODES rc = open_file(filePath, WRITE);

	if(rc != OK) return safe_return(rc); 
	
	rc = write_content(data);

	if(rc != OK) return safe_return(rc);

	close_file();
	return OK;
}

FILE_HANDLER_ERROR_CODES write_content(const char *data)
{
	size_t writtenBytes = fwrite(data, VALUES_TO_WRITE, strlen(data), _filePointer);		

	if(writtenBytes != strlen(data)) 
		return WRITE_ERROR;

	return OK;
}

FILE_HANDLER_ERROR_CODES read_content()
{
	char* readContent = fgets(_fileBuffer, BUFF_SIZE, _filePointer);

    if (readContent != NULL) 
	{
        // Remove the newline character, if it exists
        size_t len = strlen(_fileBuffer);

        if (len > 0 && _fileBuffer[len - 1] == '\n') 
		{
            _fileBuffer[len - 1] = '\0';
        }

        return OK;
    } else 
	{
        return UNEXPECTED_EOF;
	}
}

FILE_HANDLER_ERROR_CODES open_file(const char* filePath, const char* fileAction)
{
	_filePointer = fopen(filePath, fileAction);
	if(_filePointer == NULL)
		return UNABLE_TO_OPEN_FILE;
	return OK;
}

FILE_HANDLER_ERROR_CODES close_file()
{
	fclose(_filePointer);
	return 0;
}

FILE_HANDLER_ERROR_CODES safe_return(FILE_HANDLER_ERROR_CODES error_code)
{
	close_file();
	return error_code;
}

