#include "viewport.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


glm::mat4 Viewport::calcProjMatrix() const
{
    if (!this->isParallelProjection())
        return glm::perspective(glm::radians(this->fov),    // кут огляду
                                this->calcAspectRatio(),    // відношення сторін
                                this->zNear,                // ширина ближньої площини
                                this->zFar);                // ширина дальньої площини
    else {
        //const float screenWidth = 6.0f;
        const float screenWidth = static_cast<float>(this->calcTargetPlaneWidth());
        return glm::ortho(-screenWidth / 2.0f,
                          screenWidth / 2.0f,
                          -screenWidth / (2.0f * static_cast<float>
                                          (this->calcAspectRatio())),
                          screenWidth / (2.0f * static_cast<float>
                                         (this->calcAspectRatio())),
                          static_cast<float>(this->zNear),
                          static_cast<float>(this->zFar));
    }
}

void Viewport::setViewportSize(uint32_t iWidth, uint32_t iHeight)
{
    this->width = iWidth;
    this->height = iHeight;
}

void Viewport::setFOV(double inFOV)     {this->fov = inFOV;}

void Viewport::setZNear(double inZNear) {this->zNear = inZNear;}

void Viewport::setZFar(double inZFar)   {this->zFar = inZFar;}

void Viewport::setParallelProjection(bool use)  {this->isParallProj = use;}

double Viewport::getZNear() const   {return this->zNear;}

double Viewport::getZFar()  const   {return this->zFar;}

double Viewport::getFov()   const   {return this->fov;}

double Viewport::getWidth() const   {return this->width;}

double Viewport::getHeight()const   {return this->height;}

bool Viewport::isParallelProjection() const {return this->isParallProj;}

ray Viewport::calcCursorRay(double x, double y) const
{
    glm::vec3 a = this->unproject(x, y, -1.0);
    glm::vec3 b = this->unproject(x, y, 1.0);

    ray cursorRay;
    cursorRay.orig = a;
    cursorRay.dir  = glm::normalize(b - a);

    return cursorRay;
}

double Viewport::calcTargetPlaneWidth() const
{
    return this->calcTargetPlaneHeight() * this->calcAspectRatio();
}

double Viewport::calcTargetPlaneHeight() const
{
    return 2.0 * this->camera.distanceFromEyeToTarget() *
            tan(glm::radians(this->fov / 2.0));
}

double Viewport::calcAspectRatio() const
{
    return this->width / this->height;
}

Camera& Viewport::getCamera()   {return this->camera;}

const Camera& Viewport::getCamera() const {return this->camera;}

glm::vec3 Viewport::unproject(double x, double y, double z) const
{   
    glm::mat4 inverseMatrix = glm::inverse(this->calcProjMatrix() *
                                           this->getCamera().calcViewMatrix());

    glm::vec3 worldCoord = inverseMatrix * glm::vec4(x, y, z, 1.0f);

    return worldCoord;
}
