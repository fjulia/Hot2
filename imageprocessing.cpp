#include "imageprocessing.h"
#include "config.h"

#define PI 3.14159265

using namespace cv;
using namespace std;

int bw_thres_img = 25;
int thresh_img = 100;

int morph_elem = 0;
int morph_size = 0;
int morph_operator = 0;
int const max_operator = 4;
int const max_elem = 2;
int const max_kernel_size = 21;
RNG rng_img(12345);

vector<double> areas;
vector<string> files;
double init_area;
int first_shringkage;
int second_shringkage;

int startProcessing( string folderPath)
{
    first_shringkage = -1;
    second_shringkage = -1;
    files = getAllFiles(folderPath);
    for(uint t=0;t<files.size();++t){
        //cout<<files.at(t);
        //if(t==0){
        cout << "processing image "<< t << " of " << files.size() << endl;
        processImage(folderPath,files[t], t);
        //}
    }
    cout << "first shrinkage " << first_shringkage << "second shrinkage " << second_shringkage << endl;
    return 0;
}

int processImage(string folderPath,string filePath, int fId){
    string window_name = "DebugWin";

    Mat rawSrc; Mat src; Mat src_gray; Mat src_bw;
    string fullPathFile = folderPath +"/"+filePath;
    rawSrc = imread( fullPathFile, 1 );
    Size size(600,600);//the dst image size,e.g.100x100
    resize(rawSrc,src, size);//resize image
    //setHorizon
    //namedWindow( window_name, CV_WINDOW_AUTOSIZE );
    reduceHorizon(&src);
    //imshow( window_name,src );
    /// Convert image to gray
    cvtColor( src, src_gray, CV_BGR2GRAY );
    ///Convert image to B&W
    src_bw = src_gray < bw_thres_img;
    /// Create Window
    Mat dst;
    dst = find_morph_size(&src_bw,0);
    //    imshow( window_name,dst );
    double area = findBouding(&dst);
    if(fId == 0){
        init_area = area;
    }else{
        if(first_shringkage <0){
            int ap = (int)((area/init_area)*100);
            if(ap<=m_config.getShrinkage()){
                //first shrinkage found
                first_shringkage = fId;
            }
        }else{
            if(second_shringkage<0){
                int ap = (int)((area/init_area)*100);
                if(ap<=m_config.getShrinkage2()){
                    //second shrinkage found
                    second_shringkage = fId;
                }

            }
        }
    }
    areas.push_back(area);

   // waitKey(0);
    return 0;
}

double findBouding(Mat *src){
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Canny( *src, canny_output, thresh_img, thresh_img*2, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1, Point(0, 0) );
    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    //  for( int i = 0; i < contours.size(); i++ )
    //  {
    approxPolyDP( Mat(contours[0]), contours_poly[0], 3, true );
    //boundRect[i] = boundingRect( Mat(contours_poly[i]) );
    //minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    //  }
    double area = contourArea(contours_poly[0]);
    return area;
    /*  cout << "area "<< area << endl;

    /// Draw polygonal contour + bonding rects + circles
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng_img.uniform(0, 255), rng_img.uniform(0,255), rng_img.uniform(0,255) );
        drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
        circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
    }

    /// Show in a window
    namedWindow( "Contours", WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );*/
}


int reduceHorizon(Mat *src){

    int hnumcol = src->cols/2;
    for(int y=0 ; y<src->cols ; y++)
    {
        for(int x=0 ; x<src->rows; x++)
        {
            if(y<hnumcol){
                if(x>m_config.noise_down+(tan(m_config.rotation* PI / 180.0)*(hnumcol-y))){
                    src->at<Vec3b>(x,y) = Vec3b(255,255,255);
                }
            }else if(y==hnumcol){
                if(x>m_config.noise_down){
                    src->at<Vec3b>(x,y) = Vec3b(255,255,255);
                }
            }else{
                if(x>m_config.noise_down-(tan(m_config.rotation* PI / 180.0)*(y-hnumcol))){
                    src->at<Vec3b>(x,y) = Vec3b(255,255,255);
                }
            }
        }
    }
}


Mat find_morph_size(Mat* src_bw, int m_size){
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat dst;
    int operation = morph_operator + 2;

    Mat element = getStructuringElement( morph_elem, Size( 2*m_size + 1, 2*m_size+1 ), Point( m_size, m_size) );

    /// Apply the specified morphology operation
    morphologyEx( *src_bw, dst, operation, element );

    Canny( dst, canny_output, thresh_img, thresh_img*2, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1, Point(0, 0) );

    if(contours.size()>1){

        return find_morph_size(src_bw, ++m_size);
    }else{
        return dst;
    }
}

vector<string> getAllFiles(string folderPath){
    DIR *dir;
    struct dirent *ent;
    vector<string> res;
    std::string extension = "TIF";
    res.clear();
    if ((dir = opendir (folderPath.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            //printf ("%s\n", ent->d_name);
            if (ent->d_type == DT_REG){
                string fname = ent->d_name;
                if(fname.find(extension,fname.length()-3)!= std::string::npos){
                    res.push_back(ent->d_name);
                }
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");

    }
    std::sort( res.begin(), res.end() );
    return res;
}
