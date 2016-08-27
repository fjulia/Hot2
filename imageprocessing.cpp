#include "imageprocessing.h"

using namespace cv;
using namespace std;

int startProcessing( char* folderPath)
{
    vector<string> files = getAllFiles(folderPath);
    for(int t=0;t<files.size();++t){
           cout<<files.at(t);
    }
    return 0;
}

int processImage(char* filePath){
    Mat rawSrc;
    rawSrc = imread( filePath, 1 );

}

vector<string> getAllFiles(char* folderPath){
    DIR *dir;
    struct dirent *ent;
    vector<string> res;
    res.clear();
    if ((dir = opendir (folderPath)) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        //printf ("%s\n", ent->d_name);
        res.push_back(ent->d_name);
      }
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");

    }
    return res;
}
