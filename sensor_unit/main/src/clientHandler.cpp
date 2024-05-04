#include "../inc/clientHandler.h"

ClientHandler::ClientHandler(client_data_t *data)
{
    _serverIP = data->serverIP;
    _serverPort = data->serverPort;
}

bool ClientHandler::Connect(void)
{
    for(uint8_t i = 0; i < N_CONNECT_RETRIES; i++)
    {
        Serial.print("Attempt ");
        Serial.print(i + 1);
        Serial.print(": Connecting to ");
        Serial.print(_serverIP);
        Serial.print(":");
        Serial.println(_serverPort);

        if(_client.connect(_serverIP, _serverPort))
        {
            Serial.println("Connected successfully.");
            return true;
        }
        Serial.println("Connection failed, retrying...");
        delay(5000);
    }
    Serial.println("All attempts failed!");
    return false;
}

void ClientHandler::Write(String msg)
{
    _client.println(msg);
    Serial.println("Sent: " + msg);
}

void ClientHandler::Stop(void)
{
    _client.stop();
    Serial.println("Connection closed");
}
