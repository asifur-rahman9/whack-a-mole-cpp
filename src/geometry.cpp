#include "graphics.hpp"
#include <iostream>
#include <cassert>
#include <stb/stb_image.h>

// Example normals for each face (you may want to adjust these for your cube)
const vec3 normals[] = {
    vec3(-1.0f, 0.0f, 0.0f), // left
    vec3(0.0f, 0.0f, -1.0f), // far
    vec3(0.0f, -1.0f, 0.0f), // bottom
    vec3(0.0f, 0.0f, 1.0f),  // near
    vec3(1.0f, 0.0f, 0.0f),  // right
    vec3(0.0f, 1.0f, 0.0f)   // top
};

// Textured Cube model with position, color, UV, and normal
const TexturedColoredNormalVertex texturedCubeVertexArray[] = {
    // left - red
    TexturedColoredNormalVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), normals[0]),
    TexturedColoredNormalVertex(vec3(-0.5f, -0.5f, 0.5f),  vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f), normals[0]),
    TexturedColoredNormalVertex(vec3(-0.5f, 0.5f, 0.5f),   vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), normals[0]),

    TexturedColoredNormalVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), normals[0]),
    TexturedColoredNormalVertex(vec3(-0.5f, 0.5f, 0.5f),   vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), normals[0]),
    TexturedColoredNormalVertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f), normals[0]),

    // far - blue
    TexturedColoredNormalVertex(vec3(0.5f, 0.5f, -0.5f),   vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), normals[1]),
    TexturedColoredNormalVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), normals[1]),
    TexturedColoredNormalVertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), normals[1]),

    TexturedColoredNormalVertex(vec3(0.5f, 0.5f, -0.5f),   vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), normals[1]),
    TexturedColoredNormalVertex(vec3(0.5f, -0.5f, -0.5f),  vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), normals[1]),
    TexturedColoredNormalVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), normals[1]),

    // bottom - turquoise
    TexturedColoredNormalVertex(vec3(0.5f, -0.5f, 0.5f),   vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), normals[2]),
    TexturedColoredNormalVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f), normals[2]),
    TexturedColoredNormalVertex(vec3(0.5f, -0.5f, -0.5f),  vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f), normals[2]),

    TexturedColoredNormalVertex(vec3(0.5f, -0.5f, 0.5f),   vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), normals[2]),
    TexturedColoredNormalVertex(vec3(-0.5f, -0.5f, 0.5f),  vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f), normals[2]),
    TexturedColoredNormalVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f), normals[2]),
        // near - green
    TexturedColoredNormalVertex(vec3(-0.5f, 0.5f, 0.5f),   vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), normals[3]),
    TexturedColoredNormalVertex(vec3(-0.5f, -0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), normals[3]),
    TexturedColoredNormalVertex(vec3(0.5f, -0.5f, 0.5f),   vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), normals[3]),

    TexturedColoredNormalVertex(vec3(0.5f, 0.5f, 0.5f),    vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), normals[3]),
    TexturedColoredNormalVertex(vec3(-0.5f, 0.5f, 0.5f),   vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), normals[3]),
    TexturedColoredNormalVertex(vec3(0.5f, -0.5f, 0.5f),   vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), normals[3]),

    // right - purple
    TexturedColoredNormalVertex(vec3(0.5f, 0.5f, 0.5f),    vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), normals[4]),
    TexturedColoredNormalVertex(vec3(0.5f, -0.5f, -0.5f),  vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), normals[4]),
    TexturedColoredNormalVertex(vec3(0.5f, 0.5f, -0.5f),   vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), normals[4]),

    TexturedColoredNormalVertex(vec3(0.5f, -0.5f, -0.5f),  vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), normals[4]),
    TexturedColoredNormalVertex(vec3(0.5f, 0.5f, 0.5f),    vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), normals[4]),
    TexturedColoredNormalVertex(vec3(0.5f, -0.5f, 0.5f),   vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), normals[4]),

    // top - yellow
    TexturedColoredNormalVertex(vec3(0.5f, 0.5f, 0.5f),    vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), normals[5]),
    TexturedColoredNormalVertex(vec3(0.5f, 0.5f, -0.5f),   vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), normals[5]),
    TexturedColoredNormalVertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), normals[5]),

    TexturedColoredNormalVertex(vec3(0.5f, 0.5f, 0.5f),    vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), normals[5]),
    TexturedColoredNormalVertex(vec3(-0.5f, 0.5f, -0.5f),  vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), normals[5]),
    TexturedColoredNormalVertex(vec3(-0.5f, 0.5f, 0.5f),   vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), normals[5])
};


// Textured Cube model with position, color, and UV coordinates
const TexturedColoredVertex oldTexturedCubeVertexArray[] = {                                       // position,                            color
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)), // left - red
    TexturedColoredVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),

    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // far - blue
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // bottom - turquoise
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)), // near - green
    TexturedColoredVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // right - purple
    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)), // top - yellow
    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f))};

GLuint loadTexture(const char *filename)
{
    // load textures with dimension data
    int width, height, nbChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nbChannels, 0);
    if (!data)
    {
        std::cerr << "Error loading texture file:" << filename << std::endl;
        return 0;
    }

    // create and bind textures
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    assert(textureId != 0);

    glBindTexture(GL_TEXTURE_2D, textureId);

    // set filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // upload texture to GPU
    GLenum format = 0;
    if (nbChannels == 1)
        format = GL_RED;
    else if (nbChannels == 3)
        format = GL_RGB;
    else if (nbChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // free resource
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureId;
}

// creating the cube texture cube vertex array object
int createTexturedCubeVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);                // position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))); // color
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))); // uv
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float))); // normal
    glEnableVertexAttribArray(3);

    return vertexArrayObject;
}

// setting the projection matrix
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}

// setting the view matrix
void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

// setting the world matrix
void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

// loading the maop for the skybox
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    return textureID;
}  




// creating the cube for the skybox
void generateSkyboxVertices( unsigned int* skyboxVAO, unsigned int* skyboxVBO){
    float skyboxVertices[108] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
    };
    // creating the skybox VAO
    //unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, skyboxVAO);
    glGenBuffers(1, skyboxVBO);
    glBindVertexArray( *skyboxVAO );
    glBindBuffer(GL_ARRAY_BUFFER, *skyboxVBO );
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}