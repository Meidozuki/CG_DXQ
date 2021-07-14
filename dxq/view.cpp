#pragma once
#include <iostream>
#include <cstdint>
#include <memory>

#include <opencv2/opencv.hpp>
#include "viewmodel.hpp"
extern ViewModel vm;

using namespace std;

shared_ptr<cv::Mat> glop;

using cv::Point;
Point pt1,pt2;
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




void processNormalKeys(unsigned char key,int x,int y) {
    if (key == 27) exit(0);
    vm.processViewMessage(string(1,key));
}


void read_bmp() {
    std::string filename("D:/model.bmp");
    cv::Mat img=cv::imread(filename);

    if (img.empty()) {
        throw std::runtime_error("file not opened");
    }

    shared_ptr<cv::Mat> sp(new cv::Mat(img));
    glop=sp;
    std::cout << "ptr_cnt in read_fn: " << sp.use_count() << std::endl;
}
