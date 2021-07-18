
#include "Message.h"
extern void viewInitial();

Message msg_ctrl;

Message::Message() {
    ptr=nullptr;
    passf=nullptr;
    registerFunc("announceView",
                 [this](){this->announceView();});
    registerFunc("needUpdate",
                 [this](){this->passImage();});
}

void Message::announceView() {
    viewInitial();
}

void Message::registerPassF(function<cv::Mat()> &&f) {
    passf=f;
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