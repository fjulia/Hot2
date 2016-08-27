#ifndef FINDCONTOUR_H
#define FINDCONTOUR_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
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



int findContour( char* filePath);
void thresh_callback(int, void* );
Mat rotate(Mat src, double angle);

#endif // FINDCONTOUR_H
