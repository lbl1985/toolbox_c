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

bool vec2cvMat_2DTest(){
	// 2D vector to opencv mat conversion experiment
	vector<vector<float> > test_float;
	float f1[] = {1, 2, 3, 4, 5};
	float f2[] = {2, 3, 4, 5, 6};
	vector<float> tmpf1(f1, f1+5); test_float.push_back(tmpf1);
	vector<float> tmpf2(f2, f2+5); test_float.push_back(tmpf2);

	vector<vector<int> > test_int;
	int i1[] = {3, 5, 2};
	int i2[] = {8, 9, 10};
	vector<int> tmpi1(i1, i1+3); test_int.push_back(tmpi1);
	vector<int> tmpi2(i2, i2+3); test_int.push_back(tmpi2);
		
	cv::Mat_<float> testMat_float = vec2cvMat_2D(test_float);
	cv::Mat_<float> ansMat_float = (cv::Mat_<float>(2, 5) << 1, 2, 3, 4, 5, 2, 3, 4, 5, 6);

	cv::Mat_<int> testMat_int = vec2cvMat_2D(test_int);
	cv::Mat_<int> ansMat_int = (cv::Mat_<int>(2, 3) << 3, 5, 2, 8, 9, 10);
	
	if (!BasicUtil::matIsEqual(testMat_float, ansMat_float))
	{
		return false;
	}

	if (!BasicUtil::matIsEqual(testMat_int, ansMat_int))
	{
		return false;
	}
	return true;
}

bool sort2DVecTest(){
	int a[] = {2, 5, 6};	vector<int> vec1(a, a+3);
	int b[] = {-1, 3, 9};	vector<int> vec2(b, b+3);
	int c[] = {0, 9, 7};	vector<int> vec3(c, c+3);

	vector<vector<int> > testVec;
	vector<vector<int> > ansVec1; 

	testVec.push_back(vec1); testVec.push_back(vec2); testVec.push_back(vec3);
	ansVec1.push_back(vec2); ansVec1.push_back(vec3); ansVec1.push_back(vec1);

	sort2DVec(testVec, 0);
	cv::Mat_<int> testMat1 = vec2cvMat_2D(testVec);
	cv::Mat_<int> ansMat1 = vec2cvMat_2D(ansVec1);

	if (!BasicUtil::matIsEqual(testMat1, ansMat1))
	{
		return false;
	}

	sort2DVec(testVec, 1);
	vector<vector<int> > ansVec2;
	ansVec2.push_back(vec2); ansVec2.push_back(vec1); ansVec2.push_back(vec3);
	cv::Mat_<int> testMat2 = vec2cvMat_2D(testVec);
	cv::Mat_<int> ansMat2 = vec2cvMat_2D(ansVec2);

	if (!BasicUtil::matIsEqual(testMat2, ansMat2))
	{
		return false;
	}

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

	if (!vec2cvMat_2DTest())
	{
		std::cerr << "vec2cvMat_2DTest Failed" << std::endl;
		allPass = false;
	}

	if (!sort2DVecTest())
	{
		std::cerr<< "sort2DVecTest Failed" << std::endl;
		allPass = false;
	}

	if (allPass)
	{
		std::cout << "ALL util tests PASS!!!" << std::endl;
	}

    return 0;
}

