#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>
#include <viewport.h>

#define USE_MATH_CONSTANTS
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/vector_angle.hpp>

extern Viewport* viewport;


glm::mat4 Camera::calcViewMatrix() const
{
    return glm::lookAt(this->eye, this->target, this->up);
}

glm::vec3 Camera::calcForward() const
{
    return glm::normalize(this->target - this->eye);
}

glm::vec3 Camera::calcRight() const
{
    return glm::normalize(glm::cross(this->calcForward(), this->up));
}

double Camera::distanceFromEyeToTarget() const
{
    return sqrt(pow(static_cast<double>(this->target.x - this->eye.x), 2) +
                pow(static_cast<double>(this->target.y - this->eye.y), 2) +
                pow(static_cast<double>(this->target.z - this->eye.z), 2));
}

const glm::vec3& Camera::getEye()       const {return this->eye;}
const glm::vec3& Camera::getTarget()    const {return this->target;}

void Camera::setFrontView()
{
    // зберігаємо поточну відстань до цілі
    double dist = distanceFromEyeToTarget();

    this->setEyeTargetUp(target + glm::vec3{0, 0, 1},
                         target,
                         glm::vec3(0, 1, 0));

    this->setDistanceToTarget(dist);
}

void Camera::setTopView()
{
    std::cout << "top view" << std::endl;

    this->setFrontView();
    this->rotate(this->target,
                 {1, 0, 0},
                 -M_PI * 0.5);
}

void Camera::setRearView()
{
    std::cout << "rear view" << std::endl;

    this->setFrontView();
    this->rotate(this->target,
                 {0, 1, 0},
                 M_PI);
}

void Camera::setRightView()
{
    std::cout << "right view" << std::endl;

    this->setFrontView();
    this->rotate(this->target,
                 {0, 1, 0},
                 M_PI * 0.5);
}

void Camera::setLeftView()
{
    std::cout << "left view" << std::endl;

    this->setFrontView();
    this->rotate(this->target,
                 {0, 1, 0},
                 -M_PI * 0.5);
}

void Camera::setBottomView()
{
    std::cout << "bottom view" << std::endl;

    this->setFrontView();
    this->rotate(this->target,
                 {1, 0, 0},
                 M_PI * 0.5);
}

void Camera::setIsoView()
{
    std::cout << "iso view" << std::endl;

    this->setFrontView();
    this->rotate(this->target,
                 glm::vec3(1, 0, 0),
                 -M_PI / 4.0);
    this->rotate(this->target,
                 glm::vec3(0, 1, 0),
                 M_PI / 4.0);
    /*this->rotate(this->target,
                 glm::vec3(1, 0, 0),
                 -glm::radians(35.264));*/
}

void Camera::pan(double u, double v)
{
    glm::vec3 panOffset = this->calcRight() * static_cast<float>(u)
                          + this->up * static_cast<float>(v);

    this->eye    += panOffset;
    this->target += panOffset;
}

void Camera::orbit(glm::vec3 a, glm::vec3 b)
{
    glm::mat3 orbitMatrix;
    glm::mat3 toWorldSpace;

    toWorldSpace = glm::transpose(glm::mat3(this->calcViewMatrix()));

    double alpha = acos(static_cast<double>(glm::dot(a, b)));
    glm::vec3 axis = glm::cross(a, b);

    glm::vec3 newAxis = toWorldSpace * axis;


    orbitMatrix = glm::rotate(glm::mat4(1.0f),
                              static_cast<float>(alpha),
                              newAxis);

    glm::vec3 newEye = this->target
                       + orbitMatrix * (this->eye - this->target);

    glm::vec3 newUp  = this->up * orbitMatrix;

    this->eye   = newEye;
    this->up    = newUp;
}

void Camera::zoom(double factor)
{
    this->setDistanceToTarget(this->distanceFromEyeToTarget() + factor);
}

void Camera::zoomToFit(glm::vec3 min, glm::vec3 max)
{
    // переміщення камери навпроти фігури
    glm::vec3 center = min + ((max - min) / 2.0f);
    glm::vec3 offset = center - this->target;

    this->translate(offset);

    glm::vec3 extrs[2]{min, max};

    double alpha = glm::radians((viewport->getFov()
                                 * viewport->calcAspectRatio()) / 2.0);
    double betta, hamma, sideLength;
    double dist = 0.0;

    for (int i = 0; i < 2; i++)
    {
        betta = static_cast<double>(glm::angle(glm::normalize(extrs[i]
                                                              - this->target),
                                               glm::normalize(this->eye
                                                              - this->target)));
        hamma = glm::radians(180.0 - glm::degrees(alpha) - glm::degrees(betta));
        sideLength = static_cast<double>(glm::length(extrs[i] - this->target));

        if (i == 1)
            std::cout << "For max:\n";
        else std::cout << "For min:\n";

        std::cout << "alpha = " << glm::degrees(alpha)
                  << "\nbetta = " << glm::degrees(betta)
                  << "\nhamma = " << glm::degrees(hamma)
                  << "\ndist = " << dist << std::endl;

        dist = std::max((sideLength * sin(hamma)) / sin(alpha),
                        dist);
    }

    this->setDistanceToTarget(dist);
}

void Camera::translate(glm::vec3 delta)
{
    this->eye       +=  delta;
    this->target    +=  delta;
}

void Camera::setDistanceToTarget(double D)
{
    glm::vec3 f = glm::normalize(target - eye);
    f *= D;
    this->eye = this->target - f;
}

void Camera::transform(const glm::mat4 &trf)
{
    this->eye       = trf * glm::vec4(this->eye,     1.0);
    this->target    = trf * glm::vec4(this->target,  1.0);
    this->up        = trf * glm::vec4(this->up,      0.0);
}

void Camera::rotate(glm::vec3 point, glm::vec3 axis, double angle)
{
    // пересуваємо камеру в (0, 0, 0) - point
    glm::vec3 offset = glm::vec3(0.0, 0.0, 0.0) - point;
    this->translate(offset);

    // обчислюємо матрицю повороту і здійснюємо поворот
    glm::mat4 rotateMatrix = glm::rotate(glm::mat4(1.0f),
                                         static_cast<float>(angle),
                                         axis);
    this->transform(rotateMatrix);

    // повертаємо камеру у початкове місцезнаходження
    this->translate(-offset);
}

void Camera::setEyeTargetUp(glm::vec3 newEye, glm::vec3 newTarget,
                            glm::vec3 newUp)
{
    this->eye       = newEye;
    this->target    = newTarget;
    this->up        = newUp;


    glm::vec3 direction = glm::normalize(this->eye - this->target);

    this->up = glm::normalize(glm::cross(direction, this->calcRight()));
}
