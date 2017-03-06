#include "config.h"


Config m_config;

//Config constructor

Config::Config()
{
    shrinkage = 95;
    shrinkage = 85;
}


void Config::setDataPath(string d){
    dataPath = d;
    noise_up = -1;
    noise_down = -1;
    noise_left = -1;
    noise_right = -1;
    rotation = -1;
}

string Config::getDataPath(){return dataPath;}

void Config::setShrinkage(int sh){
     shrinkage = sh;
};

int Config::getShrinkage(){return shrinkage;}

void Config::setShrinkage2(int sh){
    shrinkage2 = sh;
}
int Config::getShrinkage2(){return shrinkage2;}
