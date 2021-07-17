#include <iostream>
#include <cstdint>
#include <memory>

#include <opencv2/opencv.hpp>
#include "Message.h"

using namespace std;

extern Message msg_ctrl;


using cv::Point;
static Point pt1,pt2;
void mouseCallback(int mouseEvent,int x,int y,int flags,void* param)
{
    static bool is_selecting=false;
    switch(mouseEvent)
    {
        case cv::EVENT_LBUTTONDOWN:
            pt1 = Point(x,y);
            pt2 = Point(x,y);
            is_selecting = true;
            break;
        case cv::EVENT_LBUTTONUP:
            pt2 = Point(x,y);
            is_selecting = false;
            cout << x << ends << y << endl;
            break;
        case cv::EVENT_MOUSEMOVE:
            if(is_selecting)
                pt2 = Point(x,y);
            break;
        case cv::EVENT_MOUSEHWHEEL:
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
    viewLoop();
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