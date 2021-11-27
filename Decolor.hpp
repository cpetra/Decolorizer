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
    Mat controls;

    const string input_window_name = "Original";
    const string output_window_name = "Output";
    const string control_window_name = "Control";

    // original image name
    string filename;

    bool output_is_shown;

    // blur settings
    bool b_gaussian;
    int blur_size;


    // edge detection
    int low_edge_threshold;
    int high_edge_threshold;

    // lines detection
    double rho;
    int threshold;
    int min_line_length;
    int max_line_gap;
    int edge_line_width;
    int contour_line_width;

    // trackbars
    std::map<std::string, DecolorTracker *> trackbars;
    void SetupTrackbarMaps();
    void SetupTrackbar(string name, int* value, int max);

private:
    void defaults();
    void blank();
    void doContours();
    void grayBlur();
    void doEdges();
    void doLines();

public:
    Decolor(string filename);
    ~Decolor();
    bool ok() {return !!(this->image.data);}

    void display();             // show all windows, meant for standalone usage
    void displayOutput();       // show only output

    void update();              // do all calculations
    void save(string filename); // save the image

    void getImages(const Mat **image, const Mat **output);
    void getOutputImage(const Mat **image);

    // Interface
    void setBlur(int blur);
    void setLineWidth(int width);
    void setContourWidth(int width);
    void setHighEdge(int value);
    void setLowEdge(int value);
    void setGaussian(bool b){b_gaussian = b;}

    int getBlur(){return blur_size;}
    int getLineWidth(){return edge_line_width;}
    int getContourWidth(){return contour_line_width;}
    int getHighEdge(){return high_edge_threshold;}
    int getLowEdge(){return low_edge_threshold;}
    bool getGaussian(){return b_gaussian;}
    bool getB_gaussian() const;
    void setB_gaussian(bool newB_gaussian);
};
#endif /*__DECOLOR_HPP_H_INCLUDED*/
