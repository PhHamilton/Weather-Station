#include <WiFi.h>
#include <stdint.h>
#include "inc/networkCredentials.h"
#include "inc/clientHandler.h"
#include "inc/bmp280.h"

#define BMP_ADDR 0x76
BMP280 bmp280(BMP_ADDR);

uint16_t getChipID(void);
uint16_t chipID = 0x00;

client_data_t client_data = {.serverIP = server_ip, .serverPort = server_port};
ClientHandler clientHandler(&client_data);

typedef struct
{
    uint16_t chipID;
}message_items_t;

typedef enum
{
    LOW_POWER,
    MONITORING,
    DATA_TRANSMISSION
}program_states_t;

program_states_t program_state = MONITORING;

void setup()
{
    Serial.begin(115200);
    BMP280_CONFIG bmp280Config = {
                                    .mode = SLEEP,
                                    .overSamplingTemp = OVERSAMPLING_X1,
                                    .overSamplingPress = OVERSAMPLING_X1
                                  };
    Serial.println("Initializing BMP 280");
    if(bmp280.Initialize(&bmp280Config) != BMP280_OK)
    {
        Serial.println("Failed to initialize BMP280!");
    }

    chipID = getChipID();

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
        case program_states_t::LOW_POWER:
        {

        }
        break;

        case program_states_t::MONITORING:
        {
            Serial.println("Monitoring");
            for(int i = 0; i < 5; i++)
            {
                bmp280.ReadData();
                Serial.print(bmp280.GetTemperature());
                Serial.print("\t");
                Serial.println(bmp280.GetPressure());
                delay(3000);
            }
            program_state = LOW_POWER;
        }
        break;

        case program_states_t::DATA_TRANSMISSION:
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
            program_state = program_states_t::LOW_POWER;
       }
        default:
        break;
    }
}

uint16_t getChipID(void)
{
    for(int i=0; i<17; i=i+8)
    {
        chipID |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
    return chipID;
}
