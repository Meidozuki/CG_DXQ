//
// Created by user on 2021/7/14.
//

#ifndef MAIN_CPP_VIEWMODEL_H
#define MAIN_CPP_VIEWMODEL_H

#include <iostream>
#include <Eigen/Core>
#include "rasterizer.hpp"
using namespace std;

class ViewModel {
public:
    rst::rasterizer *r;
    Eigen::VectorXf eye_pos;
    vector<Triangle*> *TriangleList;
    float angle,eye_fov,aspect_ratio,zNear,zFar;
    string filename;

    ViewModel(){
        r=nullptr;
        TriangleList=nullptr;
        angle=eye_fov=aspect_ratio=zNear=zFar=0;
    }

    ViewModel(rst::rasterizer &r,Eigen::Vector3f &eye_pos,vector<Triangle*> &TriangleList,
              string &filename,float angle,float eye_fov,float aspect_ratio,
              float zNear,float zFar):angle(angle),eye_fov(eye_fov),aspect_ratio(aspect_ratio),
              zNear(zNear),zFar(zFar),filename(filename) {
        this->r=&r;
        this->eye_pos=eye_pos;
        this->TriangleList=&TriangleList;
    }

    void viewInit(int &argc,const char **argv);
    void processViewMessage(string msg);
    void viewLoop();
};

#endif //MAIN_CPP_VIEWMODEL_H
