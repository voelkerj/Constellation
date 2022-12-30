#ifndef SATELLITE_H
#define SATELLITE_H

#include <vector>
#include <string>
#include <mutex>
#include "math_helpers.h"
using namespace math_helpers;

class Satellite {
    public:
    // Setters
    void setEccentricity(const float& e);
    void setSMA(const float& SMA);
    void setInclination(const float& i);
    void setlongitudeAscendingNode(const float& RAAN);
    void setargumentPeriapsis(const float& argp);
    void setTrueAnomaly(const float& TA);
    void setSensorHalfAngle(float HA);
    void setName(std::string name) {_name = name;}
    void setColor(const int& red, const int& green, const int& blue);

    //Getters
    std::string getName() {return _name;}

    // Functions
    GeoCoords calculateLatLon(const float& time);
    float calculateMeanAnomaly(const float& time);
    float calculateEccentricAnomaly(const float& M);
    Vector3 calculateXYZ(const float& time);
    void calculateSemiMinorAxis();
    void calculateRadius();
    float calculateSensorConeProjectedArcLength();
    std::vector<GeoCoords> sensorConeLocusLatLons(float distance, float time);
    float getTimeAtTrueAnomaly(float& TA);
    float getTrueAnomalyAtTime(const float& time);
    bool pointWithinLocus(GeoCoords evalPoint, GeoCoords subOrbitalPoint, std::vector<GeoCoords> locus);
    void simulate(const int& startTime, const int& endTime, const int& TIMESTEP);

    std::vector<GeoCoords> _groundTrack;
    Color _color;                     // RGB color of the satellite

    private:
    float _eccentricity{0};           // 0 = circular
    float _semimajorAxis{6793};       // km, roughly the SMA of international space station
    float _inclination{45};           // degrees
    float _longitudeAscendingNode{0}; // degrees
    float _argumentPeriapsis{0};      // degrees
    float _trueAnomaly{0};            // degrees
    float _sensorHalfAngle{15};       // degrees
    float _semiminorAxis;             // km
    float _radius;                    // km, radius from center of Earth to satellite at given true anomaly
    std::string _name;                // name of the satellite
};

#endif