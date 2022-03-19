# Elven Engine
Elven Engine is primarily 2D/3D real-time development platform and rendering engine

## Getting Started
Visual Studio 2017 or 2019. 
Windows platform only support for now (MacOS and Linux platforms are for the future support)

You can clone repository using git (**`--recursive`** is required to fetch all of the submodules):

`git clone --recursive https://github.com/kryvytskyidenys/ElvenEngine`

Run GenerateProject.bat file from the scripts folder to build the solution.

## Third party libraries
| Lib |  |
| ------ | ------ |
| [premake](https://github.com/premake/premake-core) | build configuration |
| [spdlog](https://github.com/gabime/spdlog) | header-only logging library |
| [GLFW](https://github.com/kryvytskyidenys/glfw) | windows, OpenGL contexts and handle input |
| [glad](https://glad.dav1d.de/) | OpenGL loader |
| [lia](https://github.com/kryvytskyidenys/lia) | my custom linear algebra library |
| [Deat ImGui](https://github.com/kryvytskyidenys/imgui) | library using for GUI (visual-editor) |


## Features

- [x] Logging system and assertions (using spdlog lib)
- [ ] Allocators:
  + [ ] Stack-based: simple stack, double-ended
  + [ ] Pool allocator
  + [ ] Aligned allocations
  + [ ] Single-Frame and Double-Buffered Memory Allocators
- [x] Event sytem
- [x] Custom math library (as separate project)
- [ ] Renderer core:
  + [ ] Shader Manager (shader files loading)
  + [ ] Renderer API (VAO, VBO, Shader abstractions)
  + [ ] Camera (orthographic, perspective)
  + [ ] Support usage of different graphics API (OpenGL only for now)

## Features (To-do)
- Renderer 
  - 2D renderer (sprites, 2D spritesheet animation)
  - Text rendering
- Enitity-component-system
- Visual-editor (based of ImGui)
- 2D Physics support (Box2d)

## Backlog
- Renderer
  - Material system
  - 3D renderer:
    - Deffered/Forward Rendering
    - PBR Rendering
- Scripting Engine
- 3D Physics Library (Bullet)
- Audio engine
- Support for Mac, Linux
- DirectX 11, DirecX 12, Vulkan support
- Procedural terrain and world generation
- Artificial Intelligence
