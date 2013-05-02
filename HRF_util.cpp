//
//  HRF_util.cpp
//  HRF
//
//  Created by Binlong Li on 3/28/13.
//  Copyright (c) 2013 Binlong Li. All rights reserved.
//

#include "HRF_util.h"

cv::Mat generateRandomW(int featDim, int N){
    cv::Mat mat(featDim, N, CV_32FC1);
    cv::randu(mat, cv::Scalar(-1.0), cv::Scalar(1.0));
    for (int i = 0; i < N; i++) {
        cv::normalize(mat.col(i), mat.col(i));
    }
    return mat;
}

// Read in trajectory files
void readTraj(const std::string &fileName, std::vector< std::vector<float> > &traj, int &nFeature, int &ncenter){
    int feaDim = 32;
    std::ifstream fs(fileName.c_str(), std::ios::in);
    
    if (fs.is_open()) {
        fs >> nFeature;
        traj.resize(nFeature);
        fs >> ncenter;
        
        for (int i = 0; i < nFeature; i++) {
            traj[i].resize(feaDim);
            for (int j = 0; j < feaDim; j++) {
                fs >> traj[i][j];
            }
        }
    }else{
        CV_Error(CV_StsBadArg, "could not open " + fileName);
    }
    std::cout << "read file: " << fileName << " successful!" << std::endl;
    fs.close();
}

void readTestTraj(const std::string &fileName, std::vector< std::vector<float> > &traj, std::vector< std::vector<float> > &removeMeanTraj, int &nFeature, int &nDim){
    std::ifstream fs(fileName.c_str(), std::ios::in);
    
    if (fs.is_open()) {
        fs >> nFeature;
        traj.resize(nFeature);
        removeMeanTraj.resize(nFeature);
        fs >> nDim;
        
        for (int i = 0; i < nFeature; i++) {
            traj[i].resize(nDim);
            for (int j = 0; j < nDim; j++) {
                fs >> traj[i][j];
            }
            removeMeanTraj[i] = removeTrajMean(traj[i]);
        }
    }else{
        CV_Error(CV_StsBadArg, "could not open " + fileName);
    }
    std::cout << "read file: " << fileName << " successful!" << std::endl;
    fs.close();
}

// read test Images filenames
void readTestImages(const std::string &fileName, std::vector<cv::Mat> &testImages){
    std::ifstream fs(fileName.c_str(), std::ios::in);
    
    char buffer[400];
    int nFrame;
    bool debug = false;
    
    std::string tmpstr;
    fs >> nFrame;
    testImages.resize(nFrame);
    fs.getline(buffer, 400);
    
    if (debug) {
        cv::namedWindow("debug");
    }
    
    for (int i = 0; i < nFrame; i++) {
        fs.getline(buffer, 400);
        tmpstr = buffer;
        testImages[i] = cv::imread(tmpstr);
        if (debug) {
            cv::imshow("debug", testImages[i]);
            cv::waitKey(30);
        }
    }
    fs.close();
}

// read test Images into container
void readTestTrajFiles(const std::string &fileName, std::vector<std::vector<std::vector<float> > > &traj, std::vector<std::vector<std::vector<float> > > &removeMeanTraj, int &ncenter){
    std::ifstream fs(fileName.c_str(), std::ios::in);
    
    char buffer[400];
    int nFile;
    int nFeature;
    int nDim;
    std::string tmpTestTrajFileName;
    
    if (fs.is_open()) {
        fs >> nFile;
        traj.resize(nFile);
        removeMeanTraj.resize(nFile);
        fs.getline(buffer, 400);
        
        for (int i = 0; i < nFile; i++) {
            fs.getline(buffer, 400);
            tmpTestTrajFileName = buffer;
            readTestTraj(tmpTestTrajFileName, traj[i], removeMeanTraj[i], nFeature, nDim);
        }        
        
    }else{
        CV_Error(CV_StsBadArg, "could not open " + fileName);
    }
    std::cout << "read TestTrajFiles: " << fileName << " successful!" << std::endl;
    fs.close();
}

// read testImagFiles into memory
void readTestImagFiles(const std::string &fileName, std::vector<std::vector<cv::Mat> > &testImages){
    std::ifstream fs(fileName.c_str(), std::ios::in);
    
    char buffer[400];
    int nVideo;
    std::string tmpTestImagFileName;
    
    if (fs.is_open()) {
        fs >> nVideo;
        testImages.resize(nVideo);
        fs.getline(buffer, 400);
        
        for (int i = 0;  i < nVideo; i++) {
            fs.getline(buffer, 400);
            tmpTestImagFileName = buffer;
            readTestImages(tmpTestImagFileName, testImages[i]);
        }
    }else{
        CV_Error(CV_StsBadArg, "could not open " + fileName);
    }
    std::cout << "read TestImagFiles: " << fileName << " successful!" << std::endl;
    fs.close();
}

// parse testTrajFiles for testVideoNames
void getTestVideoNames(const std::string &fileName, std::vector<std::string> &testVideoNames){
    std::ifstream fs(fileName.c_str(), std::ios::in);
    
    char buffer[400];
    int nVideo;
    std::string tmpTestImagFileName;
    
    if (fs.is_open()) {
        fs >> nVideo;
        testVideoNames.resize(nVideo);
        fs.getline(buffer, 400);
        
        for (int i = 0;  i < nVideo; i++) {
            fs.getline(buffer, 400);
            tmpTestImagFileName = buffer;
            boost::filesystem::path p = tmpTestImagFileName;
            
            testVideoNames[i] = p.parent_path().filename().string();
        }
    }else{
        CV_Error(CV_StsBadArg, "could not open " + fileName);
    }
    std::cout << "read TestImagFiles: " << fileName << " successful!" << std::endl;
    fs.close();
}

// get hostName
boost::filesystem::path getRootPath(){
	std::string hn;
#ifdef __APPLE__&&__MACH__
	hn = boost::asio::ip::host_name();
#else
	char myhostname[256]; 
	int rc = gethostname(myhostname, sizeof myhostname);
	hn = myhostname;
#endif
    
    boost::filesystem::path p;
    
    if (hn.compare("Binlongs-MacBook-Pro.local") == 0) {
        boost::filesystem::path tmp("/Users/herbert19lee/Develop/HRF");
        p = tmp;
    }else if (hn.compare("binlonglis-imac.wg.neu.edu") == 0){
        boost::filesystem::path tmp("/Users/binlongli/Develop/hrf");
        p = tmp;
    }else if (hn.compare("binlonglis-imac.local")){
        boost::filesystem::path tmp("/Users/binlongli/Develop/HRF");
        p = tmp;
    }else{
        CV_Error(CV_StsBadFlag, "Does not support " + hn + " machine. Only support Binlongs-MacBook-Pro.local and binlonglis-imac-wg.neu.edu for now.");
    }
    return p;
}

// work with getRootPath and file readin path to return the final assembled path string
std::string finalPath(const boost::filesystem::path &root, const char *buffer){
    std::string arg(buffer);
    boost::filesystem::path argPath(arg);
    return (root/argPath).string();
}

// convert (x, y) sequence to traj of cv::Point2f
std::vector<cv::Point2f> xy2traj(const std::vector<float> &xy){
    size_t s = xy.size();
    if (s % 2 != 0) {
        CV_Error(CV_StsBadArg, "input xy size: should be even number");
    }
    std::vector<cv::Point2f> res;
    for (int i = 0; i < s; i = i+2) {
        cv::Point2f pt(xy[i], xy[i+1]);
        res.push_back(pt);
    }
    return res;
}

// remove X/Y mean of a trajectory
std::vector<float> removeTrajMean(const std::vector<float> &traj){
    float XSum = 0;
    float YSum = 0;
    
    std::vector<float> tmpVec;
    size_t s = traj.size();
    size_t st = s - 1; // target size
    tmpVec.resize(st);
    
    for (int i = 3; i < s; i++) {
        if(i%2 == 1){
            XSum += traj[i];
        }else{
            YSum += traj[i];
        }
    }
    XSum /= (s-3)/2;
    YSum /= (s-3)/2;
    
    for (int i = 3; i < s; i++) {
        if (i % 2 == 1) {
            tmpVec[i-3] = (float)traj[i] - XSum;
        }else{
            tmpVec[i-3] = (float)traj[i] - YSum;
        }
    }
    // in order to fit to the train interface, where the last two digits should be dx dy. 
    tmpVec[s-3] = 0;
    tmpVec[s-2] = 0;
    return tmpVec;
}

// Save random projection W matrix
void saveW(const std::string &fileName, const cv::Mat &W){
    cv::FileStorage fs(fileName, cv::FileStorage::WRITE);
    fs << "W" << W;
    fs.release();
}

// read random projection W matrix
cv::Mat loadW(const std::string &fileName){
    cv::Mat W;
    cv::FileStorage fs(fileName, cv::FileStorage::READ);
    fs["W"] >> W;
    fs.release();
    return W;    
}

// only draw one Trajectory
void drawTrajectory(std::vector<cv::Point2f>& points, cv::Mat& img, cv::Scalar color){
	if(points.size() > 0){
		std::vector<cv::Point2f>::iterator it = points.begin();
		for(int i = 0; i < points.size() - 1; i++){
			cv::Point2f point0 = *it;
			cv::Point2f point1 = *(++it);
			cv::line(img, point0, point1, color);
		}
		cv::circle(img, *it, 3, color, -1);
	}
}