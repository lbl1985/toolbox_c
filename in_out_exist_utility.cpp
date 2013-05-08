// canny edge operation on input image
#include "stdafx.h"
#include "in_out_exist_utility.h"
void canny(cv::Mat& img, cv::Mat& out){
    // Convert to gray

    if(img.channels() == 3)
        cv::cvtColor(img, out, CV_BGR2GRAY);
    // Compute Canny edges
    cv::Canny(out, out, 100, 200);
    // Invert the image
    cv::threshold(out, out, 128, 255, cv::THRESH_BINARY_INV);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// make the border of the the image as a special color
void makeSpecialColorBorder(cv::Mat& output, int borderWidth, cv::Scalar color){
    // top bar
    for (int i = 0; i < output.cols; i++){
        for(int j = 0; j < borderWidth; j++){
            assignValueOnImage(output, j, i, color);
        }
    }
    // left bar
    for(int i = 0; i < borderWidth; i++){
        for(int j = 0; j < output.rows; j++){
            assignValueOnImage(output, j, i, color);
        }
    }
    // bottom bar
    for(int i = 0; i < output.cols; i++){
        for(int j = output.rows - borderWidth; j < output.rows; j ++){
            assignValueOnImage(output, j, i, color);
        }
    }
    // right bar
    for(int i = output.cols - borderWidth; i < output.cols; i++){
        for(int j = 0; j < output.rows; j++){
            assignValueOnImage(output, j, i, color);
        }
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// assign color/255 to a specific location on color/gray image
void assignValueOnImage(cv::Mat& image, int row, int col, cv::Scalar color){
    if(image.channels() == 1){
        image.at<uchar>(row, col) = 255;
    }else if(image.channels() == 3){
        image.at<cv::Vec3b>(row, col)[0] = color[0];
        image.at<cv::Vec3b>(row, col)[1] = color[1];
        image.at<cv::Vec3b>(row, col)[2] = color[2];
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// only draw one Trajectory
void drawTrajectory(std::vector<cv::Point2f> points, cv::Mat& img, cv::Scalar color){	
	if(points.size() > 0){
		std::vector<cv::Point2f>::iterator it = points.begin();
		for(int i = 0; i < points.size() - 1; i++){
			cv::Point2f point0 = *it;
			cv::Point2f point1 = *(++it);
			cv::line(img, point0, point1, color, 2);
		}
		cv::circle(img, *it, 5, color, 3);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// read the directories files names
void readDirectory( const std::string& directoryName, std::vector<std::string>& filenames, bool addDirectoryName )
{
    filenames.clear();
    
#ifdef WIN32
    struct _finddata_t s_file;
    std::string str = directoryName + "\\*.*";
    
	intptr_t h_file = _findfirst( str.c_str(), &s_file );
	if( h_file != static_cast<intptr_t>(-1.0) )
    {
        do
        {
            if( addDirectoryName )
                filenames.push_back(directoryName + "\\" + s_file.name);
            else
                filenames.push_back((std::string)s_file.name);
        }
        while( _findnext( h_file, &s_file ) == 0 );
    }
    _findclose( h_file );
#else
    DIR* dir = opendir( directoryName.c_str() );
    if( dir != NULL )
    {
        struct dirent* dent;
        while( (dent = readdir(dir)) != NULL )
        {
            if( addDirectoryName )
                filenames.push_back( directoryName + "/" + string(dent->d_name) );
            else
                filenames.push_back( string(dent->d_name) );
        }
    }
#endif
    
    sort( filenames.begin(), filenames.end() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculate foreground percent according to a bkgd binary images
double calForePercent(const cv::Mat& output){
    cv::Scalar forePixelSumPerChannel = cv::sum(output);
    double allPixels = output.cols * output.rows * output.channels();
    double foreGroundPixel = 0;
    for(int i = 0; i < output.channels(); i++){
        //        cout << "Channel " << i << "percent " << "percent " << forePixelSumPerChannel[i] << endl;
        foreGroundPixel += forePixelSumPerChannel[i];
    }
    return foreGroundPixel / allPixels;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// calculate foreground percent according to a bkgd binary images
double calForePercent(const cv::Mat& output, cv::Rect_<double> & rect)
{
	cv::Rect_<double> r = rect;
	if (r.x < 0.0)
		r.x = 0.0;
	if (r.y < 0.0)
		r.y = 0.0;
	if (r.x+r.width > output.cols)
		r.width = output.cols - r.x;
	if (r.y+r.height > output.rows)
		r.height = output.rows - r.y;

	return calForePercent(output(r));

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//compare covariance matricies
float compareCov(cv::Mat a, cv::Mat b)
{
	cv::Mat eigen_values;
	cv::eigen(a.inv() * b, eigen_values);
	cv::Mat ln;
	cv::Mat ln2;
	cv::log(eigen_values, ln);
	cv::multiply(ln,ln,ln2);
	cv::Scalar temp = cv::sum(ln2);
	return temp[0];
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
//calculate hog features
//based on code from http://www.mathworks.com/matlabcentral/fileexchange/33863
cv::Mat HOGfeatures(cv::Mat & pixels, int nb_bins, double cheight, double cwidth, int block_rows, int block_cols, bool orient, double clip_val)
{
    //const double PI = 3.1415926536;
	bool grayscale;

	//img must be uchar
	if (pixels.type() == CV_8UC1)
	{
		grayscale = 1;
	}
	else if (pixels.type() == CV_8UC3)
	{
		grayscale = 0;
	}
	else
	{
		std::cerr << "Image is not uchar, cannot calculate HOG!!!" << std::endl;
		return cv::Mat();
	}

	/*if (!pixels.isContinuous())
	{
		cv::Mat temp = pixels.clone();
		pixels = temp;
		if (!pixels.isContinuous())
		{
			std::cerr << "Memory error, cannot allocate a continuous array for HOG calc!" << std::endl;
			return cv::Mat();
		}
	}
	unsigned char * pixels = img.data;*/
	
	int img_width  = pixels.cols;
	int img_height = pixels.rows;

    int hist1= 2+ceil(-0.5 + img_height/cheight);
    int hist2= 2+ceil(-0.5 + img_width/cwidth);

	double bin_size = (1+(orient==1))*PI/nb_bins;

	cv::Mat features((hist1-2-(block_rows-1))*(hist2-2-(block_cols-1))*nb_bins*block_rows*block_cols, 1, CV_32FC1);

    float dx[3], dy[3], grad_or, grad_mag, temp_mag;
    float Xc, Yc, Oc, block_norm;
    int x1, x2, y1, y2, bin1, bin2;
    int des_indx = 0;
    
    std::vector<std::vector<std::vector<double> > > h(hist1, std::vector<std::vector<double> > (hist2, std::vector<double> (nb_bins, 0.0) ) );    
    std::vector<std::vector<std::vector<double> > > block(block_rows, std::vector<std::vector<double> > (block_cols, std::vector<double> (nb_bins, 0.0) ) );

	std::vector<cv::Mat> pixel_vec;

	if (!grayscale)
	{
		cv::split(pixels, pixel_vec);
	}

	//Calculate gradients (zero padding)
    

    for(unsigned int y=0; y<img_height; y++) 
	{
        for(unsigned int x=0; x<img_width; x++) 
		{
            if (grayscale == 1)
			{
                if (x==0) 
				{
					dx[0] = pixels.at<unsigned char>(y , x+1);
				}
                else
				{
                    if (x==img_width-1) 
					{
						dx[0] = -pixels.at<unsigned char>(y , x-1);
					}
					else
					{
						dx[0] = pixels.at<unsigned char>(y, x+1) - pixels.at<unsigned char>(y , x-1);
					}
                }
                if (y==0)
				{
					dy[0] = -pixels.at<unsigned char>(y+1, x);
				}
				else{
                    if (y==img_height-1) 
					{
						dy[0] = pixels.at<unsigned char>(y-1, x);
					}
					else
					{
						dy[0] = -pixels.at<unsigned char>(y+1, x) + pixels.at<unsigned char>(y-1, x);
					}
                }
            }
            else
			{
                if(x==0)
				{
                    dx[0] = pixel_vec[0].at<unsigned char>(y , x+1);
                    dx[1] = pixel_vec[1].at<unsigned char>(y , x+1);
                    dx[2] = pixel_vec[2].at<unsigned char>(y , x+1);                    
                }
                else
				{
                    if (x==img_width-1)
					{
                        dx[0] = -pixel_vec[0].at<unsigned char>(y , x-1);                        
                        dx[1] = -pixel_vec[1].at<unsigned char>(y , x-1);
                        dx[2] = -pixel_vec[2].at<unsigned char>(y , x-1);
                    }
                    else
					{
                        dx[0] = pixel_vec[0].at<unsigned char>(y, x+1) - pixel_vec[0].at<unsigned char>(y , x-1);
                        dx[1] = pixel_vec[1].at<unsigned char>(y, x+1) - pixel_vec[1].at<unsigned char>(y , x-1);
                        dx[2] = pixel_vec[2].at<unsigned char>(y, x+1) - pixel_vec[2].at<unsigned char>(y , x-1);
                        
                    }
                }
                if(y==0)
				{
                    dy[0] = -pixel_vec[0].at<unsigned char>(y+1, x);
                    dy[1] = -pixel_vec[1].at<unsigned char>(y+1, x);
                    dy[2] = -pixel_vec[2].at<unsigned char>(y+1, x);
                }
                else
				{
                    if (y==img_height-1)
					{
                        dy[0] = pixel_vec[0].at<unsigned char>(y-1, x);
                        dy[1] = pixel_vec[1].at<unsigned char>(y-1, x);
                        dy[2] = pixel_vec[2].at<unsigned char>(y-1, x);
                    }
                    else
					{
                        dy[0] = -pixel_vec[0].at<unsigned char>(y+1, x) + pixel_vec[0].at<unsigned char>(y-1, x);
                        dy[1] = -pixel_vec[1].at<unsigned char>(y+1, x) + pixel_vec[1].at<unsigned char>(y-1, x);
                        dy[2] = -pixel_vec[2].at<unsigned char>(y+1, x) + pixel_vec[2].at<unsigned char>(y-1, x);
                    }
                }
            }
            
            grad_mag = sqrt(dx[0]*dx[0] + dy[0]*dy[0]);
            grad_or= atan2(dy[0], dx[0]);
            
            if (grayscale == 0)
			{
                temp_mag = grad_mag;
                for (unsigned int cli=1;cli<3;++cli)
				{
                    temp_mag= sqrt(dx[cli]*dx[cli] + dy[cli]*dy[cli]);
                    if (temp_mag>grad_mag)
					{
                        grad_mag=temp_mag;
                        grad_or= atan2(dy[cli], dx[cli]);
                    }
                }
            }
            
            if (grad_or<0) grad_or+=PI + (orient==1) * PI;

            // trilinear interpolation
            
            bin1 = (int)floor(0.5 + grad_or/bin_size) - 1;
            bin2 = bin1 + 1;
            x1   = (int)floor(0.5+ x/cwidth);
            x2   = x1+1;
            y1   = (int)floor(0.5+ y/cheight);
            y2   = y1 + 1;
            
            Xc = (x1+1-1.5)*cwidth + 0.5;
            Yc = (y1+1-1.5)*cheight + 0.5;
            
            Oc = (bin1+1+1-1.5)*bin_size;
            
            if (bin2==nb_bins){
                bin2=0;
            }
            if (bin1<0){
                bin1=nb_bins-1;
            }            
           
            h[y1][x1][bin1]= h[y1][x1][bin1] + grad_mag*(1-((x+1-Xc)/cwidth))*(1-((y+1-Yc)/cheight))*(1-((grad_or-Oc)/bin_size));
            h[y1][x1][bin2]= h[y1][x1][bin2] + grad_mag*(1-((x+1-Xc)/cwidth))*(1-((y+1-Yc)/cheight))*(((grad_or-Oc)/bin_size));
            h[y2][x1][bin1]= h[y2][x1][bin1] + grad_mag*(1-((x+1-Xc)/cwidth))*(((y+1-Yc)/cheight))*(1-((grad_or-Oc)/bin_size));
            h[y2][x1][bin2]= h[y2][x1][bin2] + grad_mag*(1-((x+1-Xc)/cwidth))*(((y+1-Yc)/cheight))*(((grad_or-Oc)/bin_size));
            h[y1][x2][bin1]= h[y1][x2][bin1] + grad_mag*(((x+1-Xc)/cwidth))*(1-((y+1-Yc)/cheight))*(1-((grad_or-Oc)/bin_size));
            h[y1][x2][bin2]= h[y1][x2][bin2] + grad_mag*(((x+1-Xc)/cwidth))*(1-((y+1-Yc)/cheight))*(((grad_or-Oc)/bin_size));
            h[y2][x2][bin1]= h[y2][x2][bin1] + grad_mag*(((x+1-Xc)/cwidth))*(((y+1-Yc)/cheight))*(1-((grad_or-Oc)/bin_size));
            h[y2][x2][bin2]= h[y2][x2][bin2] + grad_mag*(((x+1-Xc)/cwidth))*(((y+1-Yc)/cheight))*(((grad_or-Oc)/bin_size));
        }
    }

	//Block normalization

    for(unsigned int x=1; x<hist2-block_cols; x++)
	{
        for (unsigned int y=1; y<hist1-block_rows; y++)
		{
            
            block_norm=0;
            for (unsigned int i=0; i<block_rows; i++)
			{
                for(unsigned int j=0; j<block_cols; j++)
				{
                    for(unsigned int k=0; k<nb_bins; k++)
					{
                        block_norm+=h[y+i][x+j][k]*h[y+i][x+j][k];
                    }
                }
            }
            
            block_norm=sqrt(block_norm);
            for (unsigned int i=0; i<block_rows; i++)
			{
                for(unsigned int j=0; j<block_cols; j++)
				{
                    for(unsigned int k=0; k<nb_bins; k++)
					{
                        if (block_norm>0)
						{
                            block[i][j][k]=h[y+i][x+j][k]/block_norm;
                            if (block[i][j][k]>clip_val) block[i][j][k]=clip_val;
                        }
                    }
                }
            }
            
            block_norm=0;
            for (unsigned int i=0; i<block_rows; i++)
			{
                for(unsigned int j=0; j<block_cols; j++)
				{
                    for(unsigned int k=0; k<nb_bins; k++)
					{
                        block_norm+=block[i][j][k]*block[i][j][k];
                    }
                }
            }
            
            block_norm=sqrt(block_norm);
            for (unsigned int i=0; i<block_rows; i++)
			{
                for(unsigned int j=0; j<block_cols; j++)
				{
                    for(unsigned int k=0; k<nb_bins; k++)
					{
                        if (block_norm>0) 
						{
							features.at<float>(des_indx,0) = block[i][j][k]/block_norm;
						}
                        else 
						{
							features.at<float>(des_indx,0) = 0.0;
						}
						des_indx++;
                    }
                }
            }
        }
    }

	return features;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//construct hankel matrix
cv::Mat constructHankel(std::vector<cv::Mat> & state, cv::Range rowrange, cv::Range colrange)
{
	if (state.size() % 2 == 0)
	{
		std::cout << "ERROR: state size not odd, in funtion constructHankel:" << state.size() << std::endl;
	}

	int N = state.size() % 2 ? (state.size() + 1)/2 : state.size()/2;
	int nr = rowrange.size();
	int nc = colrange.size();
	cv::Mat hankel(N*nr, N*nc, CV_32F);

	for (int r = 0; r < N; r++)
	{
		for (int c = 0; c < N; c++)
		{
			for (int i = 0; i < nr; i++)
			{
				for (int j = 0; j < nc; j++)
				{
					//hankel.at<float>(r*nr+i,c*nc+j) = state[r+c].at<float>(rowrange.start+i, colrange.start+j);
					hankel.at<float>(i*N+r,j*N+c) = state[r+c].at<float>(rowrange.start+i, colrange.start+j);
				}
			}
		}
	}

	return hankel;
}


// Return a string with current time info;
std::string currentTime(){
    time_t rawtime;
	struct tm * timeinfo;
	std::ostringstream convert;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	std::string s_timeInfo;
	// Former way to generate time string
	//s_timeInfo = asctime(timeinfo);
	convert << timeinfo->tm_year + 1900 << "."
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_mon + 1 << "."
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_mday << "+"
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_hour << ":"
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_min  << ":"
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_sec  << "."
		<<std::setw(3) << std::setfill('0') << 0;
	s_timeInfo = convert.str();
    return s_timeInfo;
}

//convert time to correct format
std::string convertTime(time_t rawtime)
{
	struct tm * timeinfo;
	std::ostringstream convert;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	std::string s_timeInfo;
	// Former way to generate time string
	//s_timeInfo = asctime(timeinfo);
	convert << timeinfo->tm_year + 1900 << "."
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_mon + 1 << "."
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_mday << "+"
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_hour << ":"
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_min  << ":"
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_sec  << "."
		<<std::setw(3) << std::setfill('0') << 0;
	s_timeInfo = convert.str();
    return s_timeInfo;
}
// Return a string with today in YYYY-MM-DD format
std::string todayDate(){
	time_t rawtime;
	struct tm * timeinfo;
	std::ostringstream convert;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	std::string s_timeInfo;

	convert << timeinfo->tm_year + 1900 << "-" 
		<<std::setw(2) << std::setfill('0') << timeinfo->tm_mon + 1 << "-"
		<< std::setw(2) << std::setfill('0') << timeinfo->tm_mday;
	s_timeInfo = convert.str();
	return s_timeInfo;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//compare overlap of rectrangles
double compareOverlap(cv::Rect_<double> a, cv::Rect_<double> b)
{
	/*if (a.tl().x > b.br().x || b.tl().x > a.br().x 
		|| a.tl().y > b.br().y || b.tl().y > a.br().y)
	{
		return 0.0;
	}*/

	cv::Rect_<double> and = a & b;
	//cv::Rect_<double> or = a | b;

	double area = and.area() / (a.area() + b.area() - and.area());
	return area;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
//get_subwindow with repeated values
cv::Mat get_subwindow(cv::Rect r, const cv::Mat & img)
{
	std::vector<int> xs(r.width);
	cv::Mat out(r.height,r.width,CV_8U);

	for (int i = 0; i < r.width; i++)
	{
		xs[i] = r.x + i;
		if (xs[i] < 0)
		{
			xs[i] = 0;
		}
		else if (xs[i] >= img.cols)
		{
			xs[i] = img.cols-1;
		}
	}

	std::vector<int> ys(r.height);

	for (int i = 0; i < r.height; i++)
	{
		ys[i] = r.y + i;
		if (ys[i] < 0)
		{
			ys[i] = 0;
		}
		else if (ys[i] >= img.rows)
		{
			ys[i] = img.rows-1;
		}
	}

	for (int i = 0; i < r.width; i++)
	{
		for (int j = 0; j < r.height; j++)
		{
			out.at<unsigned char>(j,i) = img.at<unsigned char>(ys[j],xs[i]);
		}
	}
	//cv::imshow("subwindow",out.clone());
	return out.clone();
}


/*cv::Mat get_subwindow(cv::Rect r, const cv::Mat & img)
{

	cv::Mat out(r.height, r.width, img.type());
	cv::Rect imgrect = r;
	cv::Rect outrect = cv::Rect(0,0,r.width,r.height);

	if (r.tl().x < 0)
	{
		//std::cout << "low x = " << r.tl().x << std::endl;
		imgrect.x = 0;
		imgrect.width = imgrect.width + r.tl().x;
		outrect.x = -r.tl().x;
		outrect.width = imgrect.width;

		cv::Range outrr(r.tl().y < 0 ? -r.tl().y	: 0,		r.br().y > img.rows ? img.rows-r.tl().y	: r.height);
		cv::Range imgrr(r.tl().y < 0 ? 0			: r.tl().y,	r.br().y > img.rows ? img.rows				: r.br().y);
		for (int i = 0; i < -r.tl().x; i++)
		{
			img.col(0).rowRange(imgrr).copyTo(out.col(i).rowRange(outrr));
		}
	}

	if (r.tl().y < 0)
	{
		//std::cout << "low y = " << r.tl().y << std::endl;
		imgrect.y = 0;
		imgrect.height = imgrect.height + r.tl().y;
		outrect.y = -r.tl().y;
		outrect.height = imgrect.height;

		cv::Range outcr(r.tl().x < 0 ? -r.tl().x	: 0,		r.br().x > img.cols ? img.cols-r.tl().x	: r.width);
		cv::Range imgcr(r.tl().x < 0 ? 0			: r.tl().x,	r.br().x > img.cols ? img.cols				: r.br().x);
		for (int i = 0; i < -r.tl().y; i++)
		{
			img.row(0).colRange(imgcr).copyTo(out.row(i).colRange(outcr));
		}
	}

	if (r.br().x > img.cols)
	{
		//std::cout << "high x = " << r.br().x << std::endl;
		//imgrect.x = r.x;
		imgrect.width = img.cols - imgrect.tl().x;
		outrect.x = 0;
		outrect.width = imgrect.width;

		cv::Range outrr(r.tl().y < 0 ? -r.tl().y	: 0,		r.br().y > img.rows ? img.rows-r.tl().y		: r.height);
		cv::Range imgrr(r.tl().y < 0 ? 0			: r.tl().y,	r.br().y > img.rows ? img.rows				: r.br().y);
		for (int i = r.width - 1; i >= img.cols - r.tl().x; i--)
		{
			img.col(img.cols-1).rowRange(imgrr).copyTo(out.col(i).rowRange(outrr));
		}
	}
	
	if (r.br().y > img.rows)
	{
		//std::cout << "high y = " << r.br().y << std::endl;
		//imgrect.y = r.y;
		imgrect.height = img.rows - imgrect.tl().y;
		outrect.y = 0;
		outrect.height = imgrect.height;

		cv::Range outcr(r.tl().x < 0 ? -r.tl().x	: 0,		r.br().x > img.cols ? img.cols-r.tl().x	: r.width);
		cv::Range imgcr(r.tl().x < 0 ? 0			: r.tl().x,	r.br().x > img.cols ? img.cols				: r.br().x);
		for (int i = r.height - 1; i >= img.rows - r.tl().y; i--)
		{
			img.row(img.rows-1).colRange(imgcr).copyTo(out.row(i).colRange(outcr));
		}
	}

	if (r.tl().x < 0 && r.tl().y < 0)
	{
		//std::cout << "both low x = " << r.tl().x << " & y = " << r.tl().y << std::endl;
		for (int i = 0; i < outrect.x; i++)
		{
			for (int j = 0; j < outrect.y; j++)
			{
				img.col(0).row(0).copyTo(out.col(i).row(j));
			}
		}
	}

	if (r.br().x > img.cols && r.br().y > img.rows)
	{
		//std::cout << "both high x = " << r.br().x << " & y = " << r.br().y << std::endl;
		for (int i = outrect.x; i < outrect.width; i++)
		{
			for (int j = outrect.y; j < outrect.height; j++)
			{
				img.col(img.cols-1).row(img.rows-1).copyTo(out.col(i).row(j));
			}
		}
	}

	//cv::Mat roi;// = out.rowRange(outrect.y, outrect.y+outrect.height).colRange(outrect.x, outrect.x+outrect.width);
	img(imgrect).copyTo(out(outrect));
	//cv::imshow("subwindow",out.clone());
	//cv::equalizeHist(out,out);
	return out.clone();
}*/



////////////////////////////////////////////////////////////////////////////////////////////////////////
//circshift
cv::Mat circshiftrows(cv::Mat x, int n)
{
	cv::Mat y(x.rows, x.cols, x.type());

	for (int i = 0; i < x.rows; i++)
	{
		int yi = (i+n) % x.rows;
		 x.row(i).copyTo(y.row(yi));
	}
	return y.clone();
}

cv::Mat circshiftcols(cv::Mat x, int n)
{
	cv::Mat y(x.rows, x.cols, x.type());

	for (int i = 0; i < x.cols; i++)
	{
		int yi = (i+n) % x.cols;
		 x.col(i).copyTo(y.col(yi));
	}
	return y.clone();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//complex division
void complexDivision(cv::Mat & dst, cv::Mat n, cv::Mat d, cv::Scalar_<double> ds)
{
	d += ds;

	std::vector<cv::Mat> nv(2, cv::Mat());
	std::vector<cv::Mat> dv(2, cv::Mat());
	std::vector<cv::Mat> out(2, cv::Mat());

	cv::split(n,nv);
	cv::split(d,dv);

	//cv::Mat Ndv = dv[0].mul(dv[0]) + dv[1].mul(dv[1]);

	out[0] = (nv[0].mul(dv[0]) + nv[1].mul(dv[1])) / (dv[0].mul(dv[0]) + dv[1].mul(dv[1]));
	out[1] = (nv[1].mul(dv[0]) - nv[0].mul(dv[1])) / (dv[0].mul(dv[0]) + dv[1].mul(dv[1]));

	cv::merge(out, dst);
}

void complexDivision(cv::Mat & dst, cv::Mat n, cv::Mat d, cv::Scalar_<float> ds)
{
	d += ds;

	std::vector<cv::Mat> nv(2, cv::Mat());
	std::vector<cv::Mat> dv(2, cv::Mat());
	std::vector<cv::Mat> out(2, cv::Mat());

	cv::split(n,nv);
	cv::split(d,dv);

	//cv::Mat Ndv = dv[0].mul(dv[0]) + dv[1].mul(dv[1]);

	out[0] = (nv[0].mul(dv[0]) + nv[1].mul(dv[1])) / (dv[0].mul(dv[0]) + dv[1].mul(dv[1]));
	out[1] = (nv[1].mul(dv[0]) - nv[0].mul(dv[1])) / (dv[0].mul(dv[0]) + dv[1].mul(dv[1]));

	cv::merge(out, dst);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a folder with under specific address
void createFolder(std::string folderName){
#ifdef _WIN32 || _WIN64
	if(!CreateDirectoryA(folderName.c_str(), NULL)){
		//if (GetLastError() != ERROR_ALREADY_EXISTS)
		//{
		//	throw "May not have authority to write.";
		//}
	}
	//if (GetFileAttributesA(folderName.c_str()) == INVALID_FILE_ATTRIBUTES)
	//{
	//	CreateDirectoryA(folderName.c_str(), NULL);
	//	//_mkdir(folderName.c_str());
	//}
	
#else
	if (0 != mkdir(folderName.c_str(), 0744)) {
		perror("mkdir failed");
	}
#endif    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//wstring to string
std::string wstring2string(std::wstring ws)
{
	std::string s;

	for (int i = 0; i < ws.size(); i++)
	{
		char next = (char)ws[i];// && 0xff);
		s = s + next;
	}
	return s;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//safe rect
cv::Mat safeRect(const cv::Mat & img, const cv::Rect & rect, cv::Rect & safe)
{
	cv::Rect imgrect(0,0,img.cols,img.rows);
	safe = rect & imgrect;

	if (safe.width < 1 || safe.height < 1)
	{
		return cv::Mat();
	}
	else
	{
		return img(safe);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//get filename from path
std::string fileFromPath(std::string path)
{
	std::string slashes = "\\/";
	int p = path.find_last_of(slashes);

	if (p == path.size()-1)
	{
		std::cout << "ERROR: no file name, path ends in / or \\, in function fileFromPath" << std::endl;
		return std::string();
	}
	else if (p == std::string::npos)
	{
		return path;
	}
	else
	{
		return std::string(path.begin()+p,path.end());
	}
}

double computeDistance(cv::Rect_<double> const &rect1, cv::Rect_<double> const &rect2){
	cv::Point_<double> p1(rect1.tl() + cv::Point_<double>(rect1.width / 2, rect1.height / 2));
	cv::Point_<double> p2(rect2.tl() + cv::Point_<double>(rect2.width / 2, rect2.height / 2));
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));

}