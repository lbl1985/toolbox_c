#ifndef __iLIDS__util__
#define __iLIDS__util__

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <deque>

#include <opencv2/opencv.hpp>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

using namespace std;
using namespace rapidxml;
struct BB{
	unsigned int fnum;
	cv::Rect_<double> rect;
};

deque<cv::Ptr<BB> >  readViper(const std::string *fileName);

#endif