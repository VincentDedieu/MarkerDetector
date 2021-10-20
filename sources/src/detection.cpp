#include <sources/inc/detection.h>

#include <unordered_set>

#include "opencv2/imgproc.hpp"

#include <sources/inc/utils.h>

using namespace std;
using namespace cv;

namespace
{
static std::unordered_set<int> minLocMatchModes = {
    TM_SQDIFF,
    TM_SQDIFF_NORMED,
};
}

namespace md
{

ostream& operator<<(ostream &os, const MarkerPos& pos)
{
    os<<"MarkerPos(center:"<<pos.center<<", size:"<<pos.size<<", angleDeg:"<<pos.angleRad*RadToDeg<<")";
    return os;
}

bool isBetterScore(int method, double newScore, double prevScore)
{
    if(isnan(prevScore))
        return true;
    if(minLocMatchModes.find(method) != minLocMatchModes.end())
    {
        if(newScore < prevScore )
            return true;
    }
    else
    {
        if(newScore > prevScore )
            return true;
    }
    return false;
}

Point detectMarker(const Mat& image, const Mat& templ, Mat& result, double &score, int method)
{
    matchTemplate( image, templ, result, method);
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );//36ms
    //cout<<"minVal:"<<minVal<<" maxVal:"<<maxVal<<endl;
    if(minLocMatchModes.find(method) != minLocMatchModes.end())
    {
        matchLoc = minLoc;
        score = minVal;
    }
    else
    {
        matchLoc = maxLoc;
        score = maxVal;
    }
    return matchLoc;
}


MarkerPos detectRotatedAndScaledMarker(const Mat& image, const map<string,pair<Mat,Mat>>& pairs, Mat& result, double &score, string &foundMarker, int method)
{
    Mat localResult;
    Mat localImage;
    Size localSize;
    Point pos;
    double localScore = 0;
    score = NAN;
    MarkerPos detectedPos;

    const auto & pairsEnd = pairs.end();
    Point2d imageCenter = centerOf(image);
    int rotationNumber = 10;
    for(int rotationIndex=0; rotationIndex<rotationNumber; ++rotationIndex)
    {
        double rotationRad = 2*Pi*rotationIndex/rotationNumber;
        if(rotationIndex==0)
        {
            localImage = image;
        }
        else
        {
            Mat rotation = getRotationMatrix2D( imageCenter, rotationRad*RadToDeg, 1.0 );
            warpAffine(image, localImage, rotation, image.size());
        }


        for(double width=256; width>=64; width/=1.1)
        {
            localSize.width = static_cast<int>(width);
            localSize.height = localSize.width * localImage.rows / localImage.cols;
            double scale = (double)localSize.width / image.cols;
            resize(localImage, localImage, localSize, INTER_LINEAR);

            for(auto pairIt = pairs.begin(); pairIt != pairsEnd; pairIt++)
            {
                const string& pairName = pairIt->first;
                const Mat& marker = pairIt->second.first;
                pos = md::detectMarker(localImage, marker, localResult, localScore, method);
                if(isBetterScore(method, localScore, score))
                {
                    foundMarker = pairName;
                    score = localScore;

                    Rect resultLoc(pos-Point(centerOf(marker)), sizeOf(marker));

                    localResult.copyTo(result);
                    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
                    rectangle( result, resultLoc.tl(), resultLoc.br(), Scalar::all(0), 2, 8, 0 );

                    detectedPos.size.width = static_cast<int>(marker.cols/scale);
                    detectedPos.size.height = static_cast<int>(marker.rows/scale);
                    detectedPos.center = rotatePoint(centerOf(localImage), Point2d(pos)+centerOf(marker), rotationRad) / scale;
                    detectedPos.angleRad = rotationRad;
                }
            }
        }
    }

    return detectedPos;
}

MarkerPos findBestMarker(const Mat& image, const map<string,pair<Mat,Mat>>& pairs, Mat& result, double &score, string &foundMarker, int method)
{
    Mat localImage;
    cvtColor(image, localImage, COLOR_RGB2GRAY);

    MarkerPos pos = detectRotatedAndScaledMarker(localImage, pairs, result, score, foundMarker, method);

    return pos;
}

}
