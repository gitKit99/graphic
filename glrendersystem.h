#ifndef GLRENDERSYSTEM_H
#define GLRENDERSYSTEM_H

#include <glm/glm.hpp>
#include "glad.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <map>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

class GLRenderSystem
{
public:
    void init();

    void clearDisplay(float r, float g, float b);
    void setViewport(double x, double y, double width, double height);
    void renderTriangleSoup(const std::vector<Vertex>& vertices);

    void setupLight(uint32_t index, glm::vec3 position, glm::vec3 Ia,
        glm::vec3 Id, glm::vec3 Is);
    void turnLight(uint32_t index, bool enable);

    void setWorldMatrix(const glm::mat4& matrix);
    const glm::mat4& getWorldMatrix() const;

    void setViewMatrix(const glm::mat4& matrix);
    const glm::mat4& getViewMatrix() const;

    void setProjMatrix(const glm::mat4& matrix);
    const glm::mat4& getProjMatrix() const;

    const std::vector<Vertex>& getRenderMesh();

private:
    // матриці трансформацій
    glm::mat4 worldMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;

    //glm::vec3 viewPos;

    // світло
    uint32_t lightIndex;    // індекс джерела світла
    glm::vec3 lightPos;     // розташування джерела світла
    glm::vec3 iAmbient;     // колір середовища
    glm::vec3 iDiffuse;     // колір розсіяного світла
    glm::vec3 iSpecular;    // колір дзеркального відбиття
    bool isLightEnable;     // стан джерела світла (увімк / вимк)

    std::vector<Vertex> renderMesh;
};

#endif // GLRENDERSYSTEM_H
