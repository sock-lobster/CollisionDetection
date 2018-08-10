README.txt

PROJECT DESCRIPTION
    SpaceCollisionSystem simulates systems of many moving objects and allows 
    for comparison of several broad phase collision detection algorithms.
    The program consists of a graphical user interface (GUI) with user contorls
    and a display for visualizing the simulation. The user can select from the
    following algorithms:
        Brute Force
        Quadtree/Octree
        Spatial Index
        Spatial Hash
        Sweep and Prune
        Multidimensional Sweep and Prune
    Additionally, the user select from the following environments:
        2-dimensional square space
        3-dimensional cube space
        3-dimensional unbounded space with gravity

CONTENTS
    CODE/
        src/                                code independent of GUI library
        GUI/
            SpaceCollisionSystem/           code relating to GUI and display
        Console/
            SpaceCollisionSystemConsole/    code for running without GUI
    DOCS/
        html/
            index.html                      main page of documentation
            ...                             other documentation pages

BUILDING AND RUNNING
    Before building the code, you must install the Qt Framework and IDE, the
    Qt3D library, and the Boost C++ libraries.

    Qt Framework and IDE:
        Download and install Qt 5.1.1 from http://qt-project.org/downloads.
        You will have to download an appropriate installer for Qt 5.2.1, and
        when installing choose instead to install version 5.1.1.

    Qt3D library:
        Follow the instructions under "Option 1: Using Qt installer and
        building only Qt3D from Git" on the page
        http://qt-project.org/wiki/Qt3D-Installation.

    Boost C++ libraries:
        Download from http://www.boost.org/users/history/version_1_55_0.html
        and unzip the file. In terminal, cd to the unzipped directory. Run
        the following commands:
            sudo ./bootstrap.sh
            sudo ./b2 install
        These commands may take awhile.

    To open the project and view the code in the Qt IDE, locate the .pro file
    in CODE/GUI/SpaceCollisionSystem/ and open it. The project can be built
    and run using the green arrow button in the IDE.

    The console version may be opened in the same way and built using the
    hammer button in the IDE. It can then be run from the build directory in
    terminal.
