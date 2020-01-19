#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "camera.h"

struct ray
{
    glm::vec3 orig;
    glm::vec3 dir{0, 0, 1};
};


class Viewport
{
public:
    glm::mat4 calcProjMatrix() const;

    void setViewportSize(uint32_t iWidth, uint32_t iHeight);
    void setFOV(double inFOV);
    void setZNear(double inZNear);
    void setZFar(double inZFar);
    void setParallelProjection(bool use);

    double getZNear()   const;              // 0.01 по замовчуванню
    double getZFar()    const;              // 500 по замовчуванню
    double getFov()     const;              // 60 градусів по замовч
    double getWidth()   const;              // 1 по замовчуванню
    double getHeight()  const;              // 1 по замовчуванню

    bool isParallelProjection() const;  // false по замовчуванню

    ray calcCursorRay(double x, double y) const;

    double calcTargetPlaneWidth()   const;
    double calcTargetPlaneHeight()  const;
    double calcAspectRatio()        const;

    Camera& getCamera();
    const Camera& getCamera() const;

private:
    glm::vec3 unproject(double x, double y, double z) const;

    Camera camera;

    double zNear        {0.01};
    double zFar         {500};
    double fov          {60.0};
    double width        {1.0};
    double height       {1.0};
    bool   isParallProj {false};
};

#endif // VIEWPORT_H
