#include <iostream>
#include <MouseLeaveEvent.h>

#include "mr.h"
#include "MFrame.h"
#include "event.h"
#include "MouseEvent.h"
#include "MouseHoverEvent.h"
#include "TextField.h"
#include "Button.h"
//1、可以考虑加入第三方库 供用户使用
//2、架构方面的问题
//1)进行事件分离
//2)渲染布局
//3)写一个自动析构指针的类
//4)既然是moe就要二次元的特点
//5)



int main() {
    MFrame frame = mr::render->Init("测试代码", "me.wangziyang.testwindow");
    frame.setTitle("二次元不会打游戏");

    frame.registerEvent<MouseEvent>([](MouseEvent& e) {
        std::cout << "Mouse: " << e.getX() << ", " << e.getY() << std::endl;
    });

    frame.registerEvent<MouseHoverEvent>([](MouseHoverEvent& e) {
        std::cout << "MouseHoverEvent: " << e.getX() << ", " << e.getY() << std::endl;
    });

    frame.registerEvent<MouseLeaveEvent>([](MouseLeaveEvent& e) {
        std::cout << "MouseLeaveEvent: " << e.getX() << ", " << e.getY() << std::endl;
    });

    MouseEvent event(20, 30);
    frame.callEvent(event);

    //按钮事件

    TextField textField("Test", 100, 20);
    textField.setPosition(20, 20);
    frame.addComponent(&textField);

    //目前测试了悬浮功能未测试点击
    Button btn("TButton", 100, 20);
    btn.setPosition(100, 50);
    frame.addComponent(&btn);


    //这里需要优化一下 但是这是标准的win32循环,之前崩溃原因是因为 队列堆积的消息拿不出来 所以直接会崩溃
    MSG msg;
    while (true) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg); //分配
        }
        // 这里可以放你的定时更新、动画、渲染之类的调用
    }
}
