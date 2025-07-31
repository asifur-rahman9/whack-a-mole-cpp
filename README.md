Elizabeth O'Meara 40065959

Asifur Rahman 40194283


# cppGraphics
A C++ OpenGL project for COMP371 that renders a whack-a-mole style game with a hammer and target cube

## Features
- 3D geometry 
- Lighting 
- Camera controls
- Arm with two levels of hierarchy (bicep + forearm)
- Basic game logic: hit the target with the arm to score points

## Controls
- Q and E to turn arm clockwise and counter-clockwise
- Left and right arrow to move bicep of arm
- Up and down arrow to move forearm
- WSAD to move the camera forward, backwards, left and right
- C and Space to move camera up and down vertically

## Structure
- `src/` — C++ source files
- `include/` — Header files
- `shaders/` — GLSL shader files


## Description
The scene shows a robotic arm that can be controlled to hit a target that disappears and reappears. Lighting and shading are handled with GLSL shaders. The project demonstrates graphics programming concepts like transformation matrices, normal vectors, lighting, and articulating objects.

## Teamwork
This project was done collaboratively, especially with pair programming where we both looked at the code and wrote modifications / new features together.

## References
This project used code from the tutorials, notable for the main loop and texture implementation, although we adapted and split everything up to meet the needs of the project. We also referenced LearnOpenGL to add the lighting and to split out the shader (glsl) files.

