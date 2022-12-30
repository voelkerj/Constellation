#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include "satellite.h"

namespace input_parser
{
    struct SimInputs
    {
        int FPS;
        int TIMESTEP;
        int START_TIME;
        int END_TIME;
    };

    SimInputs readSimInputs();

    std::vector<Satellite> readSatellites();
}

#endif