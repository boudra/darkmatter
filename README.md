# [WIP] Darkmatter


Darkmatter is my pet-project game engine, the goal is to make a crossplatform engine similar to Löve that uses Javascript as it's scripting language and JSON documents as entities and component definitions.


The porject has these dependencies:

* **FreeType2** for font loading
* **GLEW** for managing OpenGL extensions
* **DevIL** for loading a wide range of image formats
* **SDL2** for cross platform input handling and window creation
* **cppformat** for 

TODO List:

* [X] Working Entity-Component-System Framework
* [X] Spritebatch, similar to the XNA one
* [X] MemoryPool for all the entities and components
* [X] Custom TypeID for loading serializing and deserializing components
* [X] Animation System
* [X] Create engine's own map format
* [ ] A simple threadpool, also make all the systems multithreaded
* [ ] Add **Google V8 Javascript engine** for dynamic script interpeting
* [ ] GUI & HUD
* ...


### How to compile


The project uses CMake to generate makefiles and project files.

```
git clone https://github.com/boudra/darkmatter.git
cd darkmatter
cmake .
make
```

I will be making a demo soon.
