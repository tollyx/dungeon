# dungeon

A work-in-progress, cross-platform roguelike written in C++ using SDL2 and OpenGL.  

![Screenshot](https://i.imgur.com/aKUhgCz.png)

## Compiling

Dependencies: `SDL2`, `Lua53`, `glm`, and `glbinding`

### Linux

Install the dependencies using your favorite package manager and then use cmake.

### Windows

I heavily recommend you to use [vcpkg](https://github.com/Microsoft/vcpkg) to install the dependencies. When you've done that, either use cmake or the included solution to build with visual studio.

## Future plans

### To-Do

* [ ] Main menu
  * [ ] Character creation
  * [ ] Settings menu
  * [ ] Data editors?
* [ ] Dungeon generation
  * [x] Generate rooms & corridors
  * [x] Place entrance & exit
  * [x] Place enemies
  * [ ] Prefabs
* [ ] Items
  * [ ] Inventory
  * [ ] Consumables
  * [ ] Equipment
* [ ] Data-driven AI
* [ ] Lua scripting
  * [x] Data Loading
  * [ ] Item effects
  * [ ] Status effects
  * [ ] Dialouges?
  * [ ] Spells/Skills

### Wishlist

* [3D view similar to this experiment I made in Unity](https://imgur.com/dUfj0DX)
* Depth that rivals dwarf fortress (good luck, me!)
