#include <WiFi.h>
#include "inc/networkCredentials.h"
#define N_TRANSMISSION_RETRIES 5

uint8_t transmission_retry_counter = 0;

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
            WiFiClient client;

            if (client.connect(server_ip, server_port))
            {
                Serial.println("Connected to server");

                // Send data to the server
                String message = "Hello from ESP32-C3!";
                client.println(message);
                Serial.println("Sent: " + message);

                // Close the connection
                client.stop();
                Serial.println("Connection closed");
                transmission_retry_counter = 0;
                program_state = SLEEP;
            }
            else
            {
                transmission_retry_counter++;
                Serial.println("Failed to connect to server");
            }

            if(transmission_retry_counter == N_TRANSMISSION_RETRIES)
            {
                transmission_retry_counter = 0;
                program_state = SLEEP;
            }
            
            delay(5000);
        }
        default:
        break;
    }
}
