#include <iostream>
#include <cstdint>
#include <memory>

#include <opencv2/opencv.hpp>
#include "Message.h"

using namespace std;

void waitKey();
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
        case cv::EVENT_MOUSEMOVE:
            if(is_selecting)
                pt2 = Point(x,y);
            break;
        case cv::EVENT_LBUTTONUP:
            pt2 = Point(x,y);
            is_selecting = false;
            cout << x << ends << y << endl;
            break;
    }
    return;
}


void waitKey() {
    int key=cv::waitKey(10);
    if (key == -1) return;
    if (key == 27) exit(0);

    cout << "received " << static_cast<char>(key) << endl;
    string msg;
    switch (key) {
        case 'a':
            msg="angleMinus";break;
        case 'd':
            msg="anglePlus";break;
        case 'r':
            msg="redraw";break;
        default:
            msg="skip";break;
    }
    if (msg != "skip"){
        msg_ctrl.callFunc(msg);
    }
}
