#include "satellite.h"
#include "iostream"

// Thread Lock
static std::mutex mtx;

// Setters
void Satellite::setEccentricity(const float& e)
{
    if (e < 0 || e >= 1)
    {
        std::cout << "Invalid Eccentricity: Must be between 0 and 1." << std::endl;
    }
    else
    {
        _eccentricity = e;
    }
}

void Satellite::setSMA(const float& SMA)
{
    if (SMA < 6378.1)
    {
        std::cout << "Semimajor Axis is inside the Earth!" << std::endl;
    }
    else
    {
        _semimajorAxis = SMA;
    }
}

void Satellite::setInclination(const float& i)
{
    if (i < 0 || i > 360)
    {
        std::cout << "Invalid Inclination: Must be between 0 and 360." << std::endl;
    }
    else
    {
        _inclination = i;
    }
}

void Satellite::setlongitudeAscendingNode(const float& RAAN)
{
    if (RAAN < 0 || RAAN > 360)
    {
        std::cout << "Invalid Longitude of Ascending Node: Must be between 0 and 360." << std::endl;
    }
    else
    {
        _longitudeAscendingNode = RAAN;
    }
}

void Satellite::setargumentPeriapsis(const float& argp)
{
    if (argp < 0 || argp > 360)
    {
        std::cout << "Invalid Argument of Periapsis: Must be between 0 and 360." << std::endl;
    }
    else
    {
        _argumentPeriapsis = argp;
    }
}

void Satellite::setTrueAnomaly(const float& TA)
{
    if (TA < 0 || TA > 360)
    {
        std::cout << "Invalid True Anomaly: Must be between 0 and 360." << std::endl;
    }
    else
    {
        _trueAnomaly = TA;
    }
}

void Satellite::setSensorHalfAngle(float HA)
{

    // Calculate max sensor half angle
    calculateRadius();
    float number = asin(RE / _radius);
    float maxHA = rad2deg(number);

    if (HA <= 0 || HA > maxHA)
    {
        std::cout << "Invalid Sensor Half Angle: Must be between 0 and " << maxHA << "." << std::endl;
    }
    else
    {
        _sensorHalfAngle = HA;
    }
}

void Satellite::setColor(const int& red, const int& green, const int& blue)
{
    if (red < 0 || red > 255)
    {
        throw std::invalid_argument("Value of Red must be between 0 and 255");
        _color.red = 0;
    } else
    {
        _color.red = red;
    }

    if (green < 0 || green > 255)
    {
        throw std::invalid_argument("Value of Green must be between 0 and 255");
        _color.green = 0;
    } else
    {
        _color.green = green;
    }

    if (blue < 0 || blue > 255)
    {
        throw std::invalid_argument("Value of Blue must be between 0 and 255");
        _color.blue = 0;
    } else
    {
        _color.blue = blue;
    }    
}

// Functions
GeoCoords Satellite::calculateLatLon(const float& time)
{
    GeoCoords subOrbitalPoint;

    Vector3 position = calculateXYZ(time);

    // Adapted from https://gis.stackexchange.com/questions/265909/converting-from-ecef-to-geodetic-coordinates
    int a = RE*1000;
    float f = 1 / 298.257223563;

    float b = a - f * a;
    float e = sqrt(pow(a, 2) - pow(b, 2)) / a;

    subOrbitalPoint.lon = rad2deg(atan2(position.y, position.x));

    float p = sqrt(pow(position.x, 2) + pow(position.y, 2));

    float h_0 = 0;

    float theta = atan2(position.z, p * (1 - pow(e, 2)));
    float N = pow(a, 2) / sqrt(pow(a * cos(theta), 2) + pow(b * sin(theta), 2));

    float h = p / cos(theta) - N;

    int iterations = 0;
    while (abs(h - h_0) > 0.000001)
    {
        h_0 = h;
        theta = atan2(position.z, p * (1 - pow(e, 2) * N / (N + h)));
        N = pow(a, 2) / sqrt(pow(a * cos(theta), 2) + pow(b * sin(theta), 2));
        h = p / cos(theta) - N;

        // Sometimes Newton's method can get bogged down so we put an iteration limit on it
        if (iterations == 999) 
        {
            break;
        }
        iterations++;
    }

    subOrbitalPoint.lat = rad2deg(theta);

    if (subOrbitalPoint.lon < 0)
    {
        subOrbitalPoint.lon += 360;
    }

    if (h <= 0)
    {
        std::unique_lock<std::mutex> lck(mtx);
        std::cout << this->getName() << " Invalid orbit, Inside the Earth! " << std::endl;
        std::cout << "Alt = " << h << std::endl;
        std::cout << subOrbitalPoint.lat << ", " << subOrbitalPoint.lon << std::endl;
        lck.unlock();
    }
    return subOrbitalPoint;
}

Vector3 Satellite::calculateXYZ(const float& time)
{
    Vector3 position;
    Vector3 velocity;

    float M = calculateMeanAnomaly(time);

    float E = calculateEccentricAnomaly(M);

    float r = _semimajorAxis * 1000 * (1 - _eccentricity * cos(E));
    float r_x = r * cos(deg2rad(_trueAnomaly));
    float r_y = r * sin(deg2rad(_trueAnomaly));

    // https://www.mathworks.com/matlabcentral/fileexchange/80632-kepler2carts
    position.x = (cos(deg2rad(_longitudeAscendingNode)) * cos(deg2rad(_argumentPeriapsis)) - sin(deg2rad(_longitudeAscendingNode)) * sin(deg2rad(_argumentPeriapsis)) * cos(deg2rad(_inclination))) * r_x + (-cos(deg2rad(_longitudeAscendingNode)) * sin(deg2rad(_argumentPeriapsis)) - sin(deg2rad(_longitudeAscendingNode)) * cos(deg2rad(_argumentPeriapsis)) * cos(deg2rad(_inclination))) * r_y;
    position.y = (sin(deg2rad(_longitudeAscendingNode)) * cos(deg2rad(_argumentPeriapsis)) + cos(deg2rad(_longitudeAscendingNode)) * sin(deg2rad(_argumentPeriapsis)) * cos(deg2rad(_inclination))) * r_x + (-sin(deg2rad(_longitudeAscendingNode)) * sin(deg2rad(_argumentPeriapsis)) + cos(deg2rad(_longitudeAscendingNode)) * cos(deg2rad(_argumentPeriapsis)) * cos(deg2rad(_inclination))) * r_y;
    position.z = (sin(deg2rad(_argumentPeriapsis)) * sin(deg2rad(_inclination))) * r_x + (cos(deg2rad(_argumentPeriapsis)) * sin(deg2rad(_inclination))) * r_y;

    float h = sqrt(MU * _semimajorAxis * 1000 * (1 - pow(_eccentricity, 2))); // specific angular momentum
    float L = _semimajorAxis * 1000 * (1 - pow(_eccentricity, 2));            // semilatus rectum

    velocity.x = ((position.x * h * _eccentricity) / (r * L)) * sin(deg2rad(_trueAnomaly)) - (h / r) * (cos(deg2rad(_longitudeAscendingNode)) * sin(deg2rad(_argumentPeriapsis + _trueAnomaly)) + sin(deg2rad(_longitudeAscendingNode)) * cos(deg2rad(_argumentPeriapsis + _trueAnomaly)) * cos(deg2rad(_inclination)));
    velocity.y = ((position.y * h * _eccentricity) / (r * L)) * sin(deg2rad(_trueAnomaly)) - (h / r) * (sin(deg2rad(_longitudeAscendingNode)) * sin(deg2rad(_argumentPeriapsis + _trueAnomaly)) - cos(deg2rad(_longitudeAscendingNode)) * cos(deg2rad(_argumentPeriapsis + _trueAnomaly)) * cos(deg2rad(_inclination)));
    velocity.z = ((position.z * h * _eccentricity) / (r * L)) * sin(deg2rad(_trueAnomaly)) + (h / r) * (sin(deg2rad(_inclination)) * cos(deg2rad(_argumentPeriapsis + _trueAnomaly)));

    Vector3 posECF = ECI2ECF(position, velocity, time);
    return posECF;
}

void Satellite::calculateSemiMinorAxis()
{
    _semiminorAxis = _semimajorAxis * sqrt(1 - pow(_eccentricity, 2));
}

void Satellite::calculateRadius()
{
    _radius = (_semimajorAxis * _semiminorAxis) / (sqrt((pow(_semimajorAxis, 2) * pow(sin(deg2rad(_trueAnomaly)), 2)) + (pow(_semiminorAxis, 2) * pow(cos(deg2rad(_trueAnomaly)), 2))));
}

float Satellite::calculateSensorConeProjectedArcLength()
{
    float arcLength;
    float x;     // result of quadratic equation
    float theta; // sector angle

    // Law of cosines written in standard quadratic form and now solving the quadratic equation.
    float a = 1;
    float b = -2 * _radius * cos(deg2rad(_sensorHalfAngle));
    float c = pow(_radius, 2) - pow(RE, 2);

    float x1 = (-b + sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);
    float x2 = (-b - sqrt(pow(b, 2) - (4 * a * c))) / (2 * a);

    if (x1 > x2)
    {
        x = x1;
    }
    else
    {
        x = x2;
    }

    // Law of sines to get theta
    theta = asin((x * sin(deg2rad(_sensorHalfAngle))) / RE);

    // S = r * theta to get arc length
    arcLength = RE * theta;

    return arcLength;
}

/*std::vector<GeoCoords> Satellite::sensorConeLocusLatLons(float distance, float time)
{
    // this function adapted from: https://stackoverflow.com/questions/7222382/get-lat-long-given-current-point-distance-and-bearing

    std::vector<GeoCoords> locus;

    // Get coords of this satellite
    GeoCoords originCoords = calculateLatLon(time);

    // Convert to radians
    originCoords.lat = deg2rad(originCoords.lat);
    originCoords.lon = deg2rad(originCoords.lon);

    // Calculate locus points
    for (int azimuth_deg = 0; azimuth_deg < 360; azimuth_deg++) // if you change this to be more than 360 points, then you must change pointWithinLocus
    {
        float azimuth_rad = deg2rad(azimuth_deg);
        GeoCoords temp_coords;

        // Calculate lat lon of locus
        temp_coords.lat = asin((sin(originCoords.lat) * cos(distance / RE)) + (cos(originCoords.lat) * sin(distance / RE) * cos(azimuth_rad)));
        temp_coords.lon = originCoords.lon + (atan2(sin(azimuth_rad) * sin(distance / RE) * cos(originCoords.lat), cos(distance / RE) - (sin(originCoords.lat) * sin(temp_coords.lat))));

        // Convert to degrees
        temp_coords.lat = rad2deg(temp_coords.lat);
        temp_coords.lon = rad2deg(temp_coords.lon);

        locus.push_back(temp_coords);
    }
}*/

float Satellite::calculateMeanAnomaly(const float& time)
{
    float n = sqrt(MU / pow(_semimajorAxis * 1000, 3));
    float M = n * time;

    // M must be between 0 and 360
    while (M > (2 * PI))
    {
        M = M - (2 * PI);
    }

    return M;
}

float Satellite::calculateEccentricAnomaly(const float& M)
{
    float E; // Eccentric Anomaly

    // solve for eccentric anomaly using Newton's Method
    E = 1;        //initial "guess"
    float result;

    for (int iteration = 0; iteration < 100; iteration++)
    {
        E = E - (E - (_eccentricity * sin(E)) - M) / (1 - (_eccentricity * cos(E)));
        result = E - (_eccentricity * sin(E)) - M;

        if (abs(result) < .000001)
            break;
    }

    // E must be between 0 and 360
    while (E < 0)
    {
        E += (2 * PI);
    }
    while (E > (2 * PI))
    {
        E -= (2 * PI);
    }

    return E;
}

float Satellite::getTimeAtTrueAnomaly(float& TA)
{
    // adapted from https://space.stackexchange.com/questions/54396/how-to-calculate-the-time-to-reach-a-given-true-anomaly

    TA = deg2rad(TA);

    float E; // Eccentric Anomaly
    float M; // Mean Anomaly
    float n; // Mean Motion

    if (TA >= 0)
    {
        E = acos((_eccentricity + cos(TA)) / (1 + (_eccentricity * cos(TA))));
    }
    else
    {
        E = -acos((_eccentricity + cos(TA)) / (1 + (_eccentricity * cos(TA))));
    }

    M = E - (_eccentricity * sin(E));
    n = sqrt(MU / pow(abs(_semimajorAxis * 1000), 3));

    float time = M / n;

    return time;
}

float Satellite::getTrueAnomalyAtTime(const float& time)
{
    float M = calculateMeanAnomaly(time);

    float E = calculateEccentricAnomaly(M);

    float TA = rad2deg(2 * atan2(sqrt(1 + _eccentricity) * sin(E / 2), sqrt(1 - _eccentricity) * cos(E / 2)));

    if (TA < 0)
    {
        TA = 360 + TA;
    }

    return TA;
}

bool Satellite::pointWithinLocus(GeoCoords evalPoint, GeoCoords subOrbitalPoint, std::vector<GeoCoords> locus)
{
    // get suborbital point and its azimuth/distance to the evaluation point
    float az = round(azimuth(subOrbitalPoint, evalPoint));
    float evalDist = coordDistance(subOrbitalPoint, evalPoint);

    // get point nearest to locus
    GeoCoords nearestPoint = locus[az]; // only works because locus is 360 points in length
    float locusDist = coordDistance(subOrbitalPoint, nearestPoint);

    return evalDist <= locusDist;
}

void Satellite::simulate(const int& startTime, const int& endTime, const int& TIMESTEP)
{
    std::unique_lock<std::mutex> lck(mtx);
    std::cout << "Starting " << this->getName() << " simulation." << std::endl;
    lck.unlock();

    std::vector<int> pointsCoverage;

    for (int time = startTime; time < endTime; time = time + TIMESTEP)
    {
        setTrueAnomaly(getTrueAnomalyAtTime(static_cast<float>(time)));
        calculateRadius();
        GeoCoords subOrbitalPoint = calculateLatLon(time);
        this->_groundTrack.push_back(subOrbitalPoint);
    }

    lck.lock();
    std::cout << this->getName() << " simulation complete." << std::endl;
    lck.unlock();
}