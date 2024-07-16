#ifndef __GPS_HANDLER_H__
#define __GPS_HANDLER_H__
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define GPS_BAUD 9600
#define READ_GPS_MS 2000
#define MAX_SATELLITES 40

/*
struct {
    bool active;
    int elevation;
    int azimuth;
    int snr;
} sats[MAX_SATELLITES];
*/

class GPSHandler
{
    public:
        GPSHandler(uint8_t RXPin, uint8_t TXPin);
        bool Initialize(void);
        bool ReadGPSData(void);
        double GetLongitude(void) { return _longitude; }
        double GetLatitude(void) { return _latitude; }
        uint8_t GetNumberOfSatellites(void) { return _nSattelites; }
    private:
        TinyGPSPlus gps;
        SoftwareSerial ss;
        char GPSChar;
        double _longitude;
        double _latitude;
        uint8_t _nSattelites;

        /*
        TinyGPSCustom totalGPGSVMessages;
        TinyGPSCustom messageNumber;
        TinyGPSCustom satsInView;
        TinyGPSCustom satNumber[4]; // to be initialized later
        TinyGPSCustom elevation[4];
        TinyGPSCustom azimuth[4];
        TinyGPSCustom snr[4];
        */
};

#endif //__GPS_HANDLER_H__
