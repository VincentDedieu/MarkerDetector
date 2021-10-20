#include <sources/inc/apphelper.h>

#include <iostream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>

using namespace std;
using namespace cv;

namespace md
{

map<string,pair<Mat,Mat>> loadPairs(const string& configPath, int markerSize)
{
    map<string,pair<Mat,Mat>> pairs;

    string dirPath = configPath.substr(0, configPath.find_last_of("/\\"));

    QFile file(configPath.c_str());
    if( file.open( QIODevice::ReadOnly ) )
    {
        QByteArray bytes = file.readAll();
        file.close();

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson( bytes, &jsonError );
        if( jsonError.error != QJsonParseError::NoError )
        {
            cerr << "fromJson failed: " << jsonError.errorString().toStdString() << endl;
            return pairs;
        }
        if( document.isObject() )
        {
            QJsonObject jsonObj = document.object();
            QJsonArray jsonPairs = jsonObj.value("pairs").toArray();
            for(QJsonValue jsonValue: jsonPairs)
            {
                QJsonObject pairObj = jsonValue.toObject();
                string name = pairObj.value("name").toString().toStdString();
                string markerPath = dirPath+"/"+pairObj.value("markerPath").toString().toStdString();
                string assetPath = dirPath+"/"+pairObj.value("assetPath").toString().toStdString();
                Mat marker = imread(markerPath, IMREAD_GRAYSCALE);
                if(marker.empty())
                {
                    cerr<<markerPath<<" Error, marker not read!"<<endl;
                    continue;
                }
                resize(marker, marker, Size(markerSize,markerSize), INTER_LINEAR);
                Mat asset = imread(assetPath, IMREAD_COLOR);
                if(asset.empty())
                {
                    cerr<<markerPath<<" Error, marker not read!"<<endl;
                    continue;
                }
                pairs[name] = make_pair(marker,asset);
            }
        }
     }
    return pairs;
}

}

