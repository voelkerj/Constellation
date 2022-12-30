#include "math_helpers.h"

float math_helpers::deg2rad(const float& deg) { return deg * (PI / 180); }
float math_helpers::rad2deg(const float& rad) { return rad * (180 / PI); }

math_helpers::Vector3 math_helpers::ECI2ECF(Vector3 posECI, Vector3 velECI, float time)
{
    // https://space.stackexchange.com/questions/38807/transform-eci-to-ecef

    // This function is an approximation because it assumes ECI and ECEF have the same Z axis
    // In reality, the Earth nutates (wobbles) and these axis are not exactly lined up

    Vector3 posECF;

    float theta = time * OMEGA_EARTH;

    // Simple rotation solution
    posECF.x = posECI.x * cos(theta) + posECI.y * sin(theta);
    posECF.y = posECI.x * -sin(theta) + posECI.y * cos(theta);
    posECF.z = posECI.z;

    return posECF;
}

float math_helpers::coordDistance(GeoCoords p1, GeoCoords p2)
{
    // from https://community.esri.com/t5/coordinate-reference-systems-blog/distance-on-a-sphere-the-haversine-formula/ba-p/902128#:~:text=For%20example%2C%20haversine(%CE%B8),longitude%20of%20the%20two%20points.

    // Convert lat/lons to radians
    GeoCoords p1_temp;
    GeoCoords p2_temp;

    p1_temp.lat = deg2rad(p1.lat);
    p1_temp.lon = deg2rad(p1.lon);
    p2_temp.lat = deg2rad(p2.lat);
    p2_temp.lon = deg2rad(p2.lon);

    float lat_delta = p2_temp.lat - p1_temp.lat;
    float lon_delta = p2_temp.lon - p1_temp.lon;

    float a = pow(sin(lat_delta / 2), 2) + (cos(p1_temp.lat) * cos(p2_temp.lat) * pow(sin(lon_delta / 2), 2));
    float c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return RE * 1000 * c; //Distance in meters
}

float math_helpers::azimuth(GeoCoords p1, GeoCoords p2)
{
    // get the azimuth between two points
    // formula from https://www.omnicalculator.com/other/azimuth#:~:text=You%20can%20calculate%20the%20azimuth,is%20the%20difference%20in%20longitudes.

    p1.lat = deg2rad(p1.lat);
    p1.lon = deg2rad(p1.lon);
    p2.lat = deg2rad(p2.lat);
    p2.lon = deg2rad(p2.lon);

    float delta_lon = p2.lon - p1.lon;

    float az = rad2deg(atan2(sin(delta_lon) * cos(p2.lat), (cos(p1.lat) * sin(p2.lat)) - (sin(p1.lat) * cos(p2.lat) * cos(delta_lon))));

    if (az < 0)
    {
        az = 360 + az;
    }

    return az;
}

int math_helpers::convertLatToScreenCoords(const int& lat, const int& SCREEN_HEIGHT)
{
    int y_pixel = SCREEN_HEIGHT - ((SCREEN_HEIGHT * lat) / 180 + (SCREEN_HEIGHT / 2));

    return y_pixel;
}

int math_helpers::convertLonToScreenCoords(const int& lon, const int& SCREEN_WIDTH)
{
    int x_pixel;

    if (lon >= 0 && lon < 180)
    {
        x_pixel = (SCREEN_WIDTH * lon) / 360 + (SCREEN_WIDTH / 2);
    }
    else
    {
        x_pixel = (SCREEN_WIDTH * lon) / 360 - (SCREEN_WIDTH / 2);
    }
    return x_pixel;
}