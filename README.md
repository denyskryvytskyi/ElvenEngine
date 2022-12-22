<h1 align="center"> Elven Engine 
<br/>
  
[![Build](https://github.com/denyskryvytskyi/ElvenEngine/actions/workflows/cmake.yml/badge.svg)](https://github.com/denyskryvytskyi/ElvenEngine/actions/workflows/cmake.yml)
[![License](https://img.shields.io/github/license/denyskryvytskyi/ElvenEngine)](https://github.com/denyskryvytskyi/ElvenEngine/blob/master/LICENSE)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/493f09fda55b43f68886f52be5240d27)](https://www.codacy.com/gh/denyskryvytskyi/ElvenEngine/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=denyskryvytskyi/ElvenEngine&amp;utm_campaign=Badge_Grade)
  
</h1>

Elven Engine is primarily 2D/3D game engine developing from scratch.

## Getting Started

Visual Studio 2019/2022 support.
Windows platform only support for now (Linux and MacOS platforms are for the future support)

You can clone repository using git (**`--recursive`** is required to fetch all of the submodules):

`git clone --recursive https://github.com/kryvytskyidenys/ElvenEngine`

Firstly you need to install [cmake](https://cmake.org/) 3.10+ version (3.21+ for VS 2022 support)

To generate Visual Studio solution you can run script `configure-win-vs2022` from the `script` folder.

Or just call command from the build directory with your version of VS and platform name (architecture):

`cmake -G "Visual Studio Generator 17 2022" -A x64`

There is also script for building solution (if you want to build using cmake), both Debug/Release config (pass it as argument): `build-win.bat`

## Third party libraries
| Lib |  |
| ------ | ------ |
| [cmake](https://github.com/Kitware/CMake) | build system |
| [spdlog](https://github.com/gabime/spdlog) | header-only logging library |
| [GLFW](https://github.com/glfw/glfw) | windows, OpenGL contexts and handle input |
| [glad 2](https://glad.dav1d.de/) | OpenGL loader |
| [lia](https://github.com/kryvytskyidenys/lia) | my custom math library |
| [ImGui](https://github.com/kryvytskyidenys/imgui) | library using for GUI (visual-editor) |
| [stb image](https://github.com/nothings/stb/blob/master/stb_image.h) | image loader |
| [json](https://github.com/nlohmann/json) | json lib for serialization and data-driven architecture |


## Features (In Progress)

+ [x] Logging system (spdlog)
+ [x] Event sytem (event queue based system)
+ [x] Custom math library (separate project [lia](https://github.com/denyskryvytskyi/lia))
+ [x] Renderer core (support different graphics API)
  - [x] Shader Manager (shader files loading)
  - [x] Renderer API (VAO, VBO, Shader, Texture abstractions)
  - [x] Camera (orthographic, perspective)
  - [x] DSA OpenGL renderer api implementation
+ [ ] 2D Renderer
  - [x] Quad rendering
  - [x] Texture rendering
  - [x] Batch rendering (quad and texture)
  - [x] SpriteComponent
  - [ ] Spritesheet animation
  - [ ] Text Rendering
+ [x] ECS
  - [x] Data-oriented cache-friendly components system architecture
  - [x] Components serialization
+ [x] Scene
  - [x] Scene Manager
  - [x] Scene serialization
+ [x] Data-driven architecture
  - [x] Load/Save from/to json files
+ [ ] 3D Renderer
  - [ ] Mesh/Model system
  - [ ] Primitives: cube, sphere, plane
  - [ ] Model loading (Assimp)
  - [ ] Lighting support
+ [ ] Visual-editor (based of ImGui)
  - [ ] Scene hierarchy
  - [ ] Inspector: transform component
  - [ ] Graphics stats
+ [ ] 2D Physics support (Box2d)
+ [ ] Multithreading support
  - [ ] Thread pool
+ [ ] Just cool stuff
  - [x] Fullscreen switch support
  - [x] Fly(FPS-like) 3D camera support (CameraController)
+ [ ] Allocators (separate project):
  - [ ] Stack-based: simple stack, double-ended
  - [ ] Pool allocator
  - [ ] Aligned allocations
  - [ ] Single-Frame and Double-Buffered Memory Allocators

## Demo
#### Quad and texture batch rendering (example: 20000 wizards)
![](.github/presentation/BatchRenderer_1.png)

#### 3D rendering
![](.github/presentation/3D_demo.png)

## Features (Backlog)
- Renderer
    - Deffered Rendering
    - PBR Rendering
    - Vulkan, DirectX 11/12 support
- 3D Physics (Bullet)
- Audio engine
- Linux and Mac support
- Procedural terrain and world generation
- Artificial Intelligence
