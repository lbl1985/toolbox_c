//
//  HRF_util.h
//  HRF
//
//  Created by Binlong Li on 3/28/13.
//  Copyright (c) 2013 Binlong Li. All rights reserved.
//

#ifndef __HRF__HRF_util__
#define __HRF__HRF_util__

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <armadillo>

// generate random weighting vector
// each weight vector is a featDim x 1 col, random values between [-1 1], normalized by norm2 distance
// return a featDim x N matrix
// data type is CV_32FC1
cv::Mat generateRandomW(int featDim, int N);

// read trajectory into memory (without MATLAB operation)
// only read in trajectory, do not do any extra operation
void readTraj(const std::string &fileName, std::vector< std::vector<float> > &traj);
// return mat file instead of vector<vector<float> > traj
arma::fmat readTraj(const std::string &fileName);

// read trajectory into memory (file with MATLAB)
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

// =============================================
// ============== Data Transform ==============
// =============================================
// convert (x, y) to traj
std::vector<cv::Point2f> xy2traj(const std::vector<float> &xy);

// remove X/Y mean of a trajectory
std::vector<float> removeTrajMean(const std::vector<float> &traj);

// translate armadillo 2D matrix to vector< vector<double> >
std::vector< std::vector<float> > arma2vec(arma::fmat const &m);

// translate armadillo 2D matrix to OpenCV cv::Mat
cv::Mat arma2cv(arma::fmat &arMat);

// translate OpenCV cv::Mat to armadillo 2D matrix
arma::fmat cv2arma(cv::Mat const &cvMat);

// =============================================
// ==================== I/O ====================
// =============================================

// Save random projection W matrix
void saveW(const std::string &fileName, const cv::Mat &W);
// read random projection W matrix
cv::Mat loadW(const std::string &fileName);

// only draw one Trajectory
void drawTrajectory(std::vector<cv::Point2f>& points, cv::Mat& img, cv::Scalar color);

#endif /* defined(__HRF__HRF_util__) */
