# Elven Engine
Game engine from scratch.

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

## Features

- Logging system and assertions
- Event sytem
- Input Handler System
- Custom math library
- Shader Manager (shader files loading)
- Material system
- Renderer architecture aimed to support different graphics API (OpenGL only for now)
- Orthographic camera

## Features (To-do)
- Events queue
- 2D renderer
- Enitity-component based system
- visual-editor
- 2D spritesheet animation
- Scripting Engine
- 2D physics engine (Box2d)
- Text rendering

## Features (Backlog)
- 3D renderer:
  + Deffered/Forward Rendering
  + PBR Rendering
- Audio engine
- Support for Mac, Linux
- DirectX 11, DirecX 12, Vulkan support
- Procedural terrain and world generation
- Artificial Intelligence
