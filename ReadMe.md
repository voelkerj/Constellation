# CONSTELLATION

## ABOUT
Constellation is a satellite orbit propagator. It assumes a spherical, rotating Earth and displays the ground tracks of propagated orbits on a 2D map. This was originally my capstone project for the Udacity C++ nanodegree.

Inputs are read from two text files. Inputs.txt controls simulation parameters such as start time and end time. Satellites.txt is where the user can define satellites. They can list as many as they desire and the propagator will animate them all at once. A custom color can be set by the user to differentiate each satellite.

This is a true propagator. It uses math and physics to propagate orbits. It does not take shortcuts nor does it call external libraries to do this. For more on how it works, you may see the PROPAGATOR DETAILS below.

## TO BUILD
**Windows with MinGW:**  
1.) Download and extract current stable [SDL2-devel-2.26.2-mingw.zip](https://github.com/libsdl-org/SDL/releases) and [SDL2_image-devel-2.6.2-mingw.zip](https://github.com/libsdl-org/SDL_image/releases)  
2.) Add C:\Libraries\SDL2-2.26.2\x86_64-w64-mingw32\bin and C:\Libraries\SDL2_image-2.6.2\x86_64-w64-mingw32\bin to your path  
3.) Set SDL2_DIR and SDL_image_DIR in CMakeLists.txt  
4.) cd into the project directory  
5.) mkdir build  
6.) cd build  
7.) cmake -G "MinGW Makefiles" ..  
8.) mingw32-make.exe  

**Linux:**  
*I am leaving these build instructions here, but I have not actually built this project on Linux recently, so they may not apply anymore.*  
1). Two external libraries are needed: SDL2 and SDL_Image.  
Install SDL2: apt-get install libsdl2-dev
Install SDL_Image: apt-get install libsdl2-image-dev  
2.) cd into the project directory  
3.) mkdir build  
4.) cd build  
5.) cmake ..  
6.) make  

## TO RUN 
1.) Copy input.txt and satellites.txt from the "examples" folder into the build directory  
2.) cd into the build folder and run ./Constellation

The example files provided have some interesting and notable satellites preset for you. You can simply modify them to create your own. You can also comment out lines using the # character.  
Note that each satellite must be a complete block, you cannot put inputs for one satellite in between inputs for another.

For setting satellite colors, I recommend pulling the RGB values from the following site: https://htmlcolorcodes.com/color-picker/

## FILE AND CLASS STRUCTURE
All code is in the "src" folder. All images are in the "resources" folder. Once the user creates the build folder (see section "TO RUN"), they must place an inputs.txt and a satellites.txt in this folder (again, see section "TO RUN"). 

- main.cpp            - Contains the main() function. Kicks off satellite simulation and manages rendering/animation.
- satellite.cpp/.h    - Contains the satellite class, including getters/setters, and functions such as simulate().
- math_helpers.cpp/.h - Utility functions to do mathematical conversions.
- input_parser.cpp/.h - Controls the reading of input files.
- cleanup.h           - For deleting SDL elements.
- error.h             - For displaying SDL errors.
- resource_path.h     - For dynamically generating the path to the resource folder.
- texture.h           - For loading and configuring SDL textures.

## PROPAGATOR DETAILS
The user inputs the initial state of the satellite in Keplerian elements. The satellite is then propagated forward for each timestep. The Keplerian orbital state at each point is converted to Earth-Centered Intertial (ECI) coordinates, then to Earth-Centered Fixed (ECF) coordinates. This ECI-to-ECF conversion is what accounts for the rotation of the Earth. This rotation has the assumption that the Z axis for ECI and ECF are exactly aligned. In other words, this simulation does not account for the precession and nutation of the Earth's rotational axis. From there, the ECF coordinates are converted to Latitude/Longitude/Altitude. Since this is a 2D display, we ignore the altitude and continue by converting the lat/lon coordinates into screen X and Y pixel coordinates.  
The animation runs at the FPS set by the user in inputs.txt.

## ACKNOWLEDGEMENTS
I would like to credit the following sources for helping me in this project. Sources that helped with portions of code are also listed in comments next to that particular code.

NASA Visible Earth Image Collection  
https://visibleearth.nasa.gov/collection/1484/blue-marble  
For providing the Earth background image.  

TwinklebearDev SDL 2.0 Tutorials  
https://www.willusher.io/pages/sdl2/  
For SDL help, including windows, renderers, textures, errors, and cleanup.  

Lazy Foo' Productions  
https://lazyfoo.net/tutorials/SDL/  
For SDL help, including SDL_image, and modulating colors in SDL.  

User Leo Cardona on Stack Exchange  
https://gis.stackexchange.com/questions/265909/converting-from-ecef-to-geodetic-coordinates  
For help with an algorithm to conver ECF to LLA coordinates.  

User Thameur Chebbi on Mathworks  
https://www.mathworks.com/matlabcentral/fileexchange/80632-kepler2carts  
For providing formulas for converting KEP to ECI.  

User David M on Stack Exchange  
https://stackoverflow.com/questions/7222382/get-lat-long-given-current-point-distance-and-bearing  
This answer helped with a distance calculation.  

User notovny on Stack Exchange  
https://space.stackexchange.com/questions/54396/how-to-calculate-the-time-to-reach-a-given-true-anomaly  
For providing the steps to solve for true anomaly.  

User Jai Willems on Stack Exchange  
https://space.stackexchange.com/questions/38807/transform-eci-to-ecef  
For proving the formulas to approximately convert ECI to ECF.  

This post by SimonKettle  
https://community.esri.com/t5/coordinate-reference-systems-blog/distance-on-a-sphere-the-haversine-formula/ba-p/902128#:~:text=For%20example%2C%20haversine(%CE%B8),longitude%20of%20the%20two%20points.  
For a description and usage of the Haversine Formula.  

This post by Bogna Szyk and Steven Wooding  
https://www.omnicalculator.com/other/azimuth#:~:text=You%20can%20calculate%20the%20azimuth,is%20the%20difference%20in%20longitudes  
For a description and usage of a formula to calculate azimuth angle.  

and lastly...  
www.udacity.com  
Udacity! I referenced many past lessons while creating this project.  
