//
//  hankelet.cpp
//  hankelet
//
//  Created by Binlong Li on 3/16/13.
//  Copyright (c) 2013 Binlong Li. All rights reserved.
//

#include "hankel.h"

hankel::hankel(int _hws, int _buildMode):hws(_hws), buildMode(_buildMode), fs(2){}

hankel::hankel(const std::vector<cv::Point2f> &_input, int _hws, int _buildMode):traj(_input), hws(_hws), buildMode(_buildMode), fs(2){
    initialFunc();
    buildHankelMatrix();
}

hankel::hankel(const std::list<cv::Point2f> &_input, int _hws, int _buildMode):traj(_input.begin(), _input.end()), hws(_hws), buildMode(_buildMode), fs(2){
    initialFunc();
    buildHankelMatrix();
}

void hankel::setTraj(const std::vector<cv::Point2f> &_input){
    traj = _input;
}

void hankel::initialFunc(){
    s = static_cast<int>(traj.size());
    if (hws <= 0 || hws > s ) {
        CV_Error(CV_StsBadArg, "hws must be <= length of input");
    }
    
    rows = fs * hws;        // fs a variable for Template version hankelet
    cols = s - hws + 1;
    
    hankelMatrix.create(rows, cols);
}

void hankel::buildHankelMatrix(){
    switch (buildMode) {
        case 0:
            buildHankelMatrixMode00();
            break;
        case 1:
            buildHankelMatrixMode01();
            break;
        default:
            CV_Error(CV_StsBadArg, "only support 0 and 1 hankel Matrix build mode");
    }
}

void hankel::buildHankelMatrixMode00(){
    for (int i = 0; i < rows; i++) {
        float *ptr = hankelMatrix.ptr<float>(i);
        for (int j = 0; j < cols; j++) {
            if (i % fs == 0) {
                ptr[j] = traj[i/fs + j].x;
            }else{
                ptr[j] = traj[i/fs + j].y;
            }
        }
    }
}

void hankel::buildHankelMatrixMode01(){
    for (int i = 0; i < rows; i++) {
        float *ptr = hankelMatrix.ptr<float>(i);
        for (int j = 0; j < cols; j++) {
            if (i / hws == 0) {
                ptr[j] = traj[i + j].x;
            }else{
                ptr[j] = traj[i - hws + j].y;
            }
        }
    }
}