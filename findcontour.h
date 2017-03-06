#ifndef FINDCONTOUR_H
#define FINDCONTOUR_H

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <map>


using namespace cv;
using namespace std;



int findContour( string filePath);
void thresh_callback(int, void* );
void config_callback(int, void* );
void callbackButton(int, void*);
Mat rotate(Mat src, double angle);
void calcNotablePoints();

#endif // FINDCONTOUR_H
