//
// Created by user on 2021/7/15.
//

#include "Message.h"
#include "view.cpp"

Message msg_ctrl;

void viewReg(){
    msg_ctrl.registerFunc("waitKey",waitKey);
    cv::setMouseCallback("image",mouseCallback);
}

void Message::announceView() {
    viewReg();
}

void Message::mainLoop() {
    while (true) {
        callFunc("waitKey");
        callFunc("imshow");
    }
}

void Message::registerFunc(const string &s,ftype f) {
    if (func_mp.find(s) != func_mp.end()) {
        throw runtime_error("func "+s+" already exists\n");
    }

    func_mp[s]=func_v.size();
    func_v.push_back(f);
}

void Message::callFunc(const string &s) {
    if (func_mp.find(s) == func_mp.end()) {
        throw runtime_error("func "+s+" called but not found\n");
    }

    index &i=func_mp[s];
    func_v[i]();
}