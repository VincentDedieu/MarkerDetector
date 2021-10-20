#ifndef MD_APPHELPER_H
#define MD_APPHELPER_H

#include <map>

#include "opencv2/core.hpp"

namespace md
{

std::map<std::string,std::pair<cv::Mat,cv::Mat>> loadPairs(const std::string& configPath, int markerSize=24);

}

#endif

