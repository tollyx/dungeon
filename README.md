# dungeon

A work-in-progress, cross-platform roguelike written in C++ using SDL2 and OpenGL.  

## Compiling

### Linux

* Install SDL2 and its dev-headers using your distro's package manager
* Create a directory named  "build" and run `cmake .. && make` inside it
* Move either the resulting binary to the root folder of the repository or copy the assets folder into the build directory
* You can now run the binary

### Windows

* Download the [SDL2 windows development libraries](https://www.libsdl.org/download-2.0.php) and put them somewhere safe
* Edit the `SDL2_PATH` variable in `build.bat`
* Run `build.bat`
* Open up the resulting solution file and build it using visual studio.

You can probably get it to build using mingw or similar but I haven't tried it yet.

## Future plans

### To-Do

* [ ] A main menu
* [ ] Dungeon generation
* [ ] Make entities and AI completely data-driven
* [ ] Implement Lua for item/effects/action scripting
  * Might as well use it for basic data storage as well, since I can then skip creating my own parser or adding an additional library for json/yaml/toml/whatever.

### Wishlist

* [3D view similar to this experiment I made in Unity](https://imgur.com/dUfj0DX)
* Depth that rivals dwarf fortress (good luck, me!)
