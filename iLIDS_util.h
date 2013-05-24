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

struct targetPerson{
	targetPerson();
	string framespan;
	string id;
	string NAME;
	string DRESS;
	string SEX;
	string COLOUR;
	string BAG;
	cv::Mat_<float> BOUNDING_BOX;
	cv::Mat_<float> OCCLUDED_BOUNDING_BOX;
	cv::Mat_<float> INITIAL_BOUNDING_BOX;
	cv::Mat_<float> INITIAL_OCCLUDED_BOUNDING_BOX;
	cv::Mat_<float> BB;
};

class iLIDS_Reader{
public:
	iLIDS_Reader(const std::string *_fileName);

	vector<targetPerson > read();	

private:
	string fileName;
	vector<string> attrNames;

	// read information for each object
	targetPerson readViper_iLIDS_Object(xml_node<> *object_node);
	// read Bounding box related attribute information 
	vector<vector<float> > readViper_iLIDS_Attribute_BB(xml_node<> *object_node, const char *attrName);
	// read other attr
	string read_iLIDS_other(xml_node<> *object_node, const char *attrName);	
	// read framespan and id 
	string read_iLIDS_fs_id(xml_node<> *object_node, const char *attrNames);
	

};




#endif