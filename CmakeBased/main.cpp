#include <iostream>

#include "viewmodel.h"
#include "Message.h"
extern Message msg_ctrl;

int main(int argc, const char** argv)
{
    float angle = 140.0;
    std::string filename = "output.png";
    Eigen::Vector3f eye_pos = { 0,0,10 };

    ViewModel vm=ViewModel(700,700,eye_pos,filename,angle,45.0, 1, 0.1, 50);
    msg_ctrl.registerFunc("prepareVM",[&vm](){vm.vmInit();});

    msg_ctrl.callFunc("prepareVM");
    msg_ctrl.callFunc("announceView");//start loop

    return 0;
}