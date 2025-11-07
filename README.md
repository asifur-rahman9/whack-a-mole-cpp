# cppGraphics
A C++ OpenGL project for COMP371 that renders a target catching game with a hammer and target cube. Project was done on MacOS

## Features
- 3D geometry with textures and complex models (trees, grass)
- Lighting and shadows
- Camera controls
- Arm with two levels of hierarchy (bicep + forearm)
- Basic game logic: hit the target to score points - target will explode into particles
- Hammer will automatically attack the player - player will lose points and be knocked back if hit

## Controls
- WSAD to move the camera forward, backwards, left and right
- Mouse controls to determine direction

## Structure
- `src/` — C++ source files
- `include/` — Header files
- `shaders/` — GLSL shader files
- `models/` - Complex models and other geometry


## Description
The scene shows a hammer arm that will move to hit a player that is chasing a target. Lighting and shading are handled with GLSL shaders. The project demonstrates graphics programming concepts like transformation matrices, normal vectors, lighting, particles, interaction, and articulating objects.

## Teamwork
This project was done collaboratively, especially with pair programming where we both looked at the code and wrote modifications / new features together.

## References
This project used code from the tutorials, notable for the main loop and texture implementation, although we adapted and split everything up to meet the needs of the project. We also referenced LearnOpenGL to add the lighting and to split out the shader (glsl) files.

## Compilation
Compilation instructions for MacOS can be found in the compile.txt file in the main directory
