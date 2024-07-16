//#include <WiFi.h>
#include <stdint.h>
//#include "inc/networkCredentials.h"
//#include "inc/clientHandler.h"
#include "inc/bmp280.h"
#include <Adafruit_BMP280.h>
#include "inc/gpsHandler.h"
#include "inc/windSensor.h"

#define SW_VERSION "v1.0"
#define BMP280_ADDRESS 0x76
#define MSG_DELIMITER ','
#define SERIAL_DEBUG

BMP280 bmp280(BMP280_ADDRESS);
//Adafruit_BMP280 bmp280;
BMP280_CONFIG bmp280Config = {
                                .mode = FORCED,
                                .overSamplingTemp = OVERSAMPLING_X4,
                                .overSamplingPress = OVERSAMPLING_X16
                              };
WindSensor windSensor(HMC5883L_ADDR, AS5600_ADDR);

//uint16_t getChipID(void);
uint16_t chipID = 0x00;

static const int RXPin = 3, TXPin = 4;

GPSHandler gpsHandler(RXPin, TXPin);
String longitude;
String latitude;
String nSattelites;

typedef struct
{
    uint16_t chipID;
    String SWVersion;
    uint8_t nSattelites;
    double Longitude;
    double Latitude;
    float Temperature;
    float Pressure;
    float WindSpeed;
    float WindDirection;
    AS5600_STATUS_t MagnetStatus;
}message_items_t;

typedef enum
{
    LOW_POWER,
    MONITORING,
    DATA_TRANSMISSION
}program_states_t;

message_items_t message;
program_states_t program_state = MONITORING;

String composeMessage(message_items_t *message);

void setup()
{
    Serial.begin(9600);
    gpsHandler.Initialize();
    Serial.println("Initializing BMP 280");
    if(bmp280.Initialize(&bmp280Config) != BMP280_OK)
    //if(!bmp280.begin())// != BMP280_OK)
    {
        Serial.println("Failed to initialize BMP280!");
    }

    //bmp280.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
    //                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
    //                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
    //                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
    //                Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    if(!windSensor.Initialize())
        Serial.println("Failed to initialze WindSensor");
    //chipID = getChipID();

   // WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
   // while(WiFi.status() != WL_CONNECTED)
   // {
   //     delay(500);
   //     Serial.print(".");
   // }
   // Serial.println("");
    bmp280.ReadData();
    delay(1000);
    bmp280.ReadData();
    while(1);
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
            bmp280.ReadData();
            gpsHandler.ReadGPSData();
            message.chipID = chipID;
            message.SWVersion = SW_VERSION;
            message.nSattelites = gpsHandler.GetNumberOfSatellites();
            message.Longitude = gpsHandler.GetLongitude();
            message.Latitude = gpsHandler.GetLatitude();
            message.Temperature = bmp280.GetTemperature();
            message.Pressure = bmp280.GetPressure();
            message.WindSpeed = windSensor.GetWindSpeed();
            message.WindDirection = windSensor.GetWindDirection();
            message.MagnetStatus = windSensor.GetMagnetStatus();

            #ifdef SERIAL_DEBUG
            /*
            Serial.print(message.Temperature);
            Serial.print("\t");
            Serial.println(message.Pressure);
            Serial.println("Wind Sensor");
            Serial.print(message.WindDirection);
            Serial.print("\t");
            Serial.print(message.WindSpeed);
            Serial.print("\t");
            Serial.println(message.MagnetStatus);
            Serial.println("GPS");
            Serial.print("Longitude: ");
            Serial.print(message.Longitude, 6);
            Serial.print(" Latitude: ");
            Serial.print(message.Latitude, 6);
            Serial.print(" Satellites: ");
            Serial.println(message.nSattelites);
            */
            Serial.println(composeMessage(&message));
            #endif
            //program_state = LOW_POWER;
        }
        break;

        case program_states_t::DATA_TRANSMISSION:
        {
            //Serial.println("Connecting to client..");
            //if(clientHandler.Connect())
            //{
            //    String message = "Hello from ESP32-C3!";
            //    clientHandler.Write(message);

            //    clientHandler.Stop();
            //}
            //else
            //{
            //    Serial.println("Failed to connecto to client");
            //}
            program_state = program_states_t::LOW_POWER;
       }
        default:
        break;
    }
 }

String composeMessage(message_items_t *message)
{
    String msg;
    msg += String(message->chipID) + MSG_DELIMITER;
    msg += message->SWVersion + MSG_DELIMITER;
    msg += String(message->nSattelites) + MSG_DELIMITER;
    msg += String(message->Longitude, 6) + MSG_DELIMITER; // Precision of 6 decimal places
    msg += String(message->Latitude, 6) + MSG_DELIMITER; // Precision of 6 decimal places
    msg += String(message->Temperature) + MSG_DELIMITER;
    msg += String(message->Pressure) + MSG_DELIMITER;
    msg += String(message->WindSpeed) + MSG_DELIMITER;
    msg += String(message->WindDirection) + MSG_DELIMITER;
    msg += String(message->MagnetStatus);  // Last field, no delimiter
    return msg;
}
uint16_t getChipID(void)
{
    /*
    for(int i=0; i<17; i=i+8)
    {
        chipID |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }
    return chipID;
    */
    return 0;

}
