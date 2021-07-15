
#include "viewmodel.hpp"
#include <ctime>

ViewModel vm;
extern Message msg_ctrl;

void ViewModel::showImage(bool write=false) {
    cv::Mat image(700, 700, CV_32FC3, r->frame_buffer().data());
    image.convertTo(image, CV_8UC3, 1.0f);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

    if (write) {
        cv::imwrite(filename, image);
    }
    cv::imshow("image", image);
}

void ViewModel::viewInit() {
    redraw();
    showImage();
    msg_ctrl.announceView();

    msg_ctrl.registerFunc("anglePlus",
                          [this](){this->modifyAngle(1);}
                          );
    msg_ctrl.registerFunc("angleMinus",
                          [this](){this->modifyAngle(-1);}
    );
    msg_ctrl.registerFunc("redraw",
                          [this](){this->redraw();}
    );
    msg_ctrl.registerFunc("imshow",
                          [this](){this->showImage();}
    );

    msg_ctrl.mainLoop();
};



void ViewModel::modifyAngle(const float &delta) {
    angle+=delta;
}
void ViewModel::modifyAngle(float &&delta) {
    angle+=delta;
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
