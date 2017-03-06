#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <map>

using namespace cv;
using namespace std;

struct horizon{
    int high;
    int rotation;
    Point a,b,c,d;
    int* hist;
};

struct notables{
    Point a,b,c,d;
};

horizon calcHorizon(Mat src, int left, int right, int up, int down);
float angleBetween(const float v1ax,const float v1ay,const float v2ax,const float v2ay,const float v3ax,const float v3ay);
int getHoriz(int center,const float v1ax,const float v2ax,const float v1ay,const float v2ay);
notables getNotablePoints(int *hist,int size, int *maxB, float horizA, float horizB, int noiseGap, int left, int right);
Mat rotate(Mat src, double angle);
map<int,int> getColumns(vector<vector<Point>> contours);
int getLeftColumn(map<int,int> hist, int middle);
int getRightColumn(map<int,int> hist, int middle);
string type2str(int type);

#endif // IMAGEUTILS_H
