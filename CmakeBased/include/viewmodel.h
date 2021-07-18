//
// Created by user on 2021/7/14.
//

#ifndef MAIN_CPP_VIEWMODEL_H
#define MAIN_CPP_VIEWMODEL_H

#include <iostream>
#include <Eigen/Core>
#include <memory>
#include <cstdint>
#include "rasterizer.hpp"
#include "config.h"

using namespace std;

class ViewModel {
    rst::rasterizer r;
    Eigen::Vector3f eye_pos;
    vector<shared_ptr<Triangle>> TriangleList;
    float angle,eye_fov,aspect_ratio,zNear,zFar;
    string filename,cur_path;
    typedef uint16_t range_t;
public:
    ViewModel():r(0,0){
        angle=eye_fov=aspect_ratio=zNear=zFar=0;
    }

    ViewModel(range_t w,range_t h,Eigen::Vector3f &eye_pos,string &filename,
              float angle,float eye_fov,float aspect_ratio,float zNear,float zFar) :
              r(w,h),eye_pos(eye_pos),angle(angle),eye_fov(eye_fov),filename(filename),
              cur_path(CUR_SRC_DIR),aspect_ratio(aspect_ratio),zNear(zNear),zFar(zFar) {
        ;
    }
    ~ViewModel() {TriangleList.clear();}


    cv::Mat getImage(bool write=false);

    void model_main();
    void vmInit();
    void redraw();

    //(default inline) func
    void modifyAngle(const float &delta) {
        angle+=delta;
    }
    void modifyAngle(float &&delta) {
        angle+=delta;
    }
    void modifyEyepos(const float &delta,const int &id) {
        eye_pos[id]+=delta;
    }
    void modifyEyepos(float &&delta,int &&id) {
        eye_pos[id]+=delta;
    }
};

#endif //MAIN_CPP_VIEWMODEL_H
