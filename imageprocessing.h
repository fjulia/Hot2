#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <dirent.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace cv;
using namespace std;

int startProcessing( string filePath);

int processImage(string folderPath,string filePath, int fId);

Mat find_morph_size(Mat* dst,int m_size);

int reduceHorizon(Mat *src);
double findBouding(Mat *src);
vector<string>  getAllFiles(string folderPath);


#endif // IMAGEPROCESSING_H
