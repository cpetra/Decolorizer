/******************************************************************************
 * @Date    : 2021-11-23 17:08:25
 * @Author  : Constantin Petra (constantin.petra@gmail.com)
 * @Link    : http://github.com/cpetra
 * @Version : $Id$
 * 
******************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "Decolor.hpp"

using namespace std;
using namespace cv;

Decolor::Decolor(string filename, string window_name)
{
    defaults();
    this->filename = filename;
    this->window_name = window_name;
    this->image = imread(filename, 1);
    this->state_ok = !!(this->image.data);

    if (this->state_ok) {
        this->output_blank = Mat::zeros( this->image.size(), CV_8UC3 );
        this->output_blank.setTo(Scalar(255,255,255));

        this->controls = Mat::zeros( Size(320, 160), CV_8UC3 );
        this->controls.setTo(Scalar(255,255,255));


        blank();
        grayBlur();
        Edges();
        Lines();
        doContours();
    }
 //   namedWindow(window_name, WINDOW_AUTOSIZE);
//    Display();
//    SetupTrackbarMaps();


}

void Decolor::defaults()
{
    this->low_edge_threshold  = 50;
    this->high_edge_threshold = 150;
    this->rho = 1.0;
    this->threshold = 1;
    this->min_line_length = 2;  // minimum number of pixels making up a line
    this->max_line_gap = 1; // maximum gap in pixels between connectable line segments
    this->blur_size = 3;
    this->edge_line_width = 2;
    this->contour_line_width = 2;
    this->output_is_shown = false;
}

void Decolor::grayBlur()
{
    int bsize = this->blur_size;
    cvtColor( image, img_gray, COLOR_BGR2GRAY );
    if (b_gaussian) {
        // For gaussian blur, the blur size needs to be odd.
        if ((bsize % 2) == 0) {
            bsize++;
        }
        GaussianBlur(img_gray, img_blur, Size(bsize, bsize), 0);
    }
    else {
        blur(img_gray, img_blur, Size(bsize, bsize));
    }
}

void Decolor::Edges()
{
    Canny( img_blur, img_edges, low_edge_threshold, high_edge_threshold);
}

void Decolor::blank()
{
    output_blank.copyTo(output);
}


void Decolor::Lines()
{
    const double theta = 3.141 / 180.0;
    HoughLinesP(img_edges, lines, rho, theta, threshold, min_line_length, max_line_gap);

    for(auto &l : lines)
    {
        line(output, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 0), edge_line_width);
    }
}

void Decolor::doContours()
{
    findContours( img_edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_TC89_KCOS );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        drawContours( output, contours, (int)i, Scalar(0, 0, 0), contour_line_width, LINE_8, hierarchy, 0 );
    }

}
void Decolor::update()
{
    if (!ok()) {
        return;
    }

    blank();
    grayBlur();
    Edges();
    Lines();
    doContours();
}

void Decolor::display()
{
    if (!ok()) {
        return;
    }
    Mat output2;
    imshow(window_name, image);
    imshow("output", output);
    imshow("controls", controls);
}

void Decolor::displayOutput()
{
    if (!ok()) {
        return;
    }
    if(output_is_shown) {
        destroyWindow("output");
    }
    output_is_shown = true;
    imshow("output", output);
    // This does not seem to work if shown a second time, not sure why.
    setWindowProperty("output", WND_PROP_TOPMOST, 1);
}

void my_callback(int v, void* data)
{
    if (v <= 0) {
        return;
    }
    DecolorTracker *t = static_cast<DecolorTracker *>(data);
    *t->value = v;
    t->dec->update();
    t->dec->display();
}

void Decolor::SetupTrackbar(string name, int* pvalue, int max)
{
    auto dLambda = [] (Decolor *d, int *value) {
        DecolorTracker *t = new DecolorTracker;
        t->dec = d;
        t->value = value; 
        return t;
    };
    trackbars[name] = dLambda(this, pvalue);
    createTrackbar(name, "controls", NULL, max, my_callback, static_cast<void *>(trackbars[name]) );
    setTrackbarPos(name, "controls", *pvalue);

}

void Decolor::SetupTrackbarMaps()
{
    SetupTrackbar("Blur Size", &this->blur_size, 20);
    SetupTrackbar("Edge Line Width", &this->edge_line_width, 10);
    SetupTrackbar("Contour Line Width", &this->contour_line_width, 10);
    SetupTrackbar("High Edge Threshold", &this->high_edge_threshold, 255);
    SetupTrackbar("Low Edge Threshold", &this->low_edge_threshold, 255);
}

void Decolor::setTrackbar(string name, int min, int max)
{
}

Decolor::~Decolor()
{
    for (auto &t : trackbars)
    {
        delete t.second;
    }
}

void Decolor::setBlur(int blur)
{
    blur_size = blur;
}
void Decolor::setLineWidth(int width)
{
    edge_line_width = width;
}
void Decolor::setContourWidth(int width)
{
    contour_line_width = width;
}
void Decolor::setHighEdge(int value)
{
    high_edge_threshold = value;
}
void Decolor::setLowEdge(int value)
{
    low_edge_threshold = value;
}
void Decolor::save(string filename)
{
    imwrite(filename, output);
}

void Decolor::getImages(const Mat ** image , const Mat ** output)
{
    *image = &this->image;
    *output = &this->output;
}

void Decolor::getOutputImage(const Mat ** output)
{
    *output = &this->output;
}
