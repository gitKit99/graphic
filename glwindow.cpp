#include "glwindow.h"

// попередні визначення функцій
void keyFun(GLFWwindow*, int, int, int, int);
void cursorPosFun(GLFWwindow * window, double xpos, double ypos);
void mouseFun(GLFWwindow * window, int button, int action, int mods);
void scrollFun(GLFWwindow * window, double xoffset, double yoffset);

GLWindow::GLWindow(const std::string & title, uint32_t width, uint32_t height)
    : width(width), height(height)
{
    glfwInit();

    handle = glfwCreateWindow(static_cast<int>(width),
                              static_cast<int>(height),
                              title.data(),
                              nullptr,
                              nullptr);
    glfwMakeContextCurrent(handle);

    static bool initGLAD = false;
    if (!initGLAD)
    {
        initGLAD = true;
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    glfwSetWindowUserPointer(handle, this);
}

uint32_t GLWindow::getWidth()   const { return this->width; }

uint32_t GLWindow::getHeight()  const { return this->height; }

GLFWwindow* GLWindow::getGLFWHandle() const { return this->handle; }

void GLWindow::setKeyCallback(const KeyCallback &callback)
{
    key_callback = callback;
    glfwSetKeyCallback(handle, keyFun);
}

void GLWindow::setCursorPosCallback(const CursorPosCallback & callback)
{
    this->cursor_pos_callback = callback;
    glfwSetCursorPosCallback(handle, cursorPosFun);
}

void GLWindow::setMouseCallback(const MouseCallback & callback)
{
    this->mouse_callback = callback;
    glfwSetMouseButtonCallback(handle, mouseFun);
}

void GLWindow::setScrollCallback(const ScrollCallback & callback)
{
    this->scroll_callback = callback;
    glfwSetScrollCallback(handle, scrollFun);
}

void keyFun(GLFWwindow *window, int key, int scancode, int action, int modifier)
{
    void *data = glfwGetWindowUserPointer(window);
    GLWindow *w = static_cast<GLWindow *>(data);

    w->key_callback(static_cast<KeyCode>(key),
                    static_cast<Action>(action),
                    static_cast<Modifier>(modifier));
}

void cursorPosFun(GLFWwindow * window, double xpos, double ypos)
{
    void *data = glfwGetWindowUserPointer(window);
    GLWindow *w = static_cast<GLWindow *>(data);

    w->cursor_pos_callback(xpos, ypos);
}

void mouseFun(GLFWwindow * window, int button, int action, int mods)
{
    void *data = glfwGetWindowUserPointer(window);
    GLWindow *w = static_cast<GLWindow *>(data);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    w->mouse_callback(static_cast<ButtonCode>(button),
                      static_cast<Action>(action),
                      static_cast<Modifier>(mods),
                      xpos,
                      ypos);
}

void scrollFun(GLFWwindow * window, double xoffset, double yoffset)
{
    void *data = glfwGetWindowUserPointer(window);
    GLWindow *w = static_cast<GLWindow *>(data);

    w->scroll_callback(xoffset, yoffset);
}
