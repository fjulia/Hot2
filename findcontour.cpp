#include "findcontour.h"
#include "config.h"
#include "imageutils.h"

using namespace cv;
using namespace std;

Mat rawSrc; Mat src; Mat src2; Mat src_gray; Mat src_bw; Mat drawing;Mat drawing2;
Mat dst;
//int thress = 100;
int max_thresh = 255;
int bw_thres = 128;
int rotation = 180;
int max_rotation = 360;
int hLine = 100;
RNG rng(12345);
char* source_window;

/// Function header
//void thresh_callback(int, void* );

/** @function main */
int findContour( char* filePath)
{   
  /// Load source image and convert it to gray
  rawSrc = imread( filePath, 1 );

  Size size(600,600);//the dst image size,e.g.100x100
  resize(rawSrc,src, size);//resize image

  /// Convert image to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );
  ///Convert image to B&W
  src_bw = src_gray > bw_thres;

  horizon hr = calcHorizon(src_bw, 100,500);
  cout << "rotate "<< hr.rotation << endl;
  cout << "a " << hr.a.x<< " b " << hr.b.x<< " c " << hr.c.x << " d " << hr.d.x <<endl;
  cout << "a " << hr.a.y<< " b " << hr.b.y<< " c " << hr.c.y << " d " << hr.d.y <<endl;
  src = rotate(src,-hr.rotation);
  Point2f pt1(0., hr.high);
  Point2f pt2(src.cols, hr.high);
  Scalar colorRed = Scalar( 0,0,255 );
  Scalar colorBlue = Scalar( 255,0,0 );
  line(src, hr.c, hr.a,colorBlue,1);
  line(src, hr.a, hr.b,colorBlue,1);
  line(src, hr.b, hr.d,colorBlue,1);
  line(src, pt1, pt2,colorRed,1);

  /// Create Window
  namedWindow( "Bars", CV_WINDOW_AUTOSIZE );
  imshow( "Bars", src );

  //calcHorizon(src_bw,100,100);

  //Set initial config values
//  m_config.setThresh(thress);


  //m_config.setRotation(rotation);
  //m_config.setHLine(src.rows/2.);
  
  //createTrackbar( " Rotation thresh:", "Bars", &m_config.rotation, max_rotation, thresh_callback );
//  createTrackbar( " HLine thresh:", "Bars", &m_config.hLine, src.rows, thresh_callback );
//  createTrackbar( " Canny thresh:", "Bars", &m_config.thresh, max_thresh, thresh_callback );
//  thresh_callback( 0, 0 );

  waitKey(0);
  return(0);
}



/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_bw, canny_output, m_config.getThresh(), m_config.getThresh()*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1, Point(0, 0) );
  /// Draw contours
  drawing = Mat::zeros( canny_output.size(), CV_8UC3 );

  map<int,int> hist = getColumns(contours);
  int lCol = getLeftColumn(hist,drawing.cols/2);
  int rCol = getRightColumn(hist,drawing.cols/2);


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
  Scalar colorWhite = Scalar( 255,255,255 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );

       drawContours( drawing, contours, i, color, CV_FILLED, 8, hierarchy, 0, Point() );
      // printf("controu %d, h1: %d, h2: %d, h3: %d, h4: %d\n",i, hierarchy[i][0],hierarchy[i][1],hierarchy[i][2],hierarchy[i][3]);
     }


 /* std::ostringstream oss;
  oss << "/tmp/draw-" << thresh<<".tif" ;
  std::string var = oss.str();
  imwrite( var, drawing );
*/
  //Draw Rotation   
  drawing = rotate(drawing,180-m_config.getRotation());
 
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



/*
  vector<vector<Point> > contours2;
  contours2.push_back( merged_contour_points_ap);
  Scalar color2 = Scalar( 200, 0, 0 );

  drawContours( drawing2, contours2, 0, color2, 2, 8, NULL, 0, Point() );
  namedWindow( "Contours2", CV_WINDOW_AUTOSIZE );
  imshow( "Contours2", drawing2 );
*/


  /// Calculate the area with the moments 00 and compare with the result of the OpenCV function
 // printf("\t Info: Area and Contour Length \n");
 // printf(" * Contour united - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", mo.m00, contourArea(merged_contour_points), arcLength( merged_contour_points, true ) );
  /*for( int i = 0; i< contours.size(); i++ )
     {
       printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );
     }*/
}

