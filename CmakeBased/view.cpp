#include <iostream>
#include <memory>

#include <opencv2/opencv.hpp>
#include "Message.h"
extern Message msg_ctrl;

using namespace std;

using cv::Point;
void mouseCallback(int mouseEvent,int x,int y,int flags,void* param)
{
    static bool is_selecting=false;
    static Point pt;
    switch(mouseEvent)
    {
        case cv::EVENT_LBUTTONDOWN:
            pt = Point(x,y);
            is_selecting = true;
            break;
        case cv::EVENT_LBUTTONUP:
            pt = Point(x,y);
            is_selecting = false;
            break;
        case cv::EVENT_MOUSEMOVE:
            if(is_selecting){
                if (pt.x < x)
                    msg_ctrl.callFunc("anglePlus");
                if (pt.x > x)
                    msg_ctrl.callFunc("angleMinus");
                msg_ctrl.callFunc("redraw");
                pt = Point(x,y);
            }
            break;
        case cv::EVENT_MOUSEHWHEEL://没用
            int v=cv::getMouseWheelDelta(flags);
            if (v > 0)
                msg_ctrl.callFunc("eyeZPlus");
            else
                msg_ctrl.callFunc("eyeZMinus");
            msg_ctrl.callFunc("redraw");
    }
}

void viewLoop();
void viewInitial() {
    cv::namedWindow("image");
    cv::setMouseCallback("image",mouseCallback);
    msg_ctrl.registerFunc("startLoop",
                          [](){viewLoop();}
    );
}

int waitKey() {
    int key=cv::waitKey(10);
    //提前结束
    if (key == -1 || key == 27) return key;

    cout << "received " << static_cast<char>(key) << endl;
    string msg;
    switch (key) {
        case 'q':
            msg="angleMinus";break;
        case 'e':
            msg="anglePlus";break;
        case 'a':
            msg="eyeLeft";break;
        case 'd':
            msg="eyeRight";break;
        case 'w':
            msg="eyeUp";break;
        case 's':
            msg="eyeDown";break;
        case 'r':
            msg="redraw";break;
        default:
            msg="skip";break;
    }
    if (msg != "skip"){
        msg_ctrl.callFunc(msg);
    }
    msg_ctrl.callFunc("redraw");
    return key;
}

void imshow() {
    msg_ctrl.callFunc("needUpdate");
    auto ptr=msg_ctrl.ptr;
    cv::imshow ("image",*ptr);
}

void viewLoop() {
    while (waitKey() != 27) {
        imshow();
    }
    cv::destroyAllWindows();
}