//
//  helper.cpp
//  seed
//
//  Created by Binlong Li on 2/20/13.
//  Copyright (c) 2013 Binlong Li. All rights reserved.
//

#include "util.h"

// -----------------------------------------------------
// ---------------     Video System      ---------------
// -----------------------------------------------------
VideoUtil::VideoUtil(std::string const _videoName):videoName(_videoName), prefix("/Users/herbert19lee/Desktop/Rec/img_"), ext(".jpg"), numberOfDigits(5), startIndex(0), endIndex(-1){
    if (!boost::filesystem::exists(videoName)) {
        std::cerr << videoName << " is not Exist;" << std::endl;
        throw myexception(videoName.append(" is not Exist;"));
    }
    
    cap.open(videoName);
    if (!cap.isOpened()) {
        std::cerr << videoName << " could not be open" << std::endl;
        throw myexception(videoName.append(" could not be open"));
    }
}


bool VideoUtil::showVideo(){
    cv::namedWindow("testVideo");
    
    while (true) {
        cap >> image;
        cv::imshow("testVideo", image);
        if (cv::waitKey(30) >= 0) {
            break;
        }
    }
    cap.release();
    return true;
}

// save sample frames
// if endIndex == -1 we will save all the frames
bool VideoUtil::saveSampleFrames(){
    cv::namedWindow("saveSampleFrames");
    
    int count = startIndex;
    if (endIndex == -1){
        while (true) {
            cap >> image;
            cv::imshow("saveSampleFrames", image);
            writeFrameCore(image, count, prefix, ext, numberOfDigits);
            if (cv::waitKey(5) >= 0) { // This will make the saving stop as user's wish
                break;
            }
        }
    }else{
        while (count < endIndex) {
            cap >> image;
            cv::imshow("saveSampleFrames", image);
            writeFrameCore(image, count, prefix, ext, numberOfDigits);
        }
    }
    cap.release();
    return true;
}

// support function for saveSampleFrames function
void writeFrameCore(cv::Mat const &frame, int &count, std::string const &prefix, std::string const &ext, int const numberOfDigits){
    std::stringstream ss;
    ss << prefix << std::setfill('0') << std::setw(numberOfDigits) << count++ << ext;
    cv::imwrite(ss.str(), frame);
}

// -----------------------------------------------------
// ---------------     Image System      ---------------
// -----------------------------------------------------
void ImageUtil::resize(cv::Mat &dst, const cv::Mat &src, double scale){
    cv::resize(src, dst, cv::Size(), scale, scale);
}
// combine two arbitrary size images into one big image side by side
void ImageUtil::combineTwoImages(cv::Mat &dst, const cv::Mat img1, const cv::Mat img2)
{
    int rows1 = img1.rows;
    int cols1 = img1.cols;
    int rows2 = img2.rows;
    int cols2 = img2.cols;
    dst= cvCreateMat(rows1 > rows2 ? rows1 : rows2, cols1 + cols2, img1.type());
    cv::Mat tmp = dst(cv::Rect(0, 0, cols1, rows1));
    img1.copyTo(tmp);
    tmp = dst(cv::Rect(cols1, 0, cols2, rows2));
    img2.copyTo(tmp);
}

// -----------------------------------------------------
// ---------------   Numeric System      ---------------
// -----------------------------------------------------
//template <size_t T>
void NumericUtil::mat2bitset(const cv::Mat &desc, std::vector<boost::dynamic_bitset<> > &vb, const int nByte){
    vb.clear();
    int nbit = nByte * 8;
    int nFeatures = desc.rows;
    for (int i = 0; i < nFeatures; i++) {
        boost::dynamic_bitset<> b(nbit, 0);
        uchar *ptr = desc.data + desc.step * i;
        for (int x = 0; x < nByte; x++) {
            int mov = (nByte - 1 - x) * 8;
            boost::dynamic_bitset<> tmpBit(nbit, ptr[x]);
            b |= (tmpBit << mov);
        }
        vb.push_back(b);
    }
}

//template <size_t size>
void NumericUtil::countBitset(std::vector<boost::dynamic_bitset<> > const &vb, std::vector<size_t> &vc){
    vc.clear();
    size_t s = vb.size();
    for (int i = 0; i < s; i++) {
        vc.push_back(int(vb[i].count()));
    }
}

cv::Point2f NumericUtil::middlePoint(cv::Point2f const &p1, cv::Point2f const &p2){
    return cv::Point2f((p1.x + p2.x)/2, (p1.y + p2.y)/2);
}

double NumericUtil::distance(const cv::Point2f &p1, const cv::Point2f &p2){
    float dx = fabsf(p1.x - p2.x);
    float dy = fabsf(p1.y - p2.y);
    return sqrtf(powf(dx, 2.0) + powf(dy, 2.0));
}

// --------------- typedefine section ---------------
typedef std::pair<double, size_t> dataIndexPair;

bool comparator_dataIndexPair( const dataIndexPair& l, const dataIndexPair& r){
    return l.first < r.first;
}

std::vector<size_t> NumericUtil::sort_index(const std::vector<double> &vec, std::vector<double> &sorted){
    size_t s = vec.size();
    std::vector<dataIndexPair> vGeneral(s);
    for(int i = 0; i < s; i++){
        vGeneral[i] = std::make_pair(vec[i], i);
    }
    std::sort(vGeneral.begin(), vGeneral.end(), comparator_dataIndexPair);
    
    std::vector<size_t> idx(s);
    sorted.clear();
    sorted.resize(s);
    
    for(int i = 0; i < s; i++){
        dataIndexPair tmp = vGeneral[i];
        sorted[i] = tmp.first;
        idx[i] = tmp.second;
    }
    
    return idx;
}
// -----------------------------------------------------
// ---------------     Basic System      ---------------
// -----------------------------------------------------
bool BasicUtil::matIsEqual(const cv::Mat mat1, const cv::Mat mat2){
    // treat two empty mat as identical as well
    if (mat1.empty() && mat2.empty()) {
        return true;
    }
    // if dimensionality of two mat is not identical, these two mat is not identical
    if (mat1.cols != mat2.cols || mat1.rows != mat2.rows || mat1.dims != mat2.dims) {
        return false;
    }
    cv::Mat diff;
    cv::compare(mat1, mat2, diff, cv::CMP_NE);
    int nz = cv::countNonZero(diff);
    return nz==0;
}

// -----------------------------------------------------
// ---------------     Feature System    ---------------
// -----------------------------------------------------
// Testing case is in seeder::obtainBrief
void FeatureUtil::keptKPId(const std::vector<cv::KeyPoint> &cKP, const std::vector<cv::KeyPoint> &oKP, std::vector<int> &index){
    // computed Keypoint size should be less than origin Keypoint size
    size_t cs = cKP.size();    // cs: compute size
    size_t os = oKP.size();    // os: origin  size
    if (cs > os) {
        throw myexception("computed KeyPoint vector size larger than original KeyPoint vector size");
    }
    
    index.clear();
    
    int k = 0;
    for (int i = 0; i < cs; i++) {
        cv::Point2f query(cKP[i].pt);
        while (k < os && (query.x != oKP[k].pt.x || query.y != oKP[k].pt.y)) {
            k++;
        }
        index.push_back(k++);
    }
    
    if (cs != index.size()) {
        throw myexception("index vector does not enjoy the same size as computed KeyPoints");
    }
}

// -----------------------------------------------------
// ---------------      File System      ---------------
// -----------------------------------------------------
std::string fullfile(int num, ...){
    va_list arguments;
    va_start(arguments, num);
    
    boost::filesystem::path p;
    for (int x = 0; x < num; x++) {
        p /= va_arg(arguments, char*);
    }
    va_end(arguments);
    
    return p.make_preferred().native();
}

std::string to_regex_copy(std::string const &mask){
    std::string rv = boost::regex_replace( boost::regex_replace( boost::regex_replace( mask, boost::regex( "\\." ), "\\\\." ), boost::regex( "\\?" ), "\\." ), boost::regex( "\\*" ), "\\.*" ) ;
    return rv;
}

std::vector<std::string> ls(const std::string pcszMask){
    namespace fs = boost::filesystem;
    fs::path path;
    boost::regex mask;
    if (fs::is_directory(pcszMask)) {
        path = pcszMask;
        mask = ".*";
    }else{
        path = fs::path(pcszMask).remove_filename();
        mask = to_regex_copy(fs::path(pcszMask).filename().string() );
    }
    
    typedef std::vector<fs::path> vec;
    vec v;
    std::vector<std::string> rv;
    
    std::copy(fs::directory_iterator(path), fs::directory_iterator(), std::back_inserter(v));
    
    for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; ++it) {
        if (boost::regex_match(it->filename().string(), mask)) {
            rv.push_back(it->filename().string());
        }
    }
    return rv;
}

std::vector<std::string> folderList(const std::string &inPath){
    namespace fs = boost::filesystem;
    fs::path path(inPath);
    std::vector<std::string> folderNames;
    if (fs::is_directory(path)) {
        typedef std::vector<fs::path> vec;
        vec v;
        std::copy(fs::directory_iterator(path), fs::directory_iterator(), std::back_inserter(v));
        for (vec::const_iterator it(v.begin()), it_end(v.end()); it != it_end; it++) {
            if (fs::is_directory(*it)) {
                folderNames.push_back(it->filename().string());
            }
        }
    }
    return folderNames;
}

bool checkFolder(std::string const &inPath){
    namespace fs = boost::filesystem;
    fs::path path(inPath);
    if (!fs::exists(path)) {
        fs::create_directories(path);
    }
    return true;
}

// -----------------------------------------------------
// ---------------      Armadillo System      ---------------
// -----------------------------------------------------
arma::uvec allIndex(arma::uword s){
    arma::uvec index;
    index.resize((arma::uword)s);
    for(int i = 0; i < s; i++){
        index[i] = i;
    }
    return index;
}

arma::uvec everyNIndex(arma::uword start, arma::uword end, arma::uword step){
    std::vector<arma::uword> container;
    for (arma::uword i = start; i < end; i = i + step) {
        container.push_back(i);
    }
    arma::uvec index(container);    
    return index;
    
}