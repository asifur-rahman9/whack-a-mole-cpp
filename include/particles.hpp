#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "camera.hpp"
#include "shaders.hpp"
#include <iostream>
#include <vector>

using namespace glm;
using namespace std;

struct Particle {
    vec3 position;
    vec3 velocity;
    float life;
};



class ParticleSystem{

    public:
    vector<Particle*> particles;
    float emissionRate = 100.0f;
    float timeSinceLastEmission = 0.0f;
    float lifespan;
    vec3 origin;


    ParticleSystem(vec3 initPosition);

    void update(float deltaTime);
    void render(int shaderProgram, int lightVAO, int sphereVertices);













};