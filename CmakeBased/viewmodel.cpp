
#include "viewmodel.hpp"

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
    msg_ctrl.toView();

    msg_ctrl.registerFunc("anglePlus",
                          [this](){this->modifyAngle(1);}
                          );
    msg_ctrl.registerFunc("angleMinus",
                          [this](){this->modifyAngle(-1);}
    );
    msg_ctrl.registerFunc("redraw",
                          [this](){this->redraw();}
    );

    viewLoop();
};

void ViewModel::viewLoop() {
    while (true) {
        msg_ctrl.callFunc("waitKey");
        showImage();
    }
}



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
    r->draw(*TriangleList);
    cout << "render completed" << endl;
}

void ViewModel::processViewMessage(int key) {
    cout << "received " << key << endl;
    switch (key) {
        case 'a':
            angle-=1;
            break;
        case 'd':
            angle+=1;
            break;
        default:
            break;
    }

    cout << angle << endl;
}


int&& receiveMessage(int &&key) {
    return forward<int>(key);
}