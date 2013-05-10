#include "stdafx.h"
#include "iLIDS_util.h"

deque<cv::Ptr<BB> > readViper(const std::string *fileName){
	xml_document<> doc;
	ifstream theFile(*fileName);

	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	xml_node<> *viper_node = doc.first_node("viper");
	xml_node<> *data_node_out = viper_node->first_node("data");
	xml_node<> *source_node = data_node_out -> first_node("sourcefile");

	deque<cv::Ptr<BB> > boundingBox;

	for (xml_node<> *object_node = source_node->first_node("object"); object_node; object_node = object_node -> next_sibling())
	{
		if (strcmp(object_node->first_attribute("name")->value(), "Target") == 0)
		{ 
			for(xml_node<> *attr_node = object_node->first_node("attribute"); attr_node; attr_node = attr_node->next_sibling()){
				if (strcmp( attr_node->first_attribute("name")->value(), "BOUNDING-BOX" ) == 0)
				{
					int count = 0;
					for (xml_node<> *databb_node = attr_node->first_node("data:bbox"); databb_node; databb_node = databb_node->next_sibling())
					{
						string framespan(databb_node->first_attribute("framespan")->value());
						string frameStr = framespan.substr(0, framespan.find(":"));
						int f(atoi(frameStr.c_str()));
						float x(atof(databb_node->first_attribute("x")->value()));
						float y(atof(databb_node->first_attribute("y")->value()));
						float w(atof(databb_node->first_attribute("width")->value()));
						float h(atof(databb_node->first_attribute("height")->value()));
						//printf("data %d: f=%d x=%4.2f y=%4.2f w=%4.2f h=%4.2f\n",count++, f, x, y, w, h);
						cv::Ptr<BB> tmpBB(new BB);
						tmpBB->fnum = f; 
						tmpBB->rect = cv::Rect_<double>(x, y, w, h);
						boundingBox.push_back(tmpBB);
						std::cout << "data " << count++ << " f=" << boundingBox.back()->fnum << " rect=" << boundingBox.back()->rect << std::endl;
					}
				}
			}
		}		
	}

	return boundingBox;
}