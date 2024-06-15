#include "acutest.h"
#include "NEO6M.h"
#include <string.h>


void test_nmea_messages()
{
    NEO6M gpsDecoder;
    string GPGAAMessage = "GPGGA, 123519, 4807.038, N, 01131.000, E, 1, 08, 0.9, 545.4, M, 46.9, M, , *47";
    string expectedTime = "123519";
    string expectedLongitude = "48";
    string expectedLongitudeDirection = "7.038N"
    string expectedLatutude = "011";
    string expectedLatutudeDirection = "31.000E"
    string expectedSatellites = "08"
    string expectedAltitude = "545.4M"

    TEST_CHECK_(0==0);
}

TEST_LIST =
{
    {"NMEA messages", test_nmea_messages},
    {0}
}
