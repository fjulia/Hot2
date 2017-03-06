#ifndef CONFIG_H
#define CONFIG_H

#include <string>

using namespace std;

class Config
{

public:
    string dataPath;
    int noise_up;
    int noise_down;
    int noise_left;
    int noise_right;
    int rotation;

    int shrinkage;
    int shrinkage2;

    Config();
    void setDataPath(string d);

    string getDataPath();

    void setShrinkage(int sh);
    void setShrinkage2(int sh);

    int getShrinkage();
    int getShrinkage2();
};

extern Config m_config;

#endif // CONFIG_H
