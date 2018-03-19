#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/glut.h>

class Camera {
private:
    glm::vec2 rotate;
public:

    Camera() :
        rotate(glm::vec2(10.0f, -45.0f))
    {}

    void addRotate(float dx, float dy) {
        rotate.x += dx;
        rotate.y += dy;
    }

    void setViewTransform(){
        glTranslatef(0, -0.3, 0);
        glRotatef(rotate.x, 1.0f, 0.0f, 0.0f);
        glRotatef(rotate.y, 0.0f, 1.0f, 0.0f);
    }
};