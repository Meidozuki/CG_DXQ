//
// Created by user on 2021/7/14.
//

#ifndef MAIN_CPP_VIEWMODEL_H
#define MAIN_CPP_VIEWMODEL_H

#include <iostream>
#include <Eigen/Core>
#include <memory>
#include "rasterizer.hpp"

extern Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos);
extern Eigen::Matrix4f get_model_matrix(float angle);
extern Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar);

using namespace std;

class ViewModel {
    rst::rasterizer *r;
    Eigen::VectorXf eye_pos;
    vector<Triangle*> *TriangleList;
    vector<shared_ptr<Triangle>> refs;
    float angle,eye_fov,aspect_ratio,zNear,zFar;
    string filename;

public:
    ViewModel(){
        r=nullptr;
        TriangleList=nullptr;
        angle=eye_fov=aspect_ratio=zNear=zFar=0;
    }

    ViewModel(rst::rasterizer &r,Eigen::Vector3f &eye_pos,vector<Triangle*> &TriangleList,
              string &filename,float angle,float eye_fov,float aspect_ratio,
              float zNear,float zFar) : angle(angle),eye_fov(eye_fov),
              aspect_ratio(aspect_ratio),zNear(zNear),zFar(zFar),filename(filename) {
        this->r=&r;
        this->eye_pos=eye_pos;
        this->TriangleList=&TriangleList;
        int i=0;
        for (auto tri : TriangleList) {
            this->refs.emplace_back(tri);
        }
    }
    ~ViewModel() {refs.clear();}


    cv::Mat getImage(bool write=false);

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
