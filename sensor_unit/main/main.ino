#include <WiFi.h>
#include "inc/networkCredentials.h"
#include "inc/clientHandler.h"

client_data_t client_data = {.serverIP = server_ip, .serverPort = server_port};
ClientHandler clientHandler(&client_data);

typedef enum
{
    SLEEP,
    MONITORING,
    DATA_TRANSMISSION
}program_states_t;

program_states_t program_state = DATA_TRANSMISSION;



void setup()
{
    Serial.begin(115200);


    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
}

void loop()
{
    switch(program_state)
    {
        case SLEEP:
        {

        }
        break;

        case MONITORING:
        {

        }
        break;

        case DATA_TRANSMISSION:
        {
            Serial.println("Connecting to client..");
            if(clientHandler.Connect())
            {
                String message = "Hello from ESP32-C3!";
                clientHandler.Write(message);

                clientHandler.Stop();
            }
            else
            {
                Serial.println("Failed to connecto to client");
            }
            program_state = SLEEP;
       }
        default:
        break;
    }
}
