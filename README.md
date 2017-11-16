# dungeon

A work-in-progress, cross-platform roguelike written in C++ using SDL2 and OpenGL.  

![Screenshot](https://i.imgur.com/apzKOmm.png)

## Compiling

### Linux

* Install SDL2, Lua53 and GLEW using your distro's package manager
* Create a directory named  "build" and run `cmake .. && make` inside it
* Move either the resulting binary to the root folder of the repository or copy the assets folder into the build directory
* You can now run the binary

### Windows

I really need to fix a hassle-free way to build this shit on windows.

Currently, you need to import the source as a new project in visual studio and set up the dependencies yourself. (SDL2, Lua53, GLEW, OpenGL)

## Future plans

### To-Do

* [ ] A main menu
* [ ] Dungeon generation
* [ ] Make entities and AI completely data-driven
* [ ] Implement Lua for item/effects/action scripting
  * Might as well use it for basic data storage as well, since I can then skip creating my own parser or adding an additional library for json/yaml/toml/whatever.
  * [x] Initial implementation
  * [ ] Items
  * [ ] Status Effects
  * [ ] Spells

### Wishlist

* [3D view similar to this experiment I made in Unity](https://imgur.com/dUfj0DX)
* Depth that rivals dwarf fortress (good luck, me!)
