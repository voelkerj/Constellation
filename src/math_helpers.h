#ifndef MATH_HELPERS_H
#define MATH_HELPERS_H

#include "math.h"

const float PI          = 3.14159265359;   // pi
const float RE          = 6378.137;          // km, Radius of Earth
const float MU          = 3.986004418e+14; // m^3 * s^-2, standard gravitational parameter of Earth
const float SD          = 86164.0905;      // s, sidereal day
const float OMEGA_EARTH = 7.2921158553e-5; // rad/s, rotation rate of Earth

namespace math_helpers
{
    struct GeoCoords
    {
        float lat;
        float lon;
    };

    struct Vector3
    {
        float x;
        float y;
        float z;
    };

    struct Color
    {
        int red;
        int green;
        int blue;
    };

    float deg2rad(const float& deg);

    float rad2deg(const float& rad);

    float coordDistance(GeoCoords p1, GeoCoords p2);

    float azimuth(GeoCoords p1, GeoCoords p2);

    int convertLatToScreenCoords(const int& lat, const int& SCREEN_HEIGHT);
    int convertLonToScreenCoords(const int& lon, const int& SCREEN_WIDTH);

    Vector3 ECI2ECF(Vector3 posECI, Vector3 velECI, float time);

} // namespace math_helpers
#endif