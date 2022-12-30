#include "input_parser.h"
#include <string>
#include <fstream>
#include <iostream>
#include <regex>

input_parser::SimInputs input_parser::readSimInputs()
{
    SimInputs inputs;

    std::string line;
    std::string key;
    int value;

    std::string file = "inputs.txt";

    std::ifstream filestream(file);

    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            if (line[0] != '#') //if not comment character
            {
                std::replace(line.begin(), line.end(), '=', ' ');
                std::istringstream linestream(line);

                while (linestream >> key >> value)
                {                    
                    if (key == "fps")
                    {
                        inputs.FPS = value;
                    }
                    if (key == "timestep")
                    {
                        inputs.TIMESTEP = value;
                    }
                    if (key == "start_time")
                    {
                        inputs.START_TIME = value*60*60;
                    }
                    if (key == "end_time")
                    {
                        inputs.END_TIME = value*60*60;
                    }
                }
            }
        }
    }
    return inputs;
}

std::vector<Satellite> input_parser::readSatellites()
{
    std::vector<Satellite> satellite_vector;

    std::string line;
    std::string key;
    std::string value1{" "}, value2{" "}, value3{" "};

    std::string file = "satellites.txt";

    Satellite sat;

    std::ifstream filestream(file);

    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            if (line[0] != '#') //if not comment character
            {
                std::replace(line.begin(), line.end(), '=', ' ');

                std::istringstream linestream(line);

                while (linestream >> key >> value1)
                {                    
                    if (key == "name")
                    {
                        sat.setName(value1);
                    }
                    if (key == "color")
                    {
                        linestream >> value2 >> value3; // Get additional values

                        try
                        {
                            sat.setColor(std::stoi(value1), std::stoi(value2), std::stoi(value3));
                        }
                        catch(const std::invalid_argument& e)
                        {
                            std::cerr << e.what() << '\n';
                        }
                    }
                    if (key == "eccentricity")
                    {
                        sat.setEccentricity(std::stof(value1));
                    }
                    if (key == "semi-major_axis")
                    {
                        sat.setSMA(std::stof(value1));
                    }
                    if (key == "inclination")
                    {
                        sat.setInclination(std::stof(value1));
                    }
                    if (key == "RAAN")
                    {
                        sat.setlongitudeAscendingNode(std::stof(value1));
                    }
                    if (key == "argument_of_periapsis")
                    {
                        sat.setargumentPeriapsis(std::stof(value1));
                        satellite_vector.emplace_back(sat);
                        std::cout << "Loaded " << sat.getName() << std::endl;
                    }
                }
            }
        }
    }
    return satellite_vector;
}