#ifndef __NEO6M_H__
#define __NEO6M_H__

#include <string.h>

class NEO6M
{
    public:
        NEO6M(void);
        int Decode(String Message);
        string GetLongitude(void);
        string GetLongitudeDirection(void);
        string GetLatitude(void);
        string GetLatitudeDirection(void);
        string GetAltitude(void);
        string GetNTrackedSatellites(void);
        string GetDate(void);
        string GetTime(void);
   pritate:
        string _longitude;
        string _latitude;
        string _altitude;
        string _date;
        string _time;
};

#endif //__NEO6M_H__
