#ifndef MD_UTILS_H
#define MD_UTILS_H

#include "opencv2/core.hpp"

namespace md
{

static const double Pi = 3.14159265358979323846;
static const double DegToRad = Pi/180;
static const double RadToDeg = 1/DegToRad;

cv::Point2d centerOf(const cv::Rect &rect);
cv::Point2d centerOf(const cv::Mat &image);
cv::Point2d centerOf(const cv::Size &size);

cv::Size sizeOf(const cv::Mat &image);

cv::Point toPoint(const cv::Size& size);
cv::Size toSize(const cv::Point& point);

cv::Size min(const cv::Size& s1, const cv::Size& s2);

cv::Point2d rotatePoint(const cv::Point2d& p, double rad);
cv::Point2d rotatePoint(const cv::Point2d& cen_pt, const cv::Point2d& p, double rad);

void drawRotatedRectangle(cv::Mat& image, const cv::Point &centerPoint, const cv::Size &rectangleSize, double rotationDegrees);
void drawImage(const cv::Mat& imageToDraw, cv::Mat& image, const cv::Point& pos);
void drawImage(const cv::Mat& imageToDraw, cv::Mat& image, const cv::Point& center, const cv::Size &size, double angleRad);

}

#endif

