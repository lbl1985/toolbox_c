//
//  test_util.cpp
//  seed
//
//  Created by Binlong Li on 2/20/13.
//  Copyright (c) 2013 Binlong Li. All rights reserved.
//

#include "util.h"

// -----------------------------------------------------
// --------------- TEST: VideoUtil Class ---------------
// -----------------------------------------------------
int VideoShowTest(){
    std::string fileName("/Volumes/Security/sourceVideo/CLE_Data_20120830/camera_10_23.mp4");
    
    try{
        VideoUtil v(fileName);
        v.showVideo();
        //        v.saveSampleFrames();
    }catch(myexception &e){
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int VideoSaveTest(){
    std::string fileName("/Volumes/Security/sourceVideo/CLE_Data_20120830/camera_10_23.mp4");
    
    try{
        VideoUtil v(fileName);
        v.saveSampleFrames();
    }catch(myexception &e){
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

// -----------------------------------------------------
// --------------- TEST: ImageUtil Class ---------------
// -----------------------------------------------------
// --------------- TEST1: ImageUtil:resize -------------
// -----------------------------------------------------
int ImageResize(){
    std::string imgName("/Users/herbert19lee/Downloads/lena.jpg");
    cv::Mat src, dst;
    src = cv::imread(imgName);
    cv::namedWindow("orig", CV_WINDOW_AUTOSIZE);
    cv::imshow("orig", src);
    
    std::string str[3];
    str[0] = "r1/2"; str[1] = "r1/4"; str[2] = "r1/8";
    
    for (int i = 1; i <= 3; i++) {
        cv::namedWindow(str[i-1], CV_WINDOW_AUTOSIZE);
        ImageUtil::resize(dst, src, 1/(pow(2, i)));
        cv::imshow(str[i-1], dst);
    }
    cv::waitKey(0);
    return 0;
}

bool combineTwoImagesTest(){
    std::string imgName("/Users/herbert19lee/Downloads/lena.jpg");
    cv::Mat img1 = cv::imread(imgName);
    cv::Mat img2 = cv::imread(imgName);
    cv::Mat dst;
    ImageUtil::combineTwoImages(dst, img1, img2);
    cv::namedWindow("combine");
    cv::imshow("combine", dst);
    cv::waitKey();
    
    return true;
}

// -----------------------------------------------------
// --------------- TEST: BasicUtil Class ---------------
// -----------------------------------------------------
// ---------------  TEST1: minValueIndex   -------------
// -----------------------------------------------------
bool minValueIndexTest(){
    int data[] = {3, 2, 1, 5, 4, 2, 1, 0, 7};
    std::vector<int> dVec(data, data + sizeof(data) / sizeof(int));
    
    std::vector<int>::iterator result = std::min_element(dVec.begin(), dVec.end());
    long index = std::distance(dVec.begin(), result);
    
    std::cout << "The min element is at: " << *result << " at: " << index << std::endl;
    if (index != 7) {
        return false;
    }
    return true;
}

// -----------------------------------------------------
// --------------- TEST: Numeric Class ---------------
// -----------------------------------------------------
// ---------------  TEST1: sort_index   -------------
// -----------------------------------------------------
bool sort_indexTest(){
    int data[] = {3, 8, 5};
    std::vector<double> dVec(data, data + sizeof(data) / sizeof(int));
    std::vector<double> sorted;
    
    std::vector<size_t> res_index;
    res_index = NumericUtil::sort_index(dVec, sorted);
    std::for_each(res_index.begin(), res_index.end(), std::cout << _1 << " ");
    std::cout << std::endl;
    std::for_each(sorted.begin(), sorted.end(), std::cout << _1 << " ");
    std::cout << std::endl;
    return true;
}

// main controller function
int main(){
    std::string testName("sort_indexTest");
    
    if (testName.compare("VideoShow") == 0) {
        VideoShowTest();
    }else if (testName.compare("VideoSave") == 0){
        VideoSaveTest();
    }else if (testName.compare("ImageResize") == 0){
        ImageResize();
    }else if (testName.compare("minValueIndex") == 0){
        minValueIndexTest();
    }else if (testName.compare("combineTwoImages") == 0){
        combineTwoImagesTest();
    }else if (testName.compare("sort_indexTest") == 0){
        sort_indexTest();
    }
    return 0;
}

