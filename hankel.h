//
//  hankelet.h
//  hankelet
//
//  Created by Binlong Li on 3/16/13.
//  Copyright (c) 2013 Binlong Li. All rights reserved.
//

#ifndef __hankel__hankel__
#define __hankel__hankel__

#include <iostream>
#include <vector>
#include <list>
#include <opencv2/opencv.hpp>

class hankel {
    std::vector<cv::Point2f> traj;    // original trajector
    cv::Mat_<float> hankelMatrix;         // final hankel matrix
    
    int hws;                // hankel window size
    int s;                  // length of traj
    int fs;                 // feature size, eg. int is 1, Point2f is 2
    int buildMode;          // Hankel matrix building
                            // mode 0: [x1 x2 x3; y1 y2 y3; x2 x3 x4; y2 y3 y4]
                            // or 1:   [x1 x2 x3; x2 x3 x4; y1 y2 y3; y2 y3 y4]
    
    int rows;
    int cols;
    
    
    void buildHankelMatrixMode00();
    void buildHankelMatrixMode01();
    
public:
    explicit hankel(int _hws, int _buildMode = 1);
    explicit hankel(const std::vector<cv::Point2f> &_input, int _hws, int _buildMode = 1);
    explicit hankel(const std::list<cv::Point2f> &_input, int _hws, int _buildMode = 1);
    
    void setTraj(const std::vector<cv::Point2f> &_input);
    void initialFunc();
    void buildHankelMatrix();
    
    int getHws() const;
    cv::Mat getHankel() const;
    void print() const;
};

inline int hankel::getHws() const{
    return hws;
}

inline cv::Mat hankel::getHankel() const{
    return hankelMatrix;
}

inline void hankel::print() const{
    std::cout << hankelMatrix << std::endl;
}

#endif /* defined(__hankel__hankel__) */
