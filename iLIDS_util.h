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

vector<cv::Mat_<float> > readViper_iLIDS(const std::string *fileName);
cv::Mat_<float> readViper_iLIDS_Object(xml_node<> *object_node);
vector<vector<float> > readViper_iLIDS_Attribute(const char *attrName, xml_node<> *object_node);

#endif