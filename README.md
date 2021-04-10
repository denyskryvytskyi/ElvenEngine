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
| [Glad](https://glad.dav1d.de/) | OpenGL loader |
| [Deat ImGui](https://github.com/kryvytskyidenys/imgui) | library using for GUI (visual-editor) |
| [GDM](https://github.com/kryvytskyidenys/gdm) | my custom 3D mathematics library |

## Features (In progress)

- Logging system and assertions (using spdlog lib)
- Allocators:
  + Stack-based: simple stack, double-ended
  + Pool allocator
  + Aligned allocations
  + Single-Frame and Double-Buffered Memory Allocators
- Event sytem (using modern c++ and event bus pattern)
- Input Handler System (using glfw and custom event system)
- Custom math library (as separate project)
- Graphics core:
  + Shader Manager (shader files loading)
  + Renderer API (VAO, VBO, Shader abstractions)
  + Camera (orthographic, perspective)




## Features (To-do)

- Renderer 
  - Architecture should support usage of different graphics API (OpenGL only for now)
  - 2D renderer (sprites, 2D spritesheet animation)
  - Material system
  - 3D renderer:
    - Deffered/Forward Rendering
    - PBR Rendering
  - Text rendering
- Enitity-component based system
- Visual-editor (based of ImGui)
- Scripting Engine
- Physics support:
  - 2D - Box2d
  - 2D - Bullet Physics Library (Bullet)

## Backlog
- Audio engine
- Support for Mac, Linux
- DirectX 11, DirecX 12, Vulkan support
- Procedural terrain and world generation
- Artificial Intelligence
