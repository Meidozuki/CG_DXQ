
#include "viewmodel.h"
#include "Message.h"
#include <ctime>

extern Message msg_ctrl;

void ViewModel::vmInit() {
    redraw();

    msg_ctrl.registerPassF([this](){return this->getImage();});
    msg_ctrl.registerFunc("anglePlus",
                          [this](){this->modifyAngle(1);}
                          );
    msg_ctrl.registerFunc("angleMinus",
                          [this](){this->modifyAngle(-1);}
    );
    msg_ctrl.registerFunc("eyeLeft",
                          [this](){this->modifyEyepos(0.1,0);}
    );
    msg_ctrl.registerFunc("eyeRight",
                          [this](){this->modifyEyepos(-0.1,0);}
    );
    msg_ctrl.registerFunc("eyeUp",
                          [this](){this->modifyEyepos(0.1,1);}
    );
    msg_ctrl.registerFunc("eyeDown",
                          [this](){this->modifyEyepos(-0.1,1);}
    );
    msg_ctrl.registerFunc("eyeZPlus",
                          [this](){this->modifyEyepos(0.2,2);}
    );
    msg_ctrl.registerFunc("eyeZMinus",
                          [this](){this->modifyEyepos(-0.2,2);}
    );
    msg_ctrl.registerFunc("redraw",
                          [this](){this->redraw();}
    );

};

cv::Mat ViewModel::getImage(bool write) {
    cv::Mat image(700, 700, CV_32FC3, r->frame_buffer().data());
    image.convertTo(image, CV_8UC3, 1.0f);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
    if (write) {
        cv::imwrite(filename, image);
    }
    return image;
}

void ViewModel::redraw() {
    cout << "rendering" << endl;
    r->clear(rst::Buffers::Color | rst::Buffers::Depth);
    r->set_model(get_model_matrix(angle));
    r->set_view(get_view_matrix(eye_pos));
    r->set_projection(get_projection_matrix(eye_fov,aspect_ratio,zNear,zFar));
    auto start_t=clock();
    r->draw(*TriangleList);
    cout << '\r' << clock()-start_t << " ms" << ends;
    cout << "render completed" << endl;
}
