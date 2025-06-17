#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

const int WIDTH = 800;
const int HEIGHT = 600;


const float PADDLE_WIDTH = 0.02f;
const float PADDLE_HEIGHT = 0.5f;
float leftPaddleY = 0.0f, rightPaddleY = 0.0f;
const float PADDLE_SPEED = 0.01f;


float ballX = 0.0f, ballY = 0.0f;
float ballDirX = 0.01f, ballDirY = 0.01f;
const float BALL_SIZE = 0.02f;

int leftScore = 0, rightScore = 0;


bool keyUpPressed = false, keyDownPressed = false;
bool keyWPressed = false, keySPressed = false;


void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
        bool pressed = (action == GLFW_PRESS);
        switch (key) {
            case GLFW_KEY_UP: keyUpPressed = pressed; break;
            case GLFW_KEY_DOWN: keyDownPressed = pressed; break;
            case GLFW_KEY_W: keyWPressed = pressed; break;
            case GLFW_KEY_S: keySPressed = pressed; break;
        }
    }
}

void updatePaddles() {
    if (keyWPressed && leftPaddleY + PADDLE_HEIGHT / 2 < 1.0f)
        leftPaddleY += PADDLE_SPEED;
    if (keySPressed && leftPaddleY - PADDLE_HEIGHT / 2 > -1.0f)
        leftPaddleY -= PADDLE_SPEED;
    
    if (keyUpPressed && rightPaddleY + PADDLE_HEIGHT / 2 < 1.0f)
        rightPaddleY += PADDLE_SPEED;
    if (keyDownPressed && rightPaddleY - PADDLE_HEIGHT / 2 > -1.0f)
        rightPaddleY -= PADDLE_SPEED;
}


void updateBall() {
    ballX += ballDirX;
    ballY += ballDirY;


    if (ballY + BALL_SIZE / 2 > 1.0f || ballY - BALL_SIZE / 2 < -1.0f)
        ballDirY = -ballDirY;

    if (ballX - BALL_SIZE / 2 < -0.98f && 
        ballY < leftPaddleY + PADDLE_HEIGHT / 2 &&
        ballY > leftPaddleY - PADDLE_HEIGHT / 2) {
        ballDirX = -ballDirX;
    }

    if (ballX + BALL_SIZE / 2 > 0.98f &&
        ballY < rightPaddleY + PADDLE_HEIGHT / 2 &&
        ballY > rightPaddleY - PADDLE_HEIGHT / 2) {
        ballDirX = -ballDirX;
    }

   
    if (ballX < -1.0f) {
        rightScore++;
        ballX = 0.0f;
        ballY = 0.0f;
    }
    if (ballX > 1.0f) { 
        leftScore++;
        ballX = 0.0f;
        ballY = 0.0f;
    }
}


void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawRect(-0.99f, leftPaddleY - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT);
   
    drawRect(0.97f, rightPaddleY - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT);
    drawRect(ballX - BALL_SIZE / 2, ballY - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE);
}

int main() {
    if (!glfwInit()) {
        printf("Failed to initialize glfw\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ping pong", NULL, NULL);
    if (!window) {
        printf("failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        updatePaddles();
        updateBall();
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

