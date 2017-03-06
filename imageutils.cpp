#include "imageutils.h"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

# define PI           3.14159265358979323846  /* pi */
#define HORIZON_GAP  0.30 /*10%*/

using namespace std;
using namespace cv;



struct pointSorter {
    bool operator() (cv::Point pt1, cv::Point pt2)
    {   if(pt1.x == pt2.x){
            return (pt1.y < pt2.y);
        }else{
            return (pt1.x < pt2.x);
        }

    }
} pointSort;


/*
 * Calculate horizon level and rotation from black image
*/
horizon calcHorizon(Mat src, int left, int right, int up, int down)
{
    horizon hr;
    hr.high=0;
    hr.rotation=0;
    int hist[src.cols];
    int maxB[src.cols];
    vector<Point> dst;
    Vec4f ln;
    for(int i = 0; i < src.rows; i++){
        for(int j = 0; j < src.cols; j++){
            if(i==0) {
                hist[j] = 0;
                maxB[j] = 0;
            }
            int x = (int)src.at<uchar>(i,j);
            if(x==255){
                hist[j]=hist[j]+1;
                   if(i > maxB[j])maxB[j] = i;
            }
        }
    }

    for(int i = 0; i < src.cols; i++){
        if(i < left){
            dst.push_back(Point(i,maxB[i]));
        }else if(i >= right){
            dst.push_back(Point(i,maxB[i]));
        }
    }

    fitLine(dst,ln,CV_DIST_L2,0,0.01,0.01);

    //Print histogram
    Mat drawing = Mat::zeros( src.size(), CV_8UC3 );
    for(uint i = 1; i < dst.size(); i++){
        line(drawing, dst.at(i) , dst.at(i), Scalar(0, 0, 255), 2, 8, 0  );
    }

    for( int i = 1; i < src.cols; i++ )
        {
          line(drawing, Point( i,  hist[i]) , Point( i-1, hist[i-1] ), Scalar( 255, 0, 0), 2, 8, 0  );
        }


    //float angle = angleBetween(ln[2]+ln[0],ln[3]+ln[1],ln[2]+ln[0],ln[3],ln[2],ln[3]);
    //hr.rotation = angle *(180/PI);
    //hr.high = getHoriz(src.cols/2,ln[2],ln[2]+ln[0],ln[3]+ln[1],ln[3]);
    /*notables n = getNotablePoints(hist,src.cols,maxB,ln[1], ln[3], up-down, left, right);
    hr.a = n.a;
    hr.b = n.b;
    hr.c = n.c;
    hr.d = n.d;
    hr.hist = hist;
    line( drawing, hr.c, hr.a,Scalar( 255, 0, 0),1,8,0);
    line( drawing, hr.d, hr.b,Scalar( 255, 0, 0),1,8,0);

    line( drawing, Point(ln[2]-ln[0]*ln[2],ln[3+ln[1]*ln[2]]), Point(ln[2]+ln[0]*600,ln[3]+ln[1]*600),Scalar( 0, 255, 0),1,8,0);*/
    namedWindow( "Result", 1 );
    imshow( "Result", drawing );
  return hr;

}

notables getNotablePoints(int size, int *maxB, float horizA, float horizB, int noiseGap)
{
    notables res;
    //int hRef = horiz - horiz*HORIZON_GAP;
    bool lf = false;
    int maxGlobal = size;
    for(int i = 0; i <size; i++){
        int horiz = i*horizA + horizB;
        int noise = horiz -(abs(noiseGap)/2);
        if(maxB[i]<maxGlobal && maxB[i]< noise) maxGlobal = maxB[i];
        if(!lf){
            //left side
            if(maxB[i]< noise){
                res.c = Point(i,horiz);
                lf = true;
            }
        }else{
            if(maxB[i]> noise){
                res.d = Point(i-1,horiz);
                break;
            }
        }
    }
    res.a = Point(res.c.x,maxGlobal);
    res.b = Point(res.d.x,maxGlobal);
    return res;
}

int getHoriz(int center,const float v1ax,const float v2ax,const float v1ay,const float v2ay)
{
    int res = v2ay*((center-v1ax)/v2ax)+v1ay;

    return res;
}

float angleBetween(const float v1ax,const float v1ay,const float v2ax,const float v2ay,const float v3ax,const float v3ay)
{

    float v1x = v1ax-v3ax;
    float v1y = v1ay-v3ay;
    float v2x = v2ax-v3ax;
    float v2y = v2ay-v3ay;
    float len1 = sqrt(v1x * v1x + v1y * v1y);
    float len2 = sqrt(v2x * v2x + v2y * v2y);

    float dot = v1x * v2x + v1y * v2y;

    float a = dot / (len1 * len2);

    if (a >= 1.0)
        return 0.0;
    else if (a <= -1.0)
        return PI;
    else
        return acos(a); // 0..PI
}

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

/** @function rotate */
Mat rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(src.cols, src.rows));
    return dst;
}



map<int,int> getColumns(vector<vector<Point>> contours)
{
    map<int,int> res;
    for( uint i = 0; i< contours.size(); i++ )
    {
        for (uint j = 0; j < contours[i].size(); j++) {
            if (res.find(contours[i][j].x) != res.end()){
                //add
                res[contours[i][j].x] = res[contours[i][j].x]+1;
            }else{
                res[contours[i][j].x] = 1;
            }
        }

    }
    return res;
}



int getLeftColumn(map<int,int> hist, int middle)
{
    typedef map<int,int>::iterator it_type;
    int pos =0;
    int count=0;

    for(it_type iterator = hist.begin(); iterator != hist.end(); iterator++) {
        if(iterator->first >= middle) break;

        if(iterator->second > count){
            pos = iterator->first;
            count = iterator->second;
        }else if (iterator->second == count){
            if(iterator->first < pos){
                pos = iterator->first;
                count = iterator->second;
            }
        }
    }
    return pos;
}

int getRightColumn(map<int,int> hist, int middle)
{
    typedef map<int,int>::iterator it_type;
    int pos =0;
    int count=0;

    for(it_type iterator = hist.end(); iterator != hist.begin(); iterator--) {
        if(iterator->first <= middle) break;
        if(iterator->second > count){
            pos = iterator->first;
            count = iterator->second;
        }else if (iterator->second == count){
            if(iterator->first > pos){
                pos = iterator->first;
                count = iterator->second;
            }
        }
    }
    return pos;
}

