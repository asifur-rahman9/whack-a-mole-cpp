// OpenGL headers first (GLEW before GLFW)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.hpp"
#include "constants.hpp"
#include "graphics.hpp"
#include "particles.hpp"
#include "shaders.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;
using namespace glm;


ParticleSystem::ParticleSystem(vec3 initPosition){
    // particle system only renders when a cube has been hit
    lifespan = 0.f;
    origin = initPosition;
    
}

void ParticleSystem::update(float deltaTime){

    // generate particles up to the specified number (100)
    int newChance = rand() % 100;
    if((particles.size() < 100) & (newChance > 10)){
        Particle *newParticle = new Particle;
        newParticle->life = 0.5f;
        newParticle->position = origin;
        newParticle->velocity.x = float(rand() % 100) / 20.f - 2.5;  //velocity between -2.5 and 2.5
        newParticle->velocity.z = float(rand() % 100) / 20.f - 2.5;  //velocity between -2.5 and 2.5
        newParticle->velocity.y = float(rand() % 100) / 20.f - 1.f;  //velocity between -1 and 4
        particles.push_back(newParticle);
        
    }

    //update the position of each particle, reset the position to the origin if life is over
    for(Particle *particle : particles){
        particle->position += particle->velocity * deltaTime;
        particle->life -= deltaTime;
        if(particle->life < 0.001){
            particle->position = origin;
            particle->life = 0.5f;
        }
    }

    // decrease the lifespace of the particle system
    lifespan -= deltaTime;
    

}


void ParticleSystem::render(int shaderProgram, int lightVAO, int sphereVertices){
    
    // use the sphere VAO and lighting program
    glBindVertexArray(lightVAO);
    glUseProgram(shaderProgram);

    // Draw each of the particles
    for(const auto& particle: particles){
        
        // Draw each of the particles, with lower opacaity for lower life remaining
        glUniform1f(glGetUniformLocation(shaderProgram, "alpha"), 2 * particle->life);
        mat4 lightWorldMatrix = translate(mat4(1.0f), particle->position) * scale(mat4(1.0f), vec3(0.005f, 0.005f, 0.005f));
        setWorldMatrix(shaderProgram, lightWorldMatrix);
        glDrawArrays(GL_TRIANGLES, 0, sphereVertices); // counting the sphere vertices

    }

}