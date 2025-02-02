<h1 align="center"> Elven Engine 
<br/>

[![Build](https://github.com/denyskryvytskyi/ElvenEngine/actions/workflows/cmake.yml/badge.svg)](https://github.com/denyskryvytskyi/ElvenEngine/actions/workflows/cmake.yml)
[![CodeQL](https://github.com/denyskryvytskyi/ElvenEngine/actions/workflows/codeql.yml/badge.svg)](https://github.com/denyskryvytskyi/ElvenEngine/actions/workflows/codeql.yml)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](https://github.com/denyskryvytskyi/ElvenEngine/blob/master/LICENSE)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/493f09fda55b43f68886f52be5240d27)](https://www.codacy.com/gh/denyskryvytskyi/ElvenEngine/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=denyskryvytskyi/ElvenEngine&amp;utm_campaign=Badge_Grade)

</h1>

Elven Engine is primarily a 2D/3D game engine that is being developed from scratch.

## Features
+ [x] Logging system
+ [x] Event system (non-blocking event queue-based system, described architecture in the [article](https://denyskryvytskyi.github.io/event-system))
+ [x] Custom 3D math library ([lia](https://github.com/denyskryvytskyi/lia))
+ [x] Renderer Core
  - [x] RHI (Render Hardware Interface): Buffers, Shader, Texture, Render Target
  - [x] Graphics API agnostic Renderer API
  - [x] Modern OpenGL (with Direct State Access) specific Renderer API implementation
  - [x] Shader loading
  - [x] Async texture loading
  - [x] Camera (orthographic, perspective)
  - [x] Render Passes based on Render Targets
+ [ ] 2D Renderer
  - [x] Quad rendering
  - [x] Texture rendering
  - [x] Batch rendering (quad and texture)
  - [x] Text Rendering
  - [ ] Spritesheet animation
+ [ ] 3D Renderer (WIP)
  - [x] Phong/Blinn-Phong Lighting
  - [x] Multiple light casters: spotlight, directional, and point light
  - [x] Mesh-Material system (Static mesh support with one material slot per mesh/submesh for now)
  - [x] Primitives: cube, sphere, plane
  - [x] Async 3D model loading and async material textures loading
  - [x] Render Target
  - [x] MSAA (Multisample anti-alising)
  - [x] Post-processor: blur with custom mask texture building algorithms
  - [ ] Uniform buffer
  - [ ] Cubemap
  - [ ] Shadows
+ [x] ECS (investigated different techniques and my particular architecture in the [article](https://denyskryvytskyi.github.io/ecs))
  - [x] Data-oriented cache-friendly components system architecture
  - [x] In-Engine components: Transform, Sprite, Text, Quad, StaticMesh, Camera, UITransform, AABB, Tag, Sound, PointLight/DirectionalLight/SpotLight.
  - [x] Components serialization
  - [x] Behavior component as the entity brain
  - [x] Optional Systems for usability
+ [x] Scene
  - [x] Interface to work with ECS
  - [x] Scene Graph based on SceneNodeComponent
  - [x] Scene serialization (not all of the components now)
+ [x] Data-driven architecture
  - [x] Load/Save engine settings from/to JSON files
+ [x] Sound engine
  - [x] Support formats: mp3, wav, ogg, flac etc.
  - [x] Add/Play/Pause/Stop/Loop functionality
+ [ ] In-engine editor (WIP; based on ImGui)
  - [x] Editor Camera (holding RMB): WASD movement, QE up/down, ZX rotation
  - [x] Scene hierarchy panel
  - [x] Context menu to add/delete entity
  - [x] Properties panel: 
    - [x] Components: Transform, Static Mesh with material, Point/Directional/Spot light, UI transform, Text, Sprite
    - [x] "Add component" button
    - [x] Component settings context menu
  - [x] Settings panel: fullscreen, VSync, MSAA
  - [x] Telemetry: performance panel
  - [ ] Graphics stats
+ [x] Just cool stuff
  - [x] Fullscreen switch support
  - [x] Orthographic camera controller (OrthographicCameraController), that can be used if needed
  - [x] Fly(FPS-like) 3D camera support (CameraController)
+ [ ] Multithreading support
  - [x] async resources loading: texture, mesh
  - [ ] Thread pool

## Demo
### 3D rendering
Demo video on youtube (click on image):
[<img src=".github/demo/demo.png">](https://www.youtube.com/watch?v=YhUVGp8Mb54)

### 2D Rendering
**Space Invaders full demo**

https://github.com/denyskryvytskyi/ElvenEngine/assets/25298585/f0d32c8b-1ed2-44e3-9c93-a781f3da4203

| Space Invades  | TRON |
| --- | --- |
| <img src=".github/demo/invaders.gif"> | <img src=".github/demo/tron.gif"> |
| Ping Pong | Quad and texture batch rendering (20000 wizards) |
| <img src=".github/demo/pong.gif"> | <img src=".github/demo/wizards.gif"> |

## Getting Started

Windows platform only support for now (Linux and MacOS platforms are for future support)

You can clone the repository using git (**`--recursive`** is required to fetch all of the submodules):

`git clone --recursive https://github.com/kryvytskyidenys/ElvenEngine`

Firstly you need to install [cmake](https://cmake.org/) 3.10+ version (3.21+ for VS 2022 support).

### Windows
1. Configure and build third-party libraries: `scripts/setup-vendor.bat`
2. Configure Visual Studio solution for the Engine and Game/Example projects: `scripts/configure-vs2022`. 
3. Build a solution using one of the following ways:
   - cmake-based script `scripts/build.bat` (pass Debug/Release as an argument)
   - Visual Studio (`ElvenEngine.sln` file is placed in the `build` directory).

### CMake build options
You can modify configure file to enable/disable the following cmake options:
- **BUILD_SANDBOX** (default ON): Enable Sandbox2D and Sandbox3D projects build
- **BUILD_GAMES** (default ON): Enable Games build
- **PROFILE_MODE** (default ON): Enable Profiling (`PROFILE_MODE` preprocessor definition will be added)
- **MODULE_EDITOR_ENABLED** (default ON): Enable Editor (`MODULE_EDITOR_ENABLED` preprocessor definition will be defined)
- **MODULE_3D_ENABLED** (default ON): Enable 3D module and Assimp library for 3D model loading (`MODULE_3D_ENABLED` preprocessor definition will be defined). **IMPORTANT**: ON - only Sandbox3D project will be configured, OFF - only Sandbox2D and Games projects will be configured.
- **MODULE_SOUND_ENABLED** (default ON): Enable sound module (`MODULE_SOUND_ENABLED` preprocessor definition will be defined)

## Third-party libraries
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
| [irrKlang](https://www.ambiera.com/irrklang/) | sound library |
| [fmt](https://github.com/fmtlib/fmt) | formatting library |
| [freetype](https://freetype.org/) | fonts rendering |
| [assimp](https://github.com/assimp/assimp) | 3D model loading |
