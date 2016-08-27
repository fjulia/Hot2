#ifndef CONFIG_H
#define CONFIG_H


class Config
{
   private:
    char* dataPath;

   public:

    int thresh;
    int rotation;
    int hLine;

    Config();

    void setThresh(int t);
    void setRotation(int r);
    void setHLine(int h);
    void setDataPath(char* d);

    int getThresh();
    int getRotation();
    int getHLine();
    char* getDataPath();
};

extern Config m_config;

#endif // CONFIG_H
