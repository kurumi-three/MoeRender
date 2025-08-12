// #ifndef BUTTONCOMPONENT_H
// #define BUTTONCOMPONENT_H
//
// #include "Component.h"
// #include <functional>
// #include <string>
// #include "MouseEvent.h"
//
// class Button : public Component
// {
// private:
//     std::string text;
//     int width;
//     int height;
//     int x;
//     int y;
//     std::function<void(const MouseEvent&)> onClick;
//
// public:
//     Button(std::string text, int width, int height)
//     {
//         this->text = text;
//         this->width = width;
//         this->height = height;
//         this->x = 0;
//         this->y = 0;
//     }
//
//     // 设置位置（与TextField相同）
//     void setPosition(int x, int y) {
//         this->x = x;
//         this->y = y;
//     }
//
//     // 设置尺寸（与TextField相同）
//     void setSize(int width, int height) {
//         this->width = width;
//         this->height = height;
//     }
//
//     // 文本访问（与TextField相同）
//     std::string getText() { return this->text; }
//     void setText(std::string newText) { text = newText; }
//
//     // 尺寸访问（与TextField相同）
//     int getWidth() { return this->width; }
//     int getHeight() { return this->height; }
//
//     // 位置访问（与TextField相同）
//     int getX() { return this->x; }
//     int getY() { return this->y; }
// };
//
// #endif // BUTTONCOMPONENT_H

#ifndef BUTTONCOMPONENT_H
#define BUTTONCOMPONENT_H

#include "Component.h"
#include <functional>
#include <string>
#include "MouseEvent.h"
#include <Windows.h>   // for HWND, WM_COMMAND, BN_CLICKED

class Button : public Component
{
public:
    using ClickCallback = std::function<void(const MouseEvent&)>;

    Button(const std::string& text, int width, int height)
      : text(text), width(width), height(height),
        x(0), y(0),
        _hwndButton(nullptr), _ctrlId(0)
    {}

    // —— 基本属性接口 ——
    void setPosition(int x_, int y_)   { x = x_; y = y_; }
    void setSize(int w, int h)         { width = w; height = h; }
    const std::string& getText() const { return text; }
    void setText(const std::string& t) { text = t; }
    int getX()     const { return x; }
    int getY()     const { return y; }
    int getWidth() const { return width; }
    int getHeight()const { return height; }

    // —— 原生句柄 & 控件 ID ——
    void setNativeHandle(HWND h) { _hwndButton = h; }
    HWND getNativeHandle() const { return _hwndButton; }

    void setCtrlId(int id) { _ctrlId = id; }
    int  getCtrlId() const { return _ctrlId; }

    // —— 点击回调注册 ——
    void setOnClick(ClickCallback cb) { onClick = std::move(cb); }

    // —— 重写基类接口，分发 WM_COMMAND 消息 ——
    void handleNativeMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        if (msg == WM_COMMAND)
        {
            int code = HIWORD(wParam);
            HWND src = reinterpret_cast<HWND>(lParam);

            // BN_CLICKED 且是本按钮发来的
            if (code == BN_CLICKED &&
               (src == _hwndButton || LOWORD(wParam) == _ctrlId))
            {
                if (onClick)
                {
                    // 这里可以自己计算相对坐标传入 MouseEvent
                    MouseEvent ev(0, 0);
                    onClick(ev);
                }
            }
        }
    }

private:
    std::string text;
    int width, height;
    int x, y;

    HWND _hwndButton;        // Win32 原生按钮句柄
    int  _ctrlId;            // CreateWindowEx 时传入的 HMENU ID

    ClickCallback onClick;   // 用户注册的点击回调
};

#endif // BUTTONCOMPONENT_H
