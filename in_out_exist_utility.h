#ifndef IN_OUT_EXIST_UTILITY_H
#define IN_OUT_EXIST_UTILITY_H

//windows
//#include <windows.h>
#include <process.h>
//#include <afxmt.h>
//#include <afxwin.h>

#include <fftw3.h>
#include <opencv2/opencv.hpp>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <list>
#include <vector>
#include <string>
#include <deque>
#include <queue>
#include <array>
//#include <dirent.h>
#include <io.h>
#include <math.h>
#include <algorithm>
#include <limits>
#include <tuple>

#include "omp.h"

// tbb related libraries
#include "tbb/tbb.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"

//using namespace std;
//using namespace cv;


struct frameWithTimeInfo{
	cv::Mat frame;					// real frame information
	int64_t       m_lnPTS;          // presentation timestamp
	int64_t       m_lnFrameIdx;     // internal index of decoded picture
	int64_t       m_lnRTPTimestamp; // real rtp timestamp from network
	int           m_nWidth;         // picture width in pixel
	int           m_nHeight;        // picture height in pixel
	std::string	  timeString;		// recording time instance local time
	long		  fnumber;			// out recorded frame number;
};

//const double PI = 3.14159265;
//const double PI = 3.1415926536;
const double PI = 3.1415926535897932384626433832795;
// canny edge operation on input image
void canny(cv::Mat&, cv::Mat&);
// make the border of the the image as a special color
void makeSpecialColorBorder(cv::Mat& output, int borderWidth, cv::Scalar color);
// assign color/255 to a specific location on color/gray image
void assignValueOnImage(cv::Mat& image, int row, int col, cv::Scalar color);
// only draw one Trajectory
void drawTrajectory(std::vector<cv::Point2f> points, cv::Mat& img, cv::Scalar color);
// read the directories files names
void readDirectory( const std::string& directoryName, std::vector<std::string>& filenames, bool addDirectoryName=true );

// calculate foreground percent according to a bkgd binary images
double calForePercent(const cv::Mat& output);

// calculate foreground percent according to a bkgd binary images
double calForePercent(const cv::Mat& output, cv::Rect_<double> & rect);

//compare covariance matricies
float compareCov(cv::Mat a, cv::Mat b);
//calculate hog features
//based on code from http://www.mathworks.com/matlabcentral/fileexchange/33863
cv::Mat HOGfeatures(cv::Mat & img, int nb_bins = 9, double cheight = 8, double cwidth = 8, 
	int block_rows = 2, int block_cols = 2, bool orient = 0, double clip_val = 0.2);

//construct hankel matrix
cv::Mat constructHankel(std::vector<cv::Mat> & state, cv::Range rowrange = cv::Range(0,1), cv::Range colrange = cv::Range(0,1)); 


// Return a string with current time info;
std::string currentTime();
//convert time to correct format
std::string convertTime(time_t rawtime);
// Return a string with today in YYYY-MM-DD format
std::string todayDate();


//compare overlap of rectrangles
double compareOverlap(cv::Rect_<double> a, cv::Rect_<double> b);



//get_subwindow with repeated values
cv::Mat get_subwindow(cv::Rect r, const cv::Mat & img);


//circshift
cv::Mat circshiftrows(cv::Mat x, int n);
cv::Mat circshiftcols(cv::Mat x, int n);

//complex division
void complexDivision(cv::Mat & dst, cv::Mat n, cv::Mat d, cv::Scalar_<double> ds);
void complexDivision(cv::Mat & dst, cv::Mat n, cv::Mat d, cv::Scalar_<float> ds);



// Create a folder with under specific address
void createFolder(std::string folderName);

//wstring to string
std::string wstring2string(std::wstring ws);

//safe rect
cv::Mat safeRect(const cv::Mat & img, const cv::Rect & rect, cv::Rect & safe = cv::Rect());

//scale rect
inline cv::Rect_<double> scaleRect(cv::Rect_<double> & r, double s)
{
	return cv::Rect_<double>(r.x*s,r.y*s,r.width*s,r.height*s);
}


//get filename from path
std::string fileFromPath(std::string path);

// compute distance from two rect
double computeDistance(cv::Rect_<double> const &rect1, cv::Rect_<double> const &rect2);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//parabolic interp max
template <typename T>
void parabolicMax(const cv::Mat & m, cv::Point2d & maxLoc, double & maxVal, cv::Mat & mask = cv::Mat())
{
	cv::Point iml; // Integer Max Location
	cv::minMaxLoc(m, NULL, &maxVal, NULL, &iml);

	cv::Point2d dml(iml);

	if (iml.x < 1 || iml.x > m.cols-2)
	{
		maxLoc.x = dml.x;
	}
	else
	{
		maxLoc.x = dml.x + 0.5*(m.at<T>(iml.y,iml.x+1) - m.at<T>(iml.y,iml.x-1)) 
			/ (2.0*m.at<T>(iml)-m.at<T>(iml.y,iml.x+1) - m.at<T>(iml.y,iml.x-1));
	}

	if (iml.y < 1 || iml.y > m.rows-2)
	{
		maxLoc.y = dml.y;
	}
	else
	{
		maxLoc.y = dml.y + 0.5*(m.at<T>(iml.y+1,iml.x) - m.at<T>(iml.y-1,iml.x)) 
			/ (2.0*m.at<T>(iml)-m.at<T>(iml.y+1,iml.x) - m.at<T>(iml.y-1,iml.x));
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//center of mass
template <typename T>
cv::Point2d massCenter(const cv::Mat & img)
{
	double Moo = 0.0;
	double Mio = 0.0;
	double Moi = 0.0;
	double dimg;

	for (int i = 0; i < img.cols; i++)
	{
		for (int j = 0; j < img.rows; j++)
		{
			dimg = img.at<T>(j,i);
			Moo += dimg;
			Mio += i*dimg;
			Moi += j*dimg;
		}
	}

	cv::Point2d center(Mio / Moo, Moi / Moo);

	//cv::Mat out(img.size(), CV_8UC3);
	//cv::cvtColor(255*img,out, CV_GRAY2BGR);
	//cv::circle(out, center, 3, cv::Scalar(255,0,0), 2);
	//cv::destroyWindow("com");
	//cv::imshow("com",out);
	//cv::waitKey(10);
	//int x;
	//std::cin >> x;

	return center;
}


#endif // IN_OUT_EXIST_UTILITY_H
