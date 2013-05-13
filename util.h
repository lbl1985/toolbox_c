//
//  util.h
//  seed
//
//  Created by Binlong Li on 2/20/13.
//  Copyright (c) 2013 Binlong Li. All rights reserved.
//

#ifndef __seed__util__
#define __seed__util__

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <deque>
#include <queue>
#include <array>
#include <algorithm>
#include <limits>
#include <tuple>

#include <utility>
#include <iomanip>
#include <exception>
#include <iterator>
#include <cmath>
#include <cstdarg>
#include <opencv2/opencv.hpp>
#include <boost/regex.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/filesystem.hpp>
#include <boost/type_traits.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/foreach.hpp>
#include <armadillo>

#ifdef _WIN32 || _WIN64
#include "in_out_exist_utility.h"
#endif
#include "iLIDS_util.h"

#include <fftw3.h>
#include <fstream>
#include <stdlib.h>
#include <io.h>
#include <math.h>
#include <omp.h>

#include <tbb/tbb.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>

using namespace boost::lambda;


// --------------- Exception ---------------
struct myexception : public std::exception{
    std::string s;
    myexception(std::string ss):s(ss){}
    virtual const char* what() const throw() {return s.c_str();}
    ~myexception() throw(){}
};

// -----------------------------------------------------
// ---------------     Video System      ---------------
// -----------------------------------------------------
// show the frames of one video
// if any error or unexpected happens return false, otherwise return true

class VideoUtil {    
public:
    VideoUtil(std::string const _videoName);    
    //show the frames of one video
    bool showVideo();
    // save sample frames: if endIndex == -1 we will save all the frames
    bool saveSampleFrames();
    
private:
    std::string videoName;
    cv::VideoCapture cap;
    
    cv::Mat image;
    
    std::string prefix;
    std::string ext;
    int numberOfDigits;
    int startIndex;
    int endIndex;
};

// support function for saveSampleFrames function
void writeFrameCore(cv::Mat const &frame, int &count, std::string const &prefix, std::string const &ext, int const numberOfDigits);

// -----------------------------------------------------
// ---------------     Image System      ---------------
// -----------------------------------------------------
class ImageUtil{
public:
    static void resize(cv::Mat &dst, cv::Mat const &src, double scale);
    // combine two identical size images into one big image
    static void combineTwoImages(cv::Mat &dst, cv::Mat const img1, cv::Mat const img2);
};

// -----------------------------------------------------
// ---------------   Numeric System      ---------------
// -----------------------------------------------------
class NumericUtil{
public:
    // Convert mat into dynamic_bitset
    static void mat2bitset(cv::Mat const &desc, std::vector<boost::dynamic_bitset<> > &vb, int const nByte);
    // Count one entries within dynamic_bitset
    static void countBitset(std::vector<boost::dynamic_bitset<> > const &vb, std::vector<size_t> &vc);
    static cv::Point2f middlePoint(cv::Point2f const &p1, cv::Point2f const &p2);
    static double distance(cv::Point2f const &p1, cv::Point2f const &p2);
    static std::vector<size_t> sort_index(const std::vector<double> &vec, std::vector<double> &sorted);
};

template <typename T>
cv::Mat_<T> vec2cvMat_2D(std::vector< std::vector<T> > &inVec){
	int rows = static_cast<int>(inVec.size());
	int cols = static_cast<int>(inVec[0].size());

	cv::Mat_<T> resmat(rows, cols);
	for (int i = 0; i < rows; i++)
	{
		resmat.row(i) = cv::Mat(inVec[i]).t();
	}
	return resmat;
}

template <typename T>
class SortVec{
private: 
	unsigned int fieldId;
public:
	SortVec(int _fieldId = 0):fieldId(_fieldId){}
	bool operator() (vector<T> vec1, vector<T> vec2){ 
		if (fieldId >= vec1.size())
		{
			std::cerr << "fieldId number is >= size of vec" << std::endl;
			throw myexception("fieldId number is >= size of vec");
		}
		return vec1[fieldId] < vec2[fieldId];
	} 
};

// sort 2D vector according to the fieldId of each row vector
template <typename T> 
void sort2DVec(vector<vector<T> > &vec, unsigned int fieldId = 0){
	SortVec<T> mySort(fieldId);
	sort(vec.begin(), vec.end(), mySort);
}

// -----------------------------------------------------
// ---------------     Basic System      ---------------
// -----------------------------------------------------
class BasicUtil {
public:
    // compare two mat, return whether two mat is identical
    static bool matIsEqual(cv::Mat const mat1, cv::Mat const mat2);
    // given a vector data, return the index of the minmum value;
    template <class T>
    static int minValueIndex(std::vector<T> vec){
        typename std::vector<T>::iterator result;
        result = std::min_element(vec.begin(), vec.end());
        return std::distance(vec.begin(), result);
        
    }
};

// -----------------------------------------------------
// ---------------     Feature System    ---------------
// -----------------------------------------------------
class FeatureUtil{
public:
    // discovery which feature keypoints are kept after DescriptorExtract::compute
    static void keptKPId(std::vector<cv::KeyPoint> const &cKP, std::vector<cv::KeyPoint> const &oKP, std::vector<int> &index);
};

// -----------------------------------------------------
// ---------------      File System      ---------------
// -----------------------------------------------------
class FileUtil{
public:
    template <typename T>
    static bool write(std::string const fileName, std::string const variableName, T const &mat){
        cv::FileStorage f(fileName, cv::FileStorage::WRITE);
        if (!f.isOpened()) {
            return false;
        }
        if (boost::is_same<T, cv::Mat>::value){
            f << variableName << mat;
        }        
        f.release();
        return true;
    }
    
    template <typename T>
    static bool read(std::string const fileName, std::string const varialbeName, T &mat){
        cv::FileStorage f(fileName, cv::FileStorage::READ);
        if (!f.isOpened()) {
            return false;
        }
        f[varialbeName] >> mat;
        f.release();
        return true;
    }    
};

// fullfile as in MATLAB
std::string fullfile(int num, ...);

// helper function for ls
std::string to_regex_copy(std::string const &mask);
// ls as in POSIX system
std::vector<std::string> ls(const std::string pcszMask);

// folderList, return folders under one directory
std::vector<std::string> folderList(std::string const &inPath);
// check one path is exist or not, if not create it.
bool checkFolder(std::string const &path);
// num2str2 only accept long
template <typename T>
std::string num2str2(T in, int len){
	std::stringstream ss;

	if (in < 0)
	{
		ss << "-" << std::setw(len-1) << std::setfill('0') << -in;
	}else{
		ss << std::setw(len) << std::setfill('0') << in;
	}	
	
	return ss.str();
}

// -----------------------------------------------------
// ---------------      Display System      ---------------
// -----------------------------------------------------

void DrawProgressBar(int len, double percent);

template <typename T>
void print2DVect(std::vector<std::vector<T> > const &vec){
	BOOST_FOREACH(std::vector<T> v, vec){
		BOOST_FOREACH(T data, v){
			std::cout << data << " ";
		}
		std::cout << std::endl;
	}
}

// -----------------------------------------------------
// ---------------      Armadillo System      ---------------
// -----------------------------------------------------

arma::uvec allIndex(arma::uword s);
arma::uvec everyNIndex(arma::uword start, arma::uword end, arma::uword step);

#endif /* defined(__seed__helper__) */
