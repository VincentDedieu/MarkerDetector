#ifndef MD_DETECTION_H
#define MD_DETECTION_H

#include <map>
#include <iostream>

#include "opencv2/core.hpp"

namespace md
{

struct MarkerPos
{
    cv::Point center;
    cv::Size size;
    double angleRad;
};

std::ostream& operator<<(std::ostream &os, const MarkerPos& pos);

bool isBetterScore(int method, double newScore, double prevScore);


cv::Point detectMarker(const cv::Mat& image, const cv::Mat& templ, cv::Mat& result, double &score, int method);

MarkerPos detectRotatedAndScaledMarker(const cv::Mat& image, const std::map<std::string,std::pair<cv::Mat,cv::Mat>>& pairs, cv::Mat& result, double &score, std::string &foundMarker, int method);

MarkerPos findBestMarker(const cv::Mat& image, const std::map<std::string,std::pair<cv::Mat,cv::Mat>>& pairs, cv::Mat& result, double &score, std::string &foundMarker, int method);

}

#endif
