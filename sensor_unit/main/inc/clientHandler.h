#ifndef __CLIENT_H__
#define __CLIENT_H__
#include <WiFi.h>
#include <stdint.h>
#include "Arduino.h"

#define N_CONNECT_RETRIES 10
#define DELAY_BETWEEN_CONNECT_RETRIES 5000

typedef struct
{
    const char* serverIP;
    uint16_t serverPort;
}client_data_t;

typedef enum
{
    CLIENT_HANDLER_OK,
    CLIENT_HANDLER_FAILED_TO_CONNECT
}client_handler_error_codes;

class ClientHandler
{
    public:
        ClientHandler(client_data_t *data);
        bool Connect(void);
        void Write(String msg);
        void Stop();
    private:
        WiFiClient _client;
        const char * _serverIP;
        uint16_t _serverPort;
};
#endif
