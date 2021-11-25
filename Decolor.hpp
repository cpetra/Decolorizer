/******************************************************************************
 * @Date    : 2021-11-23 17:07:24
 * @Author  : Constantin Petra (constantin.petra@gmail.com)
 * @Link    : http://github.com/cpetra
 * @Version : $Id$
 *
 *****************************************************************************/

#ifndef __DECOLOR_HPP_H_INCLUDED
#define __DECOLOR_HPP_H_INCLUDED
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#ifdef USE_QT
#include <QApplication>
#endif

using namespace cv;
using namespace std;

class Decolor;
 
typedef struct {
    Decolor *dec;
    int *value;
} DecolorTracker;

class Decolor {
private:
    Mat image;          // original image
    Mat img_gray;       // grayed image
    Mat img_blur;       // blurred image
    Mat img_edges;      // for displaying the edges
    Mat output_blank;   // use this as canvas for the output
    Mat output;
#ifndef _USE_QT
    Mat controls;
#endif
    string filename;
    string window_name;

    int blur_size;      // blur algorithm size

    bool state_ok = false;

    // edge detection 
    int low_edge_threshold;
    int high_edge_threshold;

    // lines detection
    double rho;
    double theta;
    int threshold;
    int min_line_length;
    int max_line_gap;
    vector<Vec4i> lines;

    int edge_line_width;
    int contour_line_width;

    // contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    // trackbars
    std::map<std::string, DecolorTracker *> trackbars;
    void SetupTrackbarMaps();
    void SetupTrackbar(string name, int* value, int max);



public:
    Decolor(string filename, string display_window);
    void GrayBlur();
    void Edges();
    void Lines();
    void Contours();
    void Display();
    void DisplayOutput();
    void Blank();
    void Update();
    void SetTrackbar(string name, int min, int max);
    bool ok() {return state_ok;}
    ~Decolor();
#ifdef USE_QT
    void Display(QImage &qOrig, QImage &qProcessed);
    void DisplayProcessed(QImage &qProcessed);
#endif
    // Interface parts;
    void SetBlur(int blur);
    void SetLineWidth(int width);
    void SetContourWidth(int width);
    void SetHighEdge(int value);
    void SetLowEdge(int value);
    int getBlur(){return blur_size;}
    int getLineWidth(){return edge_line_width;}
    int getContourWidth(){return contour_line_width;}
    int getHighEdge(){return high_edge_threshold;}
    int getLowEdge(){return low_edge_threshold;}
    void Save(string filename);
};
#endif /*__DECOLOR_HPP_H_INCLUDED*/
