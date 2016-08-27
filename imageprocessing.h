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

int startProcessing( char* filePath);
vector<string>  getAllFiles(char* folderPath);


#endif // IMAGEPROCESSING_H
