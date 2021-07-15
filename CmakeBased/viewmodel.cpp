#pragma once

#include "viewmodel.hpp"
#include "view.cpp"

ViewModel vm;

void ViewModel::viewInit(int &argc,const char **argv) {

    r->clear(rst::Buffers::Color | rst::Buffers::Depth);
    r->set_model(get_model_matrix(angle));
    r->set_view(get_view_matrix(eye_pos));
    r->set_projection(get_projection_matrix(eye_fov,aspect_ratio,zNear,zFar));

    cout << "View initialized" << endl;

    r->draw(*TriangleList);
    cv::Mat image(700, 700, CV_32FC3, r->frame_buffer().data());
    image.convertTo(image, CV_8UC3, 1.0f);
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

    cv::imshow("image", image);
    cv::setMouseCallback("image",mouseCallback);
    cv::imwrite(filename, image);

    viewLoop();
};


void ViewModel::viewLoop() {
    unsigned char key=0;
    while (key != 27) {

        r->draw(*TriangleList);
        cv::Mat image(700, 700, CV_32FC3, r->frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
        cv::imshow("image", image);
        key = cv::waitKey(10);
    }
}

void ViewModel::processViewMessage(string msg) {
    cout << "received " << msg << endl;
    if (msg == "a") {
        angle-=0.1;
    }
    else if (msg == "d") {
        angle+=0.1;
    }
    cout << angle << endl;
}
