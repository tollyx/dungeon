# dungeon

A work-in-progress, cross-platform roguelike written in C++ using SDL2 and OpenGL.  

![Screenshot](https://i.imgur.com/apzKOmm.png)

## Compiling

Dependencies: `SDL2`, `Lua53`, `glm`, and `glbinding`

### Linux

Install the dependencies using your favorite package manager and then use cmake.

### Windows

I heavily recommend you to use [vcpkg](https://github.com/Microsoft/vcpkg) to install the dependencies. When you've done that, either use cmake or the included solution to build with visual studio.

## Future plans

### To-Do

* [ ] A main menu
* [ ] Dungeon generation
* [ ] Make entities and AI completely data-driven
* [ ] Implement Lua for item/effects/action scripting
  * Might as well use it for basic data storage as well, since I can then skip creating my own parser or adding an additional library for json/yaml/toml/whatever.
  * [x] Get Lua to work
  * [ ] Initial implementation
  * [ ] Items
  * [ ] Status Effects
  * [ ] Spells

### Wishlist

* [3D view similar to this experiment I made in Unity](https://imgur.com/dUfj0DX)
* Depth that rivals dwarf fortress (good luck, me!)
