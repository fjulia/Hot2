#include "findcontour.h"
#include "config.h"
#include "imageutils.h"
#include <math.h>
#include "highgui.h"


#define PI 3.14159265

using namespace cv;
using namespace std;

Mat rawSrc; Mat src; Mat src_conf; Mat src_gray; Mat src_bw; Mat drawing;Mat drawing2;
Mat dst;
//int thress = 100;
int bw_thres = 25;
int max_vertical_noise = 255;
int max_horizontal_noise = 255;
int max_rotation = 180;


int thresh = 100;
int max_thresh = 255;


RNG rng(12345);
string source_window;
string window_name = "Config";

/// Function header
//void thresh_callback(int, void* );

/** @function main */
int findContour( string filePath)
{   
  /// Load source image and convert it to gray
  rawSrc = imread( filePath, 1 );

  Size size(600,600);//the dst image size,e.g.100x100
  resize(rawSrc,src, size);//resize image


  /// Convert image to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );
  ///Convert image to B&W
  src_bw = src_gray < bw_thres;


  //src = rotate(src,-hr.rotation);


  /// Create Window
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /// Create Trackbar to select Morphology operation
  //createTrackbar("Operator:\n 0: Opening - 1: Closing \n 2: Gradient - 3: Top Hat \n 4: Black Hat", window_name, &morph_operator, max_operator, thresh_callback );

  /// Create Trackbar to select kernel type
//  createTrackbar( "Element:\n 0: Rect - 1: Cross - 2: Ellipse", window_name,
  //                &morph_elem, max_elem,
   //               thresh_callback );

  /// Create Trackbar to choose kernel size
  //createTrackbar( "Kernel size:\n 2n +1", window_name,
  //                &morph_size, max_kernel_size,
  //                thresh_callback );


  //createTrackbar( " Canny thresh:", window_name, &thresh, max_thresh, thresh_callback );

  //calcHorizon(src_bw,100,100);

  //Set initial config values
 // if(m_config.noise_up==-1)m_config.noise_up=(src.rows/2);
  if(m_config.noise_down==-1)m_config.noise_down=(src.rows);
  //if(m_config.noise_left==-1)m_config.noise_left=(1);
  //if(m_config.noise_right==-1)m_config.noise_right=(src.cols-1);
  if(m_config.rotation==-1)m_config.rotation = 90;
  max_vertical_noise = src.rows;
  //max_horizontal_noise = src.cols;

  //int button;
  
  //createTrackbar( " Up Noise:", "Config", &m_config.noise_up, max_vertical_noise, config_callback );
  String s = " Down Noise:";
  createTrackbar( s, window_name, &m_config.noise_down, max_vertical_noise, config_callback );
  //createTrackbar( " Left Noise:", "Config", &m_config.noise_left, max_horizontal_noise, thresh_callback );
  //createTrackbar( " Right Noise:", "Config", &m_config.noise_right, max_horizontal_noise, thresh_callback );
  s = " Rotation:";
  createTrackbar( s, window_name, &m_config.rotation, max_rotation, config_callback );
  //createTrackbar( " Calc:", "Config", &button, max_rotation, callbackButton );


  config_callback( 0, 0 );
  //thresh_callback( 0, 0 );

  while(true){
    waitKey(1);
    if(getWindowProperty(window_name,WND_PROP_VISIBLE)<=0){
        return(0);
    }
  }
}



void callbackButton(int, void*)
{
    //calcNotablePoints();
}

void calcNotablePoints()
{
   // src_bw = rotate(src_bw,90-m_config.rotation);
    /*horizon hr = calcHorizon(src_bw, m_config.noise_left,m_config.noise_right, m_config.noise_up, m_config.noise_down);
    src.copyTo(drawing);
    Point2f pt1(0., hr.high);
    Point2f pt2(drawing.cols, hr.high);
    Scalar colorRed = Scalar( 0,0,255 );
    Scalar colorBlue = Scalar( 255,0,0 );
    line(drawing, hr.c, hr.a,colorBlue,1);
    line(drawing, hr.a, hr.b,colorBlue,1);
    line(drawing, hr.b, hr.d,colorBlue,1);
    line(drawing, pt1, pt2,colorRed,1);
    for( int i = 1; i < src.cols; i++ )
        {
      //    line(drawing, Point( i,  hr.hist[i]) , Point( i-1, hr.hist[i-1] ), Scalar( 255, 0, 0), 2, 8, 0  );
        }
    namedWindow( "Config_Result", CV_WINDOW_AUTOSIZE );
    imshow( "Config_Result", drawing );*/
}

void config_callback(int, void*){
    src.copyTo(src_conf);
    Scalar colorRed = Scalar( 0,0,255 );
    int ly = (int)(m_config.noise_down+((src_conf.cols/2)*tan(m_config.rotation* PI / 180.0 )));
    int ry = (int)(m_config.noise_down-((src_conf.cols/2)*tan(m_config.rotation* PI / 180.0 )));
    Point left = Point(0,ly);
    Point right = Point(src_conf.cols,ry);
    line(src_conf, left, right,colorRed,1);
    imshow( "Config", src_conf );
}




/** @function thresh_callback */
void thresh_callback(int, void* )
{

    /// Show in result
    namedWindow( "Result", CV_WINDOW_AUTOSIZE );
    //int m_size = find_morph_size(morph_size);

   /* src.copyTo(src_conf);
    src_conf = rotate(src_conf,90-m_config.rotation);
    Scalar colorRed = Scalar( 0,0,255 );
    Scalar colorBlue = Scalar( 255,0,0 );
     line(src_conf, Point(m_config.noise_left,0), Point(m_config.noise_left,src_conf.rows),colorBlue,1);
     line(src_conf, Point(m_config.noise_right,0), Point(m_config.noise_right,src_conf.rows),colorBlue,1);
     line(src_conf, Point(0,m_config.noise_up), Point(src_conf.cols,m_config.noise_up),colorRed,1);
     line(src_conf, Point(0,m_config.noise_down), Point(src_conf.cols,m_config.noise_down),colorRed,1);

    namedWindow( "Config_lines", CV_WINDOW_AUTOSIZE );
    imshow( "Config_lines", src_conf );
    */


 Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  Canny( dst, canny_output, thresh, thresh*2, 3 );
  // Detect edges using canny
  //Canny( src_bw, canny_output, m_config.getThresh(), m_config.getThresh()*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1, Point(0, 0) );
  /// Draw contours
  drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
/*
  map<int,int> hist = getColumns(contours);
  int lCol = getLeftColumn(hist,drawing.cols/2);
  int rCol = getRightColumn(hist,drawing.cols/2);
*/

  // approximate contours
  /*std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
  for( int i = 0; i < contours.size(); i++ ) {
    approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 5, true );
  }*/


  // merge all contours into one vector
  /*std::vector<cv::Point> merged_contour_points;
  for (int i = 0; i < contours.size(); i++) {
     // std::sort(contours[i].begin(), contours[i].end(), pointSort);
          cout<< "merging contour" <<i<<endl;
          cout << "contour size "<< contours[i].size()<< endl;
          for (int j = 0; j < contours[i].size(); j++) {
        //         cout << "point: "<< contours[i][j] << endl;
                merged_contour_points.push_back(contours[i][j]);
          }
  }

  std::vector<cv::Point> merged_contour_points_ap;
  approxPolyDP(cv::Mat(merged_contour_points), merged_contour_points_ap, 5, false );
*/

  //contours[0] = merged_contour_points;
  //Moments mo;
  //mo = moments(merged_contour_points,false);

  /// Get the moments
  /*vector<Moments> mu(contours.size() );
  for( int i = 0; i < contours.size(); i++ )
     { mu[i] = moments( contours[i], false ); }*/


  //drawing2 = Mat::zeros( canny_output.size(), CV_8UC3 );
  //Scalar colorWhite = Scalar( 255,255,255 );
  for( uint i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

       drawContours( drawing, contours, i, color, CV_FILLED, 8, hierarchy, 0, Point() );
       printf("controu %d, h1: %d, h2: %d, h3: %d, h4: %d\n",i, hierarchy[i][0],hierarchy[i][1],hierarchy[i][2],hierarchy[i][3]);
     }
 imshow( "Result", drawing );



  //Draw Rotation   
 /* drawing = rotate(drawing,180-m_config.getRotation());
 
  Point2f pt1_vleft(lCol, 0.);
  Point2f pt2_vleft(lCol, drawing.rows);
  Point2f pt1_vright(rCol, 0.);
  Point2f pt2_vright(rCol, drawing.rows);
  Point2f pt1(0., m_config.getHLine());
  Point2f pt2(drawing.cols, m_config.getHLine());
  //Draw HLine
  Scalar colorRed = Scalar( 0,0,255 );
  Scalar colorBlue = Scalar( 255,0,0 );
  line(drawing, pt1, pt2,colorRed,1);   
  line(drawing, pt1_vleft, pt2_vleft,colorBlue,1);
  line(drawing, pt1_vright, pt2_vright,colorBlue,1);
  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );


  ///Show lines in origin image
  src.copyTo(src2);
  src2 = rotate(src2,180-m_config.getRotation());
  line(src2, pt1, pt2,colorRed,1);
  line(src2, pt1_vleft, pt2_vleft,colorBlue,1);
  line(src2, pt1_vright, pt2_vright,colorBlue,1);
  namedWindow( "Source", CV_WINDOW_AUTOSIZE );
  imshow( "Source", src2 );
*/
}


