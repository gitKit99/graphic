#include <iostream>
#include <limits>
#include <iterator>

#include <glm/gtc/matrix_transform.hpp>

#include "glwindow.h"
#include "glrendersystem.h"
#include "viewport.h"

#define INF std::numeric_limits<float>::infinity()

static GLRenderSystem* rs;
Viewport* viewport;

static glm::vec3 cubeMin{INF, INF, INF}, cubeMax{-INF, -INF, -INF};

// функція промальовування сцени у вікні (наразі малювання куба)
void renderScene(GLRenderSystem &rs)
{
    static bool setEncCube = false;
    const float L = 1.0f; // довжина ребра куба
    const glm::vec3 defNormal{0.0f, 0.0f, 0.0f};

    // вершини куба
    const Vertex* CUBE_VERTICES = new Vertex[8]
    {
        // нижній квадрат
        Vertex{glm::vec3(-L/2, -L/2, L/2), defNormal},
        Vertex{glm::vec3(L/2, -L/2, L/2), defNormal},
        Vertex{glm::vec3(L/2, -L/2, -L/2), defNormal},
        Vertex{glm::vec3(-L/2, -L/2, -L/2), defNormal},

        // верхній квадрат
        Vertex{glm::vec3(-L/2, L/2, L/2), defNormal},
        Vertex{glm::vec3(L/2, L/2, L/2), defNormal},
        Vertex{glm::vec3(L/2, L/2, -L/2), defNormal},
        Vertex{glm::vec3(-L/2, L/2, -L/2), defNormal}
    };

    enum class FACETS
    {
        FRONT,
        RIGHT,
        BACK,
        LEFT,
        TOP,
        BOTTOM
    };

    const glm::vec3* NORMALS = new glm::vec3[6]      // нормалі для граней
    {
        glm::vec3(0.0f, 0.0f, 1.0f),    // передньої
        glm::vec3(1.0f, 0.0f, 0.0f),    // правої
        glm::vec3(0.0f, 0.0f, -1.0f),   // задньої
        glm::vec3(-1.0f, 0.0f, 0.0f),   // лівої
        glm::vec3(0.0f, 1.0f, 0.0f),    // верхньої
        glm::vec3(0.0f, -1.0f, 0.0f)    // нижньої
    };

    if (!setEncCube)
    {
        for (int vertex = 0; vertex < 8; vertex++)
        {
            cubeMax = glm::vec3{std::max(CUBE_VERTICES[vertex].position.x, cubeMax.x),
                                std::max(CUBE_VERTICES[vertex].position.y, cubeMax.y),
                                std::max(CUBE_VERTICES[vertex].position.z, cubeMax.z)};

            cubeMin = glm::vec3{std::min(CUBE_VERTICES[vertex].position.x, cubeMin.x),
                                std::min(CUBE_VERTICES[vertex].position.y, cubeMin.y),
                                std::min(CUBE_VERTICES[vertex].position.z, cubeMin.z)};
        }

        setEncCube = true;

        std::cout << "Min(" << cubeMin.x << ", " << cubeMin.y << ", " << cubeMin.z << ")\n";
        std::cout << "Max(" << cubeMax.x << ", " << cubeMax.y << ", " << cubeMax.z << ")\n";
    }

    std::vector<Vertex> square; // поточна грань куба
    std::vector<Vertex> cubeMesh; // сітка трикутників куба
    for (int facet = 0; facet < 6; facet++)
    {
        square.clear();

        switch (static_cast<FACETS>(facet))
        {
        case FACETS::FRONT:
            square = {
              CUBE_VERTICES[0],
              CUBE_VERTICES[1],
              CUBE_VERTICES[5],
              CUBE_VERTICES[4]
            };
            break;
        case FACETS::RIGHT:
            square = {
              CUBE_VERTICES[1],
              CUBE_VERTICES[2],
              CUBE_VERTICES[6],
              CUBE_VERTICES[5]
            };
            break;
        case FACETS::BACK:
            square = {
              CUBE_VERTICES[2],
              CUBE_VERTICES[3],
              CUBE_VERTICES[7],
              CUBE_VERTICES[6]
            };
            break;
        case FACETS::LEFT:
            square = {
              CUBE_VERTICES[3],
              CUBE_VERTICES[0],
              CUBE_VERTICES[4],
              CUBE_VERTICES[7]
            };
            break;
        case FACETS::TOP:
            square = {
              CUBE_VERTICES[4],
              CUBE_VERTICES[5],
              CUBE_VERTICES[6],
              CUBE_VERTICES[7]
            };
            break;
        case FACETS::BOTTOM:
            square = {
              CUBE_VERTICES[3],
              CUBE_VERTICES[2],
              CUBE_VERTICES[1],
              CUBE_VERTICES[0]
            };
            break;
        }

        // додавання тріангульованої поточної грані
        cubeMesh.push_back(Vertex{square[0].position, NORMALS[facet]});
        cubeMesh.push_back(Vertex{square[2].position, NORMALS[facet]});
        cubeMesh.push_back(Vertex{square[3].position, NORMALS[facet]});

        cubeMesh.push_back(Vertex{square[0].position, NORMALS[facet]});
        cubeMesh.push_back(Vertex{square[1].position, NORMALS[facet]});
        cubeMesh.push_back(Vertex{square[2].position, NORMALS[facet]});
    }

    rs.renderTriangleSoup(cubeMesh);


}

void moveCube(GLRenderSystem& rs, glm::vec3 offset)
{
    offset /= 10; // швидкість руху

    rs.setWorldMatrix(glm::translate(rs.getWorldMatrix(), offset));
}

void moveCamera(Camera& camera, glm::vec3 offset)
{
    offset /= 10;
    camera.translate(offset);
}

void onKeyCallback(KeyCode key, Action action, Modifier mods)
{
    if (key == KeyCode::Up)
        moveCube(*rs, glm::vec3(0.0f, 1.0f, 0.0f));

    if (key == KeyCode::Down)
        moveCube(*rs, glm::vec3(0.0f, -1.0f, 0.0f));

    if (key == KeyCode::Left)
        moveCube(*rs, glm::vec3(-1.0f, 0.0f, 0.0f));

    if (key == KeyCode::Right)
        moveCube(*rs, glm::vec3(1.0f, 0.0f, 0.0f));

    if (key == KeyCode::Key_W)
        moveCamera(viewport->getCamera(), viewport->getCamera().calcForward());

    if (key == KeyCode::Key_A)
        moveCamera(viewport->getCamera(), -viewport->getCamera().calcRight());

    if (key == KeyCode::Key_S)
        moveCamera(viewport->getCamera(), -viewport->getCamera().calcForward());

    if (key == KeyCode::Key_D)
        moveCamera(viewport->getCamera(), viewport->getCamera().calcRight());

    // F1 ... F7
    if (static_cast<int>(key) >= 290 && static_cast<int>(key) <= 298 &&
            action == Action::Press)
    {
       switch (key)
       {
       case KeyCode::F1:
           viewport->getCamera().setFrontView();
           break;
       case KeyCode::F2:
           viewport->getCamera().setTopView();
           break;
       case KeyCode::F3:
           viewport->getCamera().setRearView();
           break;
       case KeyCode::F4:
           viewport->getCamera().setRightView();
           break;
       case KeyCode::F5:
           viewport->getCamera().setLeftView();
           break;
       case KeyCode::F6:
           viewport->getCamera().setBottomView();
           break;
       case KeyCode::F7:
           viewport->getCamera().setIsoView();
           break;
       case KeyCode::F8:
           if (!viewport->isParallelProjection())
               viewport->setParallelProjection(true);
           else viewport->setParallelProjection(false);
           break;
       case KeyCode::F9:
           std::cout << "Zoom to fit\n";
           viewport->getCamera().zoomToFit(cubeMin, cubeMax);
           break;
       default:
           break;
       }
    }
}

void scrollCallback(double xoffset, double yoffset)
{
    viewport->getCamera().zoom(yoffset / 10);
}

static int mouseButton = 0;
static double u0, v0;
static glm::vec3 a, b;

double map(double var, double begin, double end,
           double targetBegin, double targetEnd)
{
    double result = targetBegin + ((var - begin) * (targetEnd - targetBegin))
                    / (end - begin);
    return result;
}

void onMouseButtonCallback(ButtonCode button, Action action, Modifier mods,
                           double xpos, double ypos)
{
    // пересування камери (pan)
    if (button == ButtonCode::Left)
    {
        if (action == Action::Press)
        {
            u0 = map(xpos,
                     0.0,
                     viewport->getWidth(),
                     -viewport->calcTargetPlaneWidth(),
                     viewport->calcTargetPlaneWidth());

            v0 = map(ypos,
                     viewport->getHeight(),
                     0.0,
                     -viewport->calcTargetPlaneHeight(),
                     viewport->calcTargetPlaneHeight());

            mouseButton = 1;
        }
        else if (action == Action::Release)
        {
            mouseButton = 0;
        }
    }

    // обертання камери (orbit)
    else if (button == ButtonCode::Right)
    {
        if (action == Action::Press)
        {
            double xSphereVector = map(xpos,
                           0.0,
                           viewport->getWidth(),
                           -viewport->calcAspectRatio(),
                           viewport->calcAspectRatio());

            double ySphereVector = map(ypos,
                           viewport->getHeight(),
                           0.0,
                           -1.0,
                           1.0);

            glm::vec3 pointOnSphere{xSphereVector, ySphereVector, 0};

            if (glm::length(pointOnSphere) >= 1.0f)
            {
                pointOnSphere = glm::normalize(pointOnSphere);
            }
            else {
                pointOnSphere.z = static_cast<float>(
                            sqrt(1.0
                                 - pow(static_cast<double>(pointOnSphere.x), 2)
                                 - pow(static_cast<double>(pointOnSphere.y), 2)));
            }

            a = pointOnSphere;

            mouseButton = 2;
        }
        else if (action == Action::Release)
        {
            mouseButton = 0;
        }
    }
}

void cursorPosCallback(double xpos, double ypos)
{
    if (mouseButton == 1)
    {
        double u = map(xpos,
                       0.0,
                       viewport->getWidth(),
                       -viewport->calcTargetPlaneWidth(),
                       viewport->calcTargetPlaneWidth());

        double v = map(ypos,
                       viewport->getHeight(),
                       0.0,
                       -viewport->calcTargetPlaneHeight(),
                       viewport->calcTargetPlaneHeight());

        double du = u0 - u;
        double dv = v0 - v;

        viewport->getCamera().pan(du, dv);

        u0 = u;
        v0 = v;
    }
    else if (mouseButton == 2)
    {
        double xSphereVector = map(xpos,
                       0.0,
                       viewport->getWidth(),
                       -viewport->calcAspectRatio(),
                       viewport->calcAspectRatio());

        double ySphereVector = map(ypos,
                       viewport->getHeight(),
                       0.0,
                       -1.0,
                       1.0);

        glm::vec3 pointOnSphere{xSphereVector, ySphereVector, 0};

        if (glm::length(pointOnSphere) >= 1.0f)
        {
            pointOnSphere = glm::normalize(pointOnSphere);
        }
        else {
            pointOnSphere.z = static_cast<float>(
                        sqrt(1.0
                             - pow(static_cast<double>(pointOnSphere.x), 2)
                             - pow(static_cast<double>(pointOnSphere.y), 2)));
        }
        b = pointOnSphere;

        viewport->getCamera().orbit(b, a);


        a = b;
    }
}


int main()
{
    rs          = new GLRenderSystem();
    viewport    = new Viewport();

    GLWindow window("myWindow", 640, 480);

    viewport->setViewportSize(640, 480);
    viewport->setFOV(60.0);
    viewport->setZNear(0.01);
    viewport->setZFar(500);

    window.setKeyCallback(onKeyCallback);
    window.setScrollCallback(scrollCallback);
    window.setCursorPosCallback(cursorPosCallback);
    window.setMouseCallback(onMouseButtonCallback);

    rs->init();
    rs->setupLight(0,   // індекс світла
                   glm::vec3(0.0f, 5.0f, 0.0f), // розташування джерела
                   glm::vec3(1.0f, 0.0f, 0.0f), // фонове освітлення
                   glm::vec3(0.0f, 1.0f, 0.0f), // розсіяне світло
                   glm::vec3(0.0f, 0.0f, 1.0f));// дзеркально відбите світло
    rs->turnLight(0, true);

    //viewport->setParallelProjection(true);

    while (!glfwWindowShouldClose(window.getGLFWHandle()))
    {
        rs->setViewport(0, 0, window.getWidth(), window.getHeight());
        rs->clearDisplay(0.5f, 1.0f, 1.0f);

        rs->setViewMatrix(viewport->getCamera().calcViewMatrix());
        rs->setProjMatrix(viewport->calcProjMatrix());

        renderScene(*rs);
        glfwSwapBuffers(window.getGLFWHandle());
        glfwWaitEvents();
    }

    glfwTerminate();
}
