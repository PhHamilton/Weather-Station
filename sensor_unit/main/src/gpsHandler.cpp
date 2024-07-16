#include "../inc/gpsHandler.h"


GPSHandler::GPSHandler(uint8_t RXPin, uint8_t TXPin) : ss(RXPin, TXPin)
{
    /*
    totalGPGSVMessages(gps, "GPGSV", 1); // $GPGSV sentence, first element
    messageNumber(gps, "GPGSV", 2);      // $GPGSV sentence, second element
    satsInView(gps, "GPGSV", 3);         // $GPGSV sentence, third element
    */

}

bool GPSHandler::Initialize(void)
{
    ss.begin(GPS_BAUD);
    /*
    for(uint8_t i = 0; i < 4; i++)
    {
        satNumber[i].begin(gps, "GPGSV", 4 + 4 * i); // offsets 4, 8, 12, 16
        elevation[i].begin(gps, "GPGSV", 5 + 4 * i); // offsets 5, 9, 13, 17
        azimuth[i].begin(  gps, "GPGSV", 6 + 4 * i); // offsets 6, 10, 14, 18
        snr[i].begin(      gps, "GPGSV", 7 + 4 * i); // offsets 7, 11, 15, 19
    }
    */

    return true;
}

bool GPSHandler::ReadGPSData(void)
{
    uint64_t startMillis = millis();
    while(millis() - startMillis <= READ_GPS_MS)
    {
        while(ss.available() > 0)
        {
            GPSChar = ss.read();
            gps.encode(GPSChar);
        }

        if(gps.satellites.isValid())
        {
            _nSattelites = gps.satellites.value();
        }
        if(gps.location.isUpdated())
        {
            _longitude = gps.location.lng();
            _latitude = gps.location.lat();
        }
    }

    return true;
}
