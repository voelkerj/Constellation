# CONSTELLATION

**CHOSEN PROJECT**
For this Capstone I chose Option 1: Create a project of my choice.
Please see the "About" section for more details.

**TO BUILD**
1). Two external libraries are needed: SDL2 and SDL_Image.

To Install SDL2:
Linux: Open terminal and run command 
apt-get install libsdl2-dev
Other: More installation instructions can be found on the SDL_image installation page at https://wiki.libsdl.org/SDL2/Installation

To Install SDL_Image:
Linux: Open terminal and run command 
apt-get install libsdl2-image-dev
Other: I recommend the installation instructions outlined on the page https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/index.php

2.) cd into the project directory
3.) mkdir build
4.) cd build
5.) cmake ..
6.) make

**TO RUN**
1.) Copy input.txt and satellites.txt from the "examples" folder into the build directory
2.) cd into the build folder and run ./Constellation

The example files provided have some interesting and notable satellites preset for you. You can simply modify them to create your own. You can also comment out lines using the # character. Note that each satellite must be a complete block, you cannot put inputs for one satellite in between inputs for another.

For setting satellite colors, I recommend pulling the RGB values from the following site: https://htmlcolorcodes.com/color-picker/

**ABOUT**
Hello! Thank you for evaluating my project!
Constellation is a satellite orbit propagator. It assumes a spherical, rotating Earth and displays the ground tracks of propagated orbits on a 2D map.

Inputs are read from two text files. Inputs.txt controls simulation parameters such as start time and end time. Satellites.txt is where the user can define satellites. They can list as many as they desire and the propagator will animate them all at once. A custom color can be set by the user to differentiate each satellite.

This is a true propagator. It uses math and physics to propagate orbits. It does not take shortcuts nor does it call external libraries to do this. For more on how it works, you may see the PROPAGATOR DETAILS below.

**FILE AND CLASS STRUCTURE**
All code is in the "src" folder. All images are in the "resources" folder. Once the user creates the build folder (see section "TO RUN"), they must place an inputs.txt and a satellites.txt in this folder (again, see section "TO RUN"). The other notable folder is cmake_modules, which contains a .cmake file that locates the SDL2_image library on the system.

main.cpp            - Contains the main() function. Kicks off satellite simulation and manages rendering/animation.
satellite.cpp/.h    - Contains the satellite class, including getters/setters, and functions such as simulate().
math_helpers.cpp/.h - Utility functions to do mathematical conversions.
input_parser.cpp/.h - Controls the reading of input files.
cleanup.h           - For deleting SDL elements.
error.h             - For displaying SDL errors.
resource_path.h     - For dynamically generating the path to the resource folder.
texture.h           - For loading and configuring SDL textures.

**SELECTED CRITERIA**
1.) The project reads data from a file and processes the data, or the program writes data to a file
- input_parser.cpp/.h reads from two text files and is called in main.cpp on lines 23 and 26
2.) The project uses Object Oriented Programming techniques
- satellite.cpp/.h define the satellite object
- This object has getters and setters, public and private member variables, and member functions
- The getters and setters have protection built in to stop invalid inputs
- Satellites are instantiated on line 63 of input_parser.cpp
3.) The project makes use of references in function declarations
- Many functions use references. See satellite.h lines 27 to 38 for examples.
4.) The project uses multithreading
- The satellites are propagated using the simulate() function
- This function is kicked off using threads in main.cpp, line 31
5.) A mutex or lock is used in the project
- A mutex is used to protect terminal write statements. Examples are on lines 391 and 405 of satellite.cpp

**OTHER CRITERIA**
Below is extra criteria I feel this project meets
1.) The project demonstratees an understanding of C++ functions and control structures
2.) Classes use appropriate access specifiers for class members
3.) Classes encapsulate behavior

**PROPAGATOR DETAILS**
The user inputs the initial state of the satellite in Keplerian elements. The satellite is then propagated forward for each timestep. The orbital state at each point is converted to Earth-Centered Intertial (ECI) coordinates, then to Earth-Centered Fixed (ECF) coordinates. This ECI-to-ECF conversion is what accounts for the rotation of the Earth. From there, the ECF coordinates are converted to Latitude/Longitude/Altitude. Since this is a 2D display, we ignore the altitude and continue by converting the lat/lon coordinates into screen X and Y pixel coordinates.
The animation runs at the FPS that is set by the user in inputs.txt. If not enough time has passed since the next frame must be displayed, the system will sleep up to the time the next loop iteration is supposed to start.

**FINAL NOTE**
You may notice some functions that have been written, but are never called. This code is for future additions I have planned for Constellation.

**ACKNOWLEDGEMENTS**
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