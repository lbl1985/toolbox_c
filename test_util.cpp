//
//  test_util.cpp
//  seed
//
//  Created by Binlong Li on 2/20/13.
//  Copyright (c) 2013 Binlong Li. All rights reserved.
//

#ifdef _WIN32 || _WIN64

#include "stdafx.h"

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
        ImageUtil::resize(dst, src, 1/(cv::pow(static_cast<double>(2), static_cast<double>(i))));
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
    
    //std::cout << "The min element is at: " << *result << " at: " << index << std::endl;
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
    //std::for_each(res_index.begin(), res_index.end(), std::cout << _1 << " ");
    //std::cout << std::endl;
    //std::for_each(sorted.begin(), sorted.end(), std::cout << _1 << " ");
    //std::cout << std::endl;
    return true;
}

// -----------------------------------------------------
// --------------- TEST: FileUtil Class ---------------
// -----------------------------------------------------
// ---------------  TEST1: fullfile   -------------
// -----------------------------------------------------
bool fullfileTest(){
    std::string path = fullfile(3, "/Users/herbert19lee/Develop", "HRF", "Util");

    if (path.compare("/Users/herbert19lee/Develop/HRF/Util") != 0) {
        return false;
    }
    //std::cout << path << std::endl;
    return true;
}

// -----------------------------------------------------
// ---------------  TEST2: ls / folderList /checkFolder  -------------
// -----------------------------------------------------
// Not Test, just for usage example
// fullfile requre c_str() (c style input) 
// std::string imgFolder("/Users/herbert19lee/Dropbox/Upper_Body_Data/img");
// std::vector<std::string> vec = ls( fullfile(2, imgFolder.c_str(), "*.jpg") );
//std::vector<std::string> vec = ls(fullfile(2, "/Users/herbert19lee/Dropbox/Upper_Body_Data/img", "*.jpg"));
//BOOST_FOREACH(std::string s, vec){
//    std::cout << s << std::endl;
//}
//std::vector<std::string> folders = folderList(fullfile(2, "/Users/herbert19lee/Develop/HRF/MATLAB_Files/data/", "tracklet").c_str());
//BOOST_FOREACH(std::string str, folders){
//    std::cout << str << std::endl;
//}
//checkFolder(fullfile(3, "/Users/herbert19lee", "Desktop",  "test"));

// -----------------------------------------------------
// ---------------  TEST3: num2str2  -------------
// -----------------------------------------------------
bool num2str2Test(){
	int intNum = 20;
	long longNum = 200000;
	double doubleNum = 20.3;
	double negNum = -20.3;
	int len = 5;

	std::string intStr(num2str2<int>(intNum, len));
	std::string longStr(num2str2<long>(longNum, len));
	std::string doubleStr(num2str2<double>(doubleNum, len));
	std::string negStr(num2str2<double>(negNum, len));

	if (intStr.compare("00020") != 0 || longStr.compare("200000") != 0 || doubleStr.compare("020.3") != 0 || negStr.compare("-20.3") != 0 )
	{
		return false;
	}
	return true;
}

// main controller function
int main(){

	/*std::string testName("fullfileTest");

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
	}else if (testName.compare("fullfileTest") == 0){
	fullfileTest();
	}*/
	bool allPass = true;
	if (!minValueIndexTest())
	{
		std::cerr << "minValueIndexTest Failed;" << std::endl;
		allPass = false;
	}

	if(!sort_indexTest()){
		std::cerr << "sort_indexTest Failed;" << std::endl;
		allPass = false;
	}

	if (!fullfileTest())
	{
		std::cerr << "fullfileTest Failed;" << std::endl;
		allPass = false;
	}
    
    if(!num2str2Test()){
		std::cerr << "num2str2Test Failed" << std::endl;
		allPass = false;
	}

	if (allPass)
	{
		std::cout << "ALL util tests PASS!!!" << std::endl;
	}

    return 0;
}

