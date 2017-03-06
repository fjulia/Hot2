#include "utils.h"

utils::utils()
{

}


string SplitFilename (const string& str)
{
  size_t found;
  //cout << "Splitting: " << str << endl;
  found=str.find_last_of("/\\");
  //cout << " folder: " << str.substr(0,found) << endl;
  //cout << " file: " << str.substr(found+1) << endl;
  return str.substr(0,found);
}
