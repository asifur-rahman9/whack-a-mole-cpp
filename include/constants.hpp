#pragma once

// Window and camera constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float CAMERA_SPEED = 25.0f;
const float CAMERA_FAST_SPEED_MULTIPLIER = 2.0f;
const float CAMERA_ANGULAR_SPEED = 10.0f;
const float FIELD_OF_VIEW = 70.0f;
const float NEAR_PLANE = 0.01f;
const float FAR_PLANE = 300.0f;

// Mechanical arm constants
const float BICEP_LENGTH = 20.0f;
const float FOREARM_LENGTH = 10.0f;
const float ARM_ROTATION_SPEED = 0.7f;
const float FOREARM_ROTATION_SPEED = 1.0f;
const float BASE_ROTATION_SPEED = 1.0f;

// Game constants
const unsigned long CUBE_RESET_TIME_MS = 10000; // 10 seconds
const int MIN_CUBE_RADIUS = 16;
const int MAX_CUBE_RADIUS = 18;
const int MIN_CUBE_HEIGHT = 7;
const int MAX_CUBE_HEIGHT = 11;

const float PLAYER_HEIGHT = 4.0F;
