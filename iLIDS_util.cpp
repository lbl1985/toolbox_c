#include "stdafx.h"
#include "iLIDS_util.h"
#include "util.h"

targetPerson::targetPerson(){
	framespan.resize(2);
}

iLIDS_Reader::iLIDS_Reader(const std::string *_fileName){
	fileName = *_fileName;

	//attrNames.push_back("NAME");
	//attrNames.push_back("DRESS");
	//attrNames.push_back("SEX");
	//attrNames.push_back("COLOUR");
	//attrNames.push_back("BAG");
	attrNames.push_back("BOUNDING-BOX");
	attrNames.push_back("OCCLUDED-BOUNDING-BOX");
	attrNames.push_back("INITIAL-BOUNDING-BOX");
	attrNames.push_back("INITIAL-OCCLUDED-BOUNDING-BOX");
}

vector<targetPerson> iLIDS_Reader::read(){
	xml_document<> doc;
	ifstream theFile(fileName);		

	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	xml_node<> *viper_node = doc.first_node("viper");
	xml_node<> *data_node_out = viper_node->first_node("data");
	xml_node<> *source_node = data_node_out -> first_node("sourcefile");
		
	vector<targetPerson> PERSONs;

	for (xml_node<> *object_node = source_node->first_node("object"); object_node; object_node = object_node -> next_sibling())
	{
		if (strcmp(object_node->first_attribute("name")->value(), "Target") == 0){
			targetPerson tp = readViper_iLIDS_Object(object_node);
			PERSONs.push_back(tp);
		}
	}

	return PERSONs;
}



targetPerson iLIDS_Reader::readViper_iLIDS_Object(xml_node<> *object_node){

	targetPerson tp;
	tp.framespan = read_iLIDS_fs_id(object_node, "framespan");
	tp.id		= read_iLIDS_fs_id(object_node, "id");
	tp.NAME		= read_iLIDS_other(object_node, "NAME");
	tp.DRESS	= read_iLIDS_other(object_node, "DRESS");
	tp.SEX		= read_iLIDS_other(object_node, "SEX");
	tp.COLOUR	= read_iLIDS_other(object_node, "COLOUR");
	tp.BAG		= read_iLIDS_other(object_node, "BAG");	
	
	vector<vector<float> > tar_vBB;
	vector<vector<float> > tmpBB;

	tmpBB = readViper_iLIDS_Attribute_BB(object_node, "BOUNDING-BOX");
	tar_vBB.insert(tar_vBB.end(), tmpBB.begin(), tmpBB.end());
	tp.BOUNDING_BOX = vec2cvMat_2D(tmpBB);

	tmpBB.clear();
	tmpBB = readViper_iLIDS_Attribute_BB(object_node, "OCCLUDED-BOUNDING-BOX");
	tar_vBB.insert(tar_vBB.end(), tmpBB.begin(), tmpBB.end());
	tp.OCCLUDED_BOUNDING_BOX = vec2cvMat_2D(tmpBB);

	tmpBB.clear();
	tmpBB = readViper_iLIDS_Attribute_BB(object_node, "INITIAL-BOUNDING-BOX");
	tar_vBB.insert(tar_vBB.end(), tmpBB.begin(), tmpBB.end());
	tp.INITIAL_BOUNDING_BOX = vec2cvMat_2D(tmpBB);

	tmpBB.clear();
	tmpBB = readViper_iLIDS_Attribute_BB(object_node, "INITIAL-OCCLUDED-BOUNDING-BOX");
	tar_vBB.insert(tar_vBB.end(), tmpBB.begin(), tmpBB.end());
	tp.INITIAL_OCCLUDED_BOUNDING_BOX = vec2cvMat_2D(tmpBB);

	tp.BB = vec2cvMat_2D(tar_vBB);

	return tp;
}

vector<vector<float> > iLIDS_Reader::readViper_iLIDS_Attribute_BB(xml_node<> *object_node, const char *attrName){
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
}

string iLIDS_Reader::read_iLIDS_other(xml_node<> *object_node, const char *attrName){
	for(xml_node<> *attr_node = object_node->first_node("attribute"); attr_node; attr_node = attr_node->next_sibling()){
		if (strcmp(attr_node->first_attribute("name")->value(), attrName) == 0)
		{
			if (strcmp(attrName, "BAG") == 0)
			{
				xml_node<> *bvalue = attr_node->first_node("data:bvalue");
				return bvalue->first_attribute("value")->value();
			}else{
				xml_node<> *svalue = attr_node->first_node("data:svalue");
				return svalue->first_attribute("value")->value();
			}			
		}
	}
}

// read framespan and id 
string iLIDS_Reader::read_iLIDS_fs_id(xml_node<> *object_node, const char *attrNames){
	return object_node->first_attribute(attrNames)->value();
}