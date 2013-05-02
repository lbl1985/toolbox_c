//
//  HRF_util.h
//  HRF
//
//  Created by Binlong Li on 3/28/13.
//  Copyright (c) 2013 Binlong Li. All rights reserved.
//

#ifndef __HRF__HRF_util__
#define __HRF__HRF_util__

#define WIN32_LEAN_AND_MEAN 
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

#ifdef __APPLE__&&__MACH__
#include <boost/asio.hpp>
#else
#include <winsock.h>
#endif

#include <boost/filesystem.hpp>


// generate random weighting vector
// each weight vector is a featDim x 1 col, random values between [-1 1], normalized by norm2 distance
// return a featDim x N matrix
// data type is CV_32FC1
cv::Mat generateRandomW(int featDim, int N);

// read trajectory into memory
void readTraj(const std::string &fileName, std::vector< std::vector<float> > &traj, int &nFeature, int &ncenter);
// read test trajectory into memory
void readTestTraj(const std::string &fileName, std::vector< std::vector<float> > &traj, std::vector< std::vector<float> > &removeMeanTraj, int &nFeature, int &nDim);
// read test Images into container
void readTestImages(const std::string &fileName, std::vector<cv::Mat> &testImages);
// read testTrajFiles into memory
void readTestTrajFiles(const std::string &fileName, std::vector<std::vector<std::vector<float> > > &traj, std::vector<std::vector<std::vector<float> > > &removeMeanTraj, int &ncenter);
// read testImagFiles into memory
void readTestImagFiles(const std::string &fileName, std::vector<std::vector<cv::Mat> > &testImages);
// parse testTrajFiles for testVideoNames
void getTestVideoNames(const std::string &fileName, std::vector<std::string> &testVideoNames);

// get hostName
boost::filesystem::path getRootPath();


// work with getRootPath and file readin path to return the final assembled path string
std::string finalPath(const boost::filesystem::path &root, const char *buff);

// convert (x, y) to traj
std::vector<cv::Point2f> xy2traj(const std::vector<float> &xy);

// remove X/Y mean of a trajectory
std::vector<float> removeTrajMean(const std::vector<float> &traj);

// Save random projection W matrix
void saveW(const std::string &fileName, const cv::Mat &W);
// read random projection W matrix
cv::Mat loadW(const std::string &fileName);

// only draw one Trajectory
void drawTrajectory(std::vector<cv::Point2f>& points, cv::Mat& img, cv::Scalar color);

#endif /* defined(__HRF__HRF_util__) */
