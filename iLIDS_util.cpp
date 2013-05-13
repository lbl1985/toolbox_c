#include "stdafx.h"
#include "iLIDS_util.h"
#include "util.h"

vector<cv::Mat_<float> > readViper_iLIDS(const std::string *fileName){
	xml_document<> doc;
	ifstream theFile(*fileName);

	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	xml_node<> *viper_node = doc.first_node("viper");
	xml_node<> *data_node_out = viper_node->first_node("data");
	xml_node<> *source_node = data_node_out -> first_node("sourcefile");
		
	vector<cv::Mat_<float> > personBB;
	for (xml_node<> *object_node = source_node->first_node("object"); object_node; object_node = object_node -> next_sibling())
	{
		if (strcmp(object_node->first_attribute("name")->value(), "Target") == 0){
			cv::Mat_<float> pBB = readViper_iLIDS_Object(object_node);
			personBB.push_back(pBB);
		}
	}

	return personBB;
}

cv::Mat_<float> readViper_iLIDS_Object(xml_node<> *object_node){
	vector<vector<float> > tar_vBB;
	vector<vector<float> > vBB = readViper_iLIDS_Attribute("BOUNDING-BOX", object_node);
	vector<vector<float> > vOBB = readViper_iLIDS_Attribute("OCCLUDED-BOUNDING-BOX", object_node);
	vector<vector<float> > vIBB = readViper_iLIDS_Attribute("INITIAL-BOUNDING-BOX", object_node);
	vector<vector<float> > vIOBB = readViper_iLIDS_Attribute("INITIAL-OCCLUDED-BOUNDING-BOX", object_node);
	tar_vBB = vBB;
	tar_vBB.insert(tar_vBB.end(), vOBB.begin(), vOBB.end());
	tar_vBB.insert(tar_vBB.end(), vIBB.begin(), vIBB.end());
	tar_vBB.insert(tar_vBB.end(), vIOBB.begin(), vIOBB.end());
	sort2DVec(tar_vBB, 0);
	return vec2cvMat_2D(tar_vBB);
}

vector<vector<float> > readViper_iLIDS_Attribute(const char *attrName, xml_node<> *object_node){
	// vector enjoys constructor interface with opencv cv::Mat and armadillo::mat
	vector< vector<float> > vbbData;

	for(xml_node<> *attr_node = object_node->first_node("attribute"); attr_node; attr_node = attr_node->next_sibling()){
		if (strcmp( attr_node->first_attribute("name")->value(), attrName ) == 0)
		{
			int count = 0;
			for (xml_node<> *databb_node = attr_node->first_node("data:bbox"); databb_node; databb_node = databb_node->next_sibling())
			{
				vector<float> tmpData;
				tmpData.resize(5);
				string framespan(databb_node->first_attribute("framespan")->value());
				string frameStr = framespan.substr(0, framespan.find(":"));

				tmpData[0] = atof(frameStr.c_str());
				tmpData[1] = atof(databb_node->first_attribute("x")->value());
				tmpData[2] = atof(databb_node->first_attribute("y")->value());
				tmpData[3] = atof(databb_node->first_attribute("width")->value());
				tmpData[4] = atof(databb_node->first_attribute("height")->value());

				vbbData.push_back(tmpData);
			}
		}
	}
	return vbbData;
	/*cv::Mat_<float> bbMat = vec2cvMat_2D<float>(vbbData);
	std::cout << bbMat << std::endl;*/
}