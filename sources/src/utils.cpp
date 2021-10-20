#include <sources/inc/utils.h>

#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

namespace md
{

Point2d centerOf(const Rect &rect)
{
    return Point2d(rect.tl()+rect.br())/2.0;
}

Point2d centerOf(const Mat &image)
{
    return Point2d((image.cols-1)/2.0,(image.rows-1)/2.0);
}

Point2d centerOf(const Size &size)
{
    return Point2d((size.width-1)/2.0,(size.height-1)/2.0);
}

Size sizeOf(const Mat &image)
{
    return Size(image.cols, image.rows);
}

Point toPoint(const Size& size)
{
    return Point(size.width, size.height);
}

Size toSize(const Point& point)
{
    return Size(point.x, point.y);
}

Size min(const Size& s1, const Size& s2)
{
    return Size(std::min(s1.width, s2.width), std::min(s1.height, s2.height));
}

Point2d rotatePoint(const Point2d& p, double rad)
{
    const double x = std::cos(rad) * p.x - std::sin(rad) * p.y;
    const double y = std::sin(rad) * p.x + std::cos(rad) * p.y;

    const Point2d rot_p(x, y);
    return rot_p;
}

Point2d rotatePoint(const Point2d& cen_pt, const Point2d& p, double rad)
{
    const Point2d trans_pt = p - cen_pt;
    const Point2d rot_pt   = rotatePoint(trans_pt, rad);
    const Point2d fin_pt   = rot_pt + cen_pt;

    return fin_pt;
}

void drawRotatedRectangle(Mat& image, const Point &centerPoint, const Size &rectangleSize, double rotationDegrees)
{
    RotatedRect rRect = RotatedRect(centerPoint, rectangleSize, static_cast<float>(rotationDegrees));
    Point2f vertices[4];
    rRect.points(vertices);
    for (int i = 0; i < 4; i++)
        line(image, vertices[i], vertices[(i+1)%4], Scalar(0,255,0), 2);
}

void drawImage(const Mat& imageToDraw, Mat& image, const Point& pos)
{
    Size copySizeMax = sizeOf(image) - toSize(pos);
    Size roiSize = min(copySizeMax, imageToDraw.size());
    Rect roi( pos, roiSize );
    imageToDraw(Rect(Point(0,0),roiSize)).copyTo(image(roi));
}

void drawImage(const Mat& imageToDraw, Mat& image, const Point& center, const Size &size, double angleRad)
{
    static const double ToBigLength = 2 * std::sin(45*DegToRad);
    Mat localImageToDraw;
    resize(imageToDraw, localImageToDraw, size, 2, 3, INTER_LINEAR);

    Size imageSize = sizeOf(image);
    Point2d imageCenter = centerOf(image);

    double bigLength = std::max(imageSize.width, imageSize.height);
    bigLength = bigLength * ToBigLength;
    Size2d bigSize(bigLength, bigLength);
    Point2d translation = centerOf(bigSize)-imageCenter;

    Matx23d rotation = getRotationMatrix2D( imageCenter, angleRad*RadToDeg, 1.0 );
    rotation(0,2) += translation.x;
    rotation(1,2) += translation.y;

    warpAffine(image, image, rotation, bigSize);

    Point2d drawingCenter = rotatePoint(imageCenter, center, -angleRad);
    drawingCenter += translation;
    drawingCenter -= centerOf(size);

    drawImage(localImageToDraw, image, drawingCenter);

    Matx33d invTransfrom {
                rotation(0,0), rotation(0,1), rotation(0,2),
                rotation(1,0), rotation(1,1), rotation(1,2),
                0, 0, 1,
    };
    invTransfrom = invTransfrom.inv();
    warpAffine(image, image, invTransfrom.get_minor<2,3>(0,0), imageSize);
}

}

