
// Standard library includes
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cassert>

// OpenGL includes
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Math library includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

// Image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// Using statements
using namespace glm;
using namespace std;

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float CAMERA_SPEED = 5.0f;
const float CAMERA_FAST_SPEED_MULTIPLIER = 2.0f;
const float CAMERA_ANGULAR_SPEED = 6.0f;
const float FIELD_OF_VIEW = 70.0f;
const float NEAR_PLANE = 0.01f;
const float FAR_PLANE = 100.0f;
const float BICEP_LENGTH = 20.0f;
const float FOREARM_LENGTH = 10.0f;
const float ARM_ROTATION_SPEED = 0.7f;
const float FOREARM_ROTATION_SPEED = 1.0f;
const float BASE_ROTATION_SPEED = 1.0f;
const unsigned long CUBE_RESET_TIME_MS = 10000; // 10 seconds
const int MIN_CUBE_RADIUS = 16;
const int MAX_CUBE_RADIUS = 18;
const int MIN_CUBE_HEIGHT = 4;
const int MAX_CUBE_HEIGHT = 9;

// Declaring our functions and structs
GLuint loadTexture(const char *filename);

const char *getVertexShaderSource();

const char *getFragmentShaderSource();

const char *getTexturedVertexShaderSource();

const char *getTexturedFragmentShaderSource();

int compileAndLinkShaders(const char *vertexShaderSource, const char *fragmentShaderSource);

struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
        : position(_position), color(_color), uv(_uv) {}

    vec3 position;
    vec3 color;
    vec2 uv;
};

// Textured Cube model with position, color, and UV coordinates
const TexturedColoredVertex texturedCubeVertexArray[] = {                                       // position,                            color
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

// String version of vertex shader
const char *getVertexShaderSource()
{
    return "#version 330 core\n"
           "layout (location = 0) in vec3 aPos;"
           "layout (location = 1) in vec3 aColor;"
           ""
           "uniform mat4 worldMatrix;"
           "uniform mat4 viewMatrix = mat4(1.0);" // default value for view matrix (identity)
           "uniform mat4 projectionMatrix = mat4(1.0);"
           ""
           "out vec3 vertexColor;"
           "void main()"
           "{"
           "   vertexColor = aColor;"
           "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
           "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
           "}";
}

// String version of fragment shader
const char *getFragmentShaderSource()
{
    return "#version 330 core\n"
           "in vec3 vertexColor;"
           "out vec4 FragColor;"
           "void main()"
           "{"
           "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
           "}";
}
// string version of textured shader with UV coordinates
const char *getTexturedVertexShaderSource()
{
    return "#version 330 core\n"
           "layout (location = 0) in vec3 aPos;"
           "layout (location = 1) in vec3 aColor;"
           "layout (location = 2) in vec2 aUV;"
           ""
           "uniform mat4 worldMatrix;"
           "uniform mat4 viewMatrix = mat4(1.0);" // default value for view matrix (identity)
           "uniform mat4 projectionMatrix = mat4(1.0);"
           ""
           "out vec3 vertexColor;"
           "out vec2 vertexUV;"
           "void main()"
           "{"
           "   vertexColor = aColor;"
           "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
           "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
           "   vertexUV = aUV;"
           "}";
}

//  string version of textured vertex shader with UV coordinates
const char *getTexturedFragmentShaderSource()
{
    return "#version 330 core\n"
           "in vec3 vertexColor;"
           "in vec2 vertexUV;"
           "uniform sampler2D textureSampler;"
           ""
           "out vec4 FragColor;"
           "void main()"
           "{"
           "   vec4 textureColor = texture(textureSampler, vertexUV);"
           "   FragColor = textureColor; "
           "}";
}

// compile and linking shaders
int compileAndLinkShaders(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
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

    glVertexAttribPointer(0,                             // attribute 0 matches aPos in Vertex Shader
                          3,                             // size
                          GL_FLOAT,                      // type
                          GL_FALSE,                      // normalized?
                          sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
                          (void *)0                      // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void *)sizeof(vec3) // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, // attribute 2 matches aUV in Vertex Shader
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(TexturedColoredVertex),
                          (void *)(2 * sizeof(vec3)) // uv is offseted by 2 vec3 (comes after position and color)
    );
    glEnableVertexAttribArray(2);

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

// main function
int main(int argc, char *argv[])
{
    // Initialize GLFW and OpenGL version
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
    GLuint brickTextureID = loadTexture("Textures/brick.jpg");
    GLuint grassTextureID = loadTexture("Textures/image.png");
    GLuint woodTextureID = loadTexture("Textures/wood.png");
    GLuint cementTopTextureID = loadTexture("Textures/cementTop.png");
    GLuint cementBaseTextureID = loadTexture("Textures/cementBase.png");
    GLuint metalTextureID = loadTexture("Textures/metal.png");

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile and link shaders here ...
    int colorShaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
    int texturedShaderProgram = compileAndLinkShaders(getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());

    // We can set the shader once, since we have only one
    glUseProgram(colorShaderProgram);

    // Camera parameters for view transform
    vec3 cameraPosition(-35.0f, 20.0f, -35.0f);
    vec3 cameraLookAt(1.0f, -0.3f, 1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraSpeed = CAMERA_SPEED;
    float cameraFastSpeed = CAMERA_FAST_SPEED_MULTIPLIER * cameraSpeed;
    float cameraHorizontalAngle = 45.0f;
    float cameraVerticalAngle = -10.0f;
    bool cameraFirstPerson = true; // press 1 or 2 to toggle this variable

    // Spinning cube at camera position
    float spinningCubeAngle = 0.0f;

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(FIELD_OF_VIEW,                       // field of view in degrees
                                             (float)WINDOW_WIDTH / WINDOW_HEIGHT, // aspect ratio
                                             NEAR_PLANE, FAR_PLANE);              // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(colorShaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,                // eye
                             cameraPosition + cameraLookAt, // center
                             cameraUp);                     // up

    GLuint viewMatrixLocation = glGetUniformLocation(colorShaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    // Set View and Projection matrices on both shaders
    setViewMatrix(colorShaderProgram, viewMatrix);
    setViewMatrix(texturedShaderProgram, viewMatrix);

    setProjectionMatrix(colorShaderProgram, projectionMatrix);
    setProjectionMatrix(texturedShaderProgram, projectionMatrix);

    // Define and upload geometry to the GPU here ...
    int vao = createTexturedCubeVertexArrayObject();

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

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

        // keeping track of time for adjustment with time purposes
        now = std::chrono::system_clock::now();
        duration = now.time_since_epoch();
        milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        millis = (unsigned long)milliseconds;
        if (millis - baseTime > reset)
        {
            newCube = true;
            baseTime = millis;
            cout << "new cube pos!" << endl;
        }

        if ((millis - baseTime) % 1000 < 15)
        {
            cout << "time to cube move: " << 10 - ((millis - baseTime) / 1000) << endl;
        }

        // Enable our texture shader program, set the texture location, bind the brick texture
        glUseProgram(texturedShaderProgram);
        glActiveTexture(GL_TEXTURE0);
        GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
        glBindTexture(GL_TEXTURE_2D, grassTextureID);
        glUniform1i(textureLocation, 0); // Set our Texture sampler to user Texture Unit 0

        // Bind the appropriate VAO
        glBindVertexArray(vao);

        // Draw ground
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 0.02f, 1000.0f));
        setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

        // draw bicep base
        glBindTexture(GL_TEXTURE_2D, cementTopTextureID);
        mat4 baseMatrix = translate(mat4(1.0f), vec3(5.0f, 1.f, 5.0f)) * rotate(mat4(1.0f), radians(baseRotation + 180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(4.1f, 2.4f, 4.1f));
        setWorldMatrix(texturedShaderProgram, baseMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // draw larger fixed base
        glBindTexture(GL_TEXTURE_2D, cementBaseTextureID);
        mat4 bigBaseMatrix = translate(mat4(1.0f), vec3(5.0f, 1.f, 5.0f)) * scale(mat4(1.0f), vec3(6.1f, 1.6f, 6.1f));
        setWorldMatrix(texturedShaderProgram, bigBaseMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Create base rotation matrix for all arm components
        mat4 baseRotationMatrix = rotate(mat4(1.0f), radians(baseRotation), vec3(0.0f, 1.0f, 0.0f));

        // Draw bicep
        // translate in z sin(rotation)
        // translate in y cos(rotation)
        glBindTexture(GL_TEXTURE_2D, woodTextureID);
        vec3 bicepLocalPos = vec3(0.0f, bicepLength / 2 * cos(radians(bicepAngle)), bicepLength / 2 * sin(radians(bicepAngle)));
        vec3 bicepWorldPos = vec3(baseRotationMatrix * vec4(bicepLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
        mat4 bicepMatrix = translate(mat4(1.0f), bicepWorldPos) * baseRotationMatrix * rotate(mat4(1.0f), radians(bicepAngle), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, bicepLength, 1.0f));
        setWorldMatrix(texturedShaderProgram, bicepMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw the joint, angle following bicep angle
        float jointHeight = bicepLength * cos(radians(bicepAngle));
        float jointHorizontal = bicepLength * sin(radians(bicepAngle));
        vec3 jointLocalPos = vec3(0.0f, jointHeight, jointHorizontal);
        vec3 jointWorldPos = vec3(baseRotationMatrix * vec4(jointLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
        mat4 hingeMatrix = translate(mat4(1.0f), jointWorldPos) * baseRotationMatrix * rotate(mat4(1.0f), radians(forearmAngle + bicepAngle), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.1f, 2.1f, 2.1f));
        setWorldMatrix(texturedShaderProgram, hingeMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Calculate forearm angle and draw
        float totalAngle = forearmAngle + bicepAngle;
        vec3 forearmLocalPos = vec3(0.0f, forearmLength / 2 * cos(radians(totalAngle)) + jointHeight, jointHorizontal + forearmLength / 2 * sin(radians(totalAngle)));
        vec3 forearmWorldPos = vec3(baseRotationMatrix * vec4(forearmLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
        mat4 foreArmMatrix = translate(mat4(1.0f), forearmWorldPos) * baseRotationMatrix * rotate(mat4(1.0f), radians(totalAngle), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, forearmLength, 1.0f));
        setWorldMatrix(texturedShaderProgram, foreArmMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Calculate hammer position and draw
        glBindTexture(GL_TEXTURE_2D, metalTextureID);
        vec3 hammerLocalPos = vec3(0.0f, forearmLength * cos(radians(totalAngle)) + jointHeight, jointHorizontal + forearmLength * sin(radians(totalAngle)));
        vec3 hammerWorldPos = vec3(baseRotationMatrix * vec4(hammerLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
        mat4 hammerMatrix = translate(mat4(1.0f), hammerWorldPos) * baseRotationMatrix * rotate(mat4(1.0f), radians(totalAngle), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 8.0f));
        setWorldMatrix(texturedShaderProgram, hammerMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // calculate new cube position if necessary
        if (newCube == true)
        {
            // choose new cube position
            cubeRad = (rand() % (MAX_CUBE_RADIUS - MIN_CUBE_RADIUS + 1)) + MIN_CUBE_RADIUS;
            cubeRot = rand() % 360;
            cubeY = rand() % (MAX_CUBE_HEIGHT - MIN_CUBE_HEIGHT + 1) + MIN_CUBE_HEIGHT;
            newCube = false;
        }
        // draw cube
        vec3 cubePosition = vec3(cubeX + (cubeRad * sin(cubeRot)), cubeY, cubeRad * cos(cubeRot));
        mat4 cubeMatrix = translate(mat4(1.0f), cubePosition) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f));
        setWorldMatrix(texturedShaderProgram, cubeMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // calculate distance from hammer to cube
        float distanceX = abs(hammerWorldPos.x - cubePosition.x);
        float distanceY = abs(hammerWorldPos.y - cubePosition.y);
        float distanceZ = abs(hammerWorldPos.z - cubePosition.z);
        if (distanceX < (4.0f * sin(baseRotation) + 1) && distanceY < 4.0f && distanceZ < (4.0f * cos(baseRotation) + 1))
        {
            cout << "you got it!" << endl;
            newCube = true;
            baseTime = millis;
            points = points + 1;
            cout << "you now have " << points << " points!" << endl;
        }

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = true;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = false;
        }

        // This was solution for Lab02 - Moving camera exercise
        // We'll change this to be a first or third person camera
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

        // Update camera horizontal and vertical angle using the mouse position
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = -mousePosX + lastMousePosX;
        double dy = -mousePosY + lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // convert to spherical coordinates
        cameraHorizontalAngle -= dx * CAMERA_ANGULAR_SPEED * dt;
        cameraVerticalAngle -= dy * CAMERA_ANGULAR_SPEED * dt;

        // Clamp vertical angle to -85,85
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        if (cameraHorizontalAngle > 360)
        {
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360)
        {
            cameraHorizontalAngle += 360;
        }

        float theta = radians(-cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi) * cosf(theta), -sinf(phi), -cosf(phi) * sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);

        // Use camera lookat and side vectors to update positions with ASDW
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition += cameraSideVector * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
        {
            cameraPosition -= cameraLookAt * currentCameraSpeed * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
        {
            cameraPosition += cameraLookAt * currentCameraSpeed * dt;
        }

        // Arm Controls

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move robot bicep to the left
        {
            if (bicepAngle > -30.0)
                bicepAngle = bicepAngle - ARM_ROTATION_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move robot bicep to the right
        {
            if (bicepAngle < 30.0)
                bicepAngle = bicepAngle + ARM_ROTATION_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move forearm up/to the left
        {
            if (forearmAngle > -140)
                forearmAngle += -FOREARM_ROTATION_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move forearm down/to the right
        {
            if (forearmAngle < 140)
                forearmAngle += FOREARM_ROTATION_SPEED;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // rotate base left
        {
            baseRotation -= BASE_ROTATION_SPEED;
            if (baseRotation < -360.0f)
                baseRotation += 360.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // rotate base right
        {
            baseRotation += BASE_ROTATION_SPEED;
            if (baseRotation > 360.0f)
                baseRotation -= 360.0f;
        }

        // Set the view matrix for first person
        mat4 viewMatrix = mat4(1.0f);
        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

        setViewMatrix(colorShaderProgram, viewMatrix);
        setViewMatrix(texturedShaderProgram, viewMatrix);

        // left click action
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            const float projectileSpeed = 25.0f;
            cout << "left click !" << endl;
        }
        lastMouseLeftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    }
    cout << "your total points: " << points << "!!" << endl;

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}

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
