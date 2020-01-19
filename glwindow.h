#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "glad.h"

#include <GLFW/glfw3.h>

#include <string>
#include <functional>

enum class Modifier
{
    NoModifier = 0,
    Shift = 1,
    Control = 2,
    Alt = 4,
    Super = 8,
};

enum class Action
{
    Release = 0,
    Press = 1,
    Repeat = 2,
};

enum class ButtonCode
{
    Button_1 = 0,
    Button_2 = 1,
    Button_3 = 2,
    Button_4 = 3,
    Button_5 = 4,
    Button_6 = 5,
    Button_7 = 6,
    Button_8 = 7,
    Last = Button_8,
    Left = Button_1,
    Right = Button_2,
    Middle = Button_3
};

enum class KeyCode
{
    UNKNOWN = -1,
    Space = 32,
    Apostrophe = 39,     /* ' */
    Comma = 44,    /* , */
    Minus = 45,    /* - */
    Period = 46,   /* . */
    Slash = 47,    /* / */
    Key_0 = 48,
    Key_1 = 49,
    Key_2 = 50,
    Key_3 = 51,
    Key_4 = 52,
    Key_5 = 53,
    Key_6 = 54,
    Key_7 = 55,
    Key_8 = 56,
    Key_9 = 57,
    Semicolon = 58,  /* ; */
    Equal = 61,      /* = */
    Key_A = 65,
    Key_B = 66,
    Key_C = 67,
    Key_D = 68,
    Key_E = 69,
    Key_F = 70,
    Key_G = 71,
    Key_H = 72,
    Key_I = 73,
    Key_J = 74,
    Key_K = 75,
    Key_L = 76,
    Key_M = 77,
    Key_N = 78,
    Key_O = 79,
    Key_P = 80,
    Key_Q = 81,
    Key_R = 82,
    Key_S = 83,
    Key_T = 84,
    Key_U = 85,
    Key_V = 86,
    Key_W = 87,
    Key_X = 88,
    Key_Y = 89,
    Key_Z = 90,
    Left_bracket = 91,  /* [ */
    Backslash = 92,     /* \ */
    Right_bracket = 93, /* ] */
    Grave_accent = 96,   /* ` */
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    Page_up = 266,
    Page_down = 267,
    Home = 268,
    End = 269,
    Caps_lock = 280,
    Scroll_lock = 281,
    Num_lock = 282,
    Print_screen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    KP_0 = 320,
    KP_1 = 321,
    KP_2 = 322,
    KP_3 = 323,
    KP_4 = 324,
    KP_5 = 325,
    KP_6 = 326,
    KP_7 = 327,
    KP_8 = 328,
    KP_9 = 329,
    KP_decimal = 330,
    KP_divide = 331,
    KP_multiply = 332,
    KP_subtract = 333,
    KP_add = 334,
    KP_enter = 335,
    KP_equal = 336,
    Left_shift = 340,
    Left_control = 341,
    Left_alt = 342,
    Left_super = 343,
    Right_shift = 344,
    Right_control = 345,
    Right_alt = 346,
    Right_super = 347,
    Menu = 348,

};

class GLWindow
{
public:
    using KeyCallback       = std::function<void(KeyCode, Action, Modifier)>;
    using CursorPosCallback = std::function<void(double, double)>;
    using MouseCallback     = std::function<void(ButtonCode, Action, Modifier,
                                double, double)>;
    using ScrollCallback    = std::function<void(double, double)>;

    GLWindow(const std::string& title, uint32_t width, uint32_t height);

    uint32_t getWidth()         const;
    uint32_t getHeight()        const;
    GLFWwindow*  getGLFWHandle()const;

    void setKeyCallback(const KeyCallback& callback);
    void setCursorPosCallback(const CursorPosCallback& callback);
    void setMouseCallback(const MouseCallback& callback);
    void setScrollCallback(const ScrollCallback& callback);

private:
    uint32_t width;
    uint32_t height;

    GLFWwindow* handle;

    KeyCallback			key_callback;
    CursorPosCallback	cursor_pos_callback;
    MouseCallback		mouse_callback;
    ScrollCallback		scroll_callback;

    friend void keyFun(GLFWwindow* window, int key, int scancode, int action,
        int modifier);
    friend void cursorPosFun(GLFWwindow* window, double xpos, double ypos);
    friend void mouseFun(GLFWwindow* window, int button, int action, int mods);
    friend void scrollFun(GLFWwindow* window, double xoffset, double yoffset);
};

#endif // GLWINDOW_H
