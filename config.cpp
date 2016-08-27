#include "config.h"


Config m_config;

//Config constructor

Config::Config()
{
    //Default Canny thresh
    thresh = 150;
}

void Config::setThresh(int t){
    thresh = t;
}

void Config::setRotation(int r){
    rotation = r;
}

void Config::setHLine(int h){
    hLine = h;
}

void Config::setDataPath(char* d){
    dataPath = d;
}

int Config::getThresh(){return thresh;}
int Config::getRotation(){return rotation;}
int Config::getHLine(){return hLine;}
char* Config::getDataPath(){return dataPath;}
