
#include <chrono>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <sources/inc/detection.h>
#include <sources/inc/utils.h>
#include <sources/inc/apphelper.h>

using namespace std;
using namespace cv;
using namespace md;


int main( int argc, char** argv )
{
    if (argc < 2)
    {
        cout << "Not enough parameters" << endl;
        cout << "Usage:\n" << argv[0] << " <config>" << endl;
        return -1;
    }

    Mat frame;
    VideoCapture cap;
    int deviceID = 0;         // 0 = open default camera
    int apiID = CAP_ANY;      // 0 = autodetect default API
    cap.open(deviceID, apiID);
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    double score = NAN;
    string foundMarker;
    Mat result;
    map<string,pair<Mat,Mat>> pairs = loadPairs(argv[1]);

    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;
    for (;;)
    {
        cap.read(frame);

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        MarkerPos pos = findBestMarker(frame, pairs, result, score, foundMarker, TM_CCORR_NORMED);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        cout<<"Found marker:"<<foundMarker<<" score:"<<score<<" "<<pos<<" in "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()<<"ms"<<endl;

        const auto& it = pairs.find(foundMarker);
        if(it!=pairs.end())
        {
            const Mat& assetImage = it->second.second;
            drawImage(assetImage, frame, pos.center, pos.size, pos.angleRad);
        }

        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        imshow("Live", frame);
        if (waitKey(5) >= 0)
            break;
    }
    return 0;
}
