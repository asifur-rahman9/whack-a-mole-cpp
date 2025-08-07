
// Standard library includes
#include <iostream>
#include <chrono>
#include <string>

// OpenGL includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Math library includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

// Image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// Local includes
#include "constants.hpp"
#include "graphics.hpp"
#include "input.hpp"
#include "game.hpp"
#include "arm.hpp"
#include "camera.hpp"
#include "shaders.hpp"
#include "loadObj.hpp"

// Using statements
using namespace glm;
using namespace std;

GLuint setupModelVBO(string path, int& vertexCount);

// main function
int main(int argc, char *argv[])
{
    // Initialization
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Project 1", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Disable mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Load Textures
    // GLuint brickTextureID = loadTexture("textures/brick.jpg");
    GLuint grassTextureID = loadTexture("textures/grass.png");
    GLuint woodTextureID = loadTexture("textures/wood.png");
    GLuint cementTopTextureID = loadTexture("textures/cementTop.png");
    GLuint cementBaseTextureID = loadTexture("textures/cementBase.png");
    GLuint metalTextureID = loadTexture("textures/metal.png");

    // Sky blue background for sky
    glClearColor(0.23f, 0.21f, 0.62f, 1.0f);

    //enabling blending
    glEnable(GL_BLEND); 	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Compile and link shaders here ...
   // Shader colorShader("./shaders/vertexShader.glsl","./shaders/fragmentShader.glsl");
    Shader textureShader("./shaders/textureVertex.glsl","./shaders/textureFragment.glsl");
    Shader lightShader("./shaders/lightingVertex.glsl","./shaders/lightingFragment.glsl");
    Shader skyboxShader("./shaders/skyboxVertex.glsl","./shaders/skyboxFragment.glsl");
    Shader shadowShader("./shaders/shadowVertex.glsl","./shaders/shadowFragment.glsl");

    //int colorShaderProgram = colorShader.ID;
    int texturedShaderProgram = textureShader.ID;
    int lightingShaderProgram = lightShader.ID;
    int skyboxShaderProgram = skyboxShader.ID;
    int shadowShaderProgram = shadowShader.ID;


    // load object models
    // Setup models
    string spherePath = "models/sphere.obj";
    
    // load the sphere coordinates
    int sphereVertices;
    GLuint sphereVAO = setupModelVBO(spherePath, sphereVertices);

    // load the skybox cube
    vector<std::string> faces = {
    "textures/skybox/right.jpg",
    "textures/skybox/left.jpg",
    "textures/skybox/top.jpg",
    "textures/skybox/bottom.jpg",
    "textures/skybox/front.jpg",
    "textures/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces); 
    

    // We can set the shader once, since we have only one
    glUseProgram(texturedShaderProgram);

    // Initialize camera with default parameters
    Camera camera;

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(FIELD_OF_VIEW,                       // field of view in degrees
                                             (float)WINDOW_WIDTH / WINDOW_HEIGHT, // aspect ratio
                                             NEAR_PLANE, FAR_PLANE);              // near and far (near > 0)

    

    // Set initial view matrix using camera
    mat4 viewMatrix = camera.getViewMatrix();

    // Set View and Projection matrices on both shaders
    camera.updateViewMatrix(texturedShaderProgram);
    camera.updateViewMatrix(lightingShaderProgram);
    
    setProjectionMatrix(texturedShaderProgram, projectionMatrix);
    setProjectionMatrix(lightingShaderProgram, projectionMatrix);

    // Define and upload geometry to the GPU here ...
    int vao = createTexturedCubeVertexArrayObject();

    //create the skybox vertex data
    unsigned int skyboxVAO, skyboxVBO;
    generateSkyboxVertices( &skyboxVAO, &skyboxVBO);

    // create the depth map
    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // For frame time
    float lastFrameTime = glfwGetTime();

    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    // Parameters to control robotic arm
    float bicepAngle = 25.0f;  // relative to vertical
    float forearmAngle = 0.0f; // relative to bicep
    float bicepLength = BICEP_LENGTH;
    float forearmLength = FOREARM_LENGTH;
    float baseRotation = 0.0f; // rotation around Y-axis

    // parameters for our target cube
    bool newCube = true;
    float cubeX = 5.f; // this is our displacement
    float cubeY = 5.f;
    float cubeRad = 25.f;
    float cubeRot = 0.0f;
    // float cubeZ = 25.f;

    int points = 0;

    // variables to keep track of passage of time for cube
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    unsigned long millis = (unsigned long)milliseconds;
    unsigned long reset = CUBE_RESET_TIME_MS;
    unsigned long baseTime = millis;

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        


        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the skybox first
        //renderSkybox(camera, skyboxShaderProgram, skyboxShader, projectionMatrix, skyboxVAO, cubemapTexture);
        

        // keeping track of time for adjustment with time purposes
        now = std::chrono::system_clock::now();
        duration = now.time_since_epoch();
        milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        millis = (unsigned long)milliseconds;

        // Update game logic (cube timing and positioning)
        updateGameLogic(newCube, cubeRad, cubeRot, cubeY, millis, baseTime, reset);

        //update lighting parameters
        const int LIGHT_NUMBR = 5;
        vec3* lightPos = setLights(LIGHT_NUMBR, texturedShaderProgram, textureShader, millis, lightingShaderProgram);

        // set cameraPosition
        textureShader.setVec3("viewPos", camera.getPosition());

        //render the Shadow map !!
        glCullFace(GL_FRONT);
        
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 300.0f;
        //float fov = glm::radians(160.0f); // or another suitable value
        //float aspect = 1.0f; // shadow map is usually square
        //lightProjection = glm::perspective(fov, aspect, near_plane, far_plane);
        lightProjection = glm::ortho(-100.f, 100.f, -100.f, 100.f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos[0], glm::vec3(5.0f, 0.0f, 5.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        // 2. Use shadow shader
        shadowShader.use();
        // Set lightSpaceMatrix uniform (projection * view from light's POV)
        shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        // Render scene with shadow shader
        renderScene(grassTextureID, cementTopTextureID, cementBaseTextureID, woodTextureID,
                    metalTextureID, shadowShaderProgram, vao, baseRotation, bicepAngle,
                    forearmAngle, bicepLength, forearmLength, cubeX, cubeY, cubeRad, cubeRot);

        




        // Render the entire scene with regular shaders
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //
        // Set projection matrix for shader, this won't change
        projectionMatrix = glm::perspective(FIELD_OF_VIEW,                       // field of view in degrees
                                             (float)WINDOW_WIDTH / WINDOW_HEIGHT, // aspect ratio
                                             NEAR_PLANE, FAR_PLANE);              // near and far (near > 0)

    

        // Set initial view matrix using camera
        viewMatrix = camera.getViewMatrix();

        // set cameraPosition
        textureShader.setVec3("viewPos", camera.getPosition());

        // Set View and Projection matrices on both shaders
        camera.updateViewMatrix(texturedShaderProgram);
        camera.updateViewMatrix(lightingShaderProgram);
    
        setProjectionMatrix(texturedShaderProgram, projectionMatrix);
        setProjectionMatrix(lightingShaderProgram, projectionMatrix);

        // 1. Set viewport to window size
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        glViewport(0, 0, fbWidth, fbHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the skybox first
        renderSkybox(camera, skyboxShaderProgram, skyboxShader, projectionMatrix, skyboxVAO, cubemapTexture);

        // draw the lights
        drawLights(LIGHT_NUMBR, lightingShaderProgram, sphereVAO, sphereVertices, lightPos, texturedShaderProgram, textureShader);

        // 2. Use main shader
        textureShader.use();
        textureShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        textureShader.setInt("shadowMap", 1);


        renderScene(grassTextureID, cementTopTextureID, cementBaseTextureID, woodTextureID,
                    metalTextureID, texturedShaderProgram, vao, baseRotation, bicepAngle,
                    forearmAngle, bicepLength, forearmLength, cubeX, cubeY, cubeRad, cubeRot);

        
        
                    
       
        // Check for collision between hammer and cube
        vec3 hammerWorldPos = calculateHammerPosition(baseRotation, bicepAngle, forearmAngle, bicepLength, forearmLength);
        vec3 cubePosition = vec3(cubeX + (cubeRad * sin(cubeRot)), cubeY, cubeRad * cos(cubeRot));

        if (checkCollision(hammerWorldPos, cubePosition, baseRotation))
        {
            cout << "Hit!" << endl;
            newCube = true;
            baseTime = millis;
            points = points + 1;
            cout << "Current score: " << points << " points!" << endl;
        }

        

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (handleGeneralInput(window))
            glfwSetWindowShouldClose(window, true);

        // Handle camera input and movement using the camera object
        camera.handleInput(window, dt);

        // Handle robotic arm controls
        handleRobotArmInput(window, bicepAngle, forearmAngle, baseRotation);

        // Update view matrix using camera
        camera.updateViewMatrix(texturedShaderProgram);
        camera.updateViewMatrix(lightingShaderProgram);
    }
    cout << "Total points: " << points << "!!" << endl;

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}

GLuint setupModelVBO(string path, int& vertexCount) {
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> UVs;

  // read the vertex data from the model's OBJ file
  loadOBJ(path.c_str(), vertices, normals, UVs);

  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);  // Becomes active VAO
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and
  // attribute pointer(s).

  // Vertex VBO setup
  GLuint vertices_VBO;
  glGenBuffers(1, &vertices_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
               &vertices.front(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid*)0);
  glEnableVertexAttribArray(0);

  // Normals VBO setup
  GLuint normals_VBO;
  glGenBuffers(1, &normals_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
               &normals.front(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid*)0);
  glEnableVertexAttribArray(1);

  // UVs VBO setup
  GLuint uvs_VBO;
  glGenBuffers(1, &uvs_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
  glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
                        (GLvoid*)0);
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent
  // strange bugs, as we are using multiple VAOs)
  vertexCount = vertices.size();
  return VAO;
}

