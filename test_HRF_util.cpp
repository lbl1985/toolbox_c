//
//  test_HRF_util.cpp
//  HRF
//
//  Created by Binlong Li on 3/28/13.
//  Copyright (c) 2013 Binlong Li. All rights reserved.
//

#include "HRF_util.h"
#include <fstream>

bool generateRandomW_test(){
    int N = 16 * 16;
    cv::Mat mat = generateRandomW(8, N);
//    std::cout << "after normalize" << std::endl << mat << std::endl;
    
    for (int i = 0; i < N; i++) {
        float dis = (float)(cv::norm(mat.col(i)) - 1.0);
        if (dis > 1e-6) {
            return false;
        }
    }
    return true;    
}

bool readTraj_test(){
    std::string fileName("/Users/herbert19lee/Develop/HRF/HRF/Utility/testData/trainPos.txt");
    std::vector<std::vector<float> > traj;
    int nFeature = 0;
    int ncenter = 0;
    readTraj(fileName, traj, nFeature, ncenter);
    
    return true;    
}

bool readAndloadW_test(){
    int N = 16 * 16;
    std::string fileName("/Users/herbert19lee/Desktop/testW.yml");
    cv::Mat W = generateRandomW(8, N);
    saveW(fileName, W);
    cv::Mat rW = loadW(fileName);
    for (int row = 0; row < 8; row ++) {
        float *ptr0 = W.ptr<float>(row);
        float *ptr1 = rW.ptr<float>(row);
        for (int j = 0; j < N; j++) {
            if (ptr0[j] != ptr1[j]) {
                std::cout << "readAndloadW_test failed" << std::endl;
                return false;
            }
        }
    }
    return true;
}

// main controller function
int main(){
    std::string testName("readAndloadW_test");
    
    if (testName.compare("generateRandomW_test") == 0) {
        if(generateRandomW_test())
            std::cout << "generateRandomW_test success" << std::endl;
        else{
            std::cout << "generateRandomW_test unSuccess" << std::endl;
        }
    }else if(testName.compare("readTraj_test") == 0){
        if(readTraj_test())
            std::cout << "readTraj_test success" << std::endl;
        else{
            std::cout << "readTraj_test unSuccess" << std::endl;
        }
    }else if (testName.compare("readAndloadW_test") == 0){
        if(readAndloadW_test()){
            std::cout << "readAndloadW_test success" << std::endl;
        }else{
            std::cout << "readAndloadW_test unsuccess" << std::endl;
        }
    }else if (false){
        NULL;
    }
    
    return 0;
}
