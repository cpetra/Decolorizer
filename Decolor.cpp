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
#ifdef _USE_QT
#include <QApplication>
#endif

using namespace std;
using namespace cv;

Decolor::Decolor(string filename, string window_name)
{
    this->low_edge_threshold  = 50;
    this->high_edge_threshold = 150;
    this->rho = 1.0;
    this->theta = 3.141 / 180;
    this->threshold = 1;
    this->min_line_length = 2;  // minimum number of pixels making up a line
    this->max_line_gap = 1; // maximum gap in pixels between connectable line segments
    this->window_name = window_name;
    this->blur_size = 3;

    this->filename = filename;
    this->image = imread(filename, 1);
    this->output_blank = Mat::zeros( this->image.size(), CV_8UC3 );
    this->output_blank.setTo(Scalar(255,255,255));

    this->controls = Mat::zeros( Size(320, 160), CV_8UC3 );
    this->controls.setTo(Scalar(255,255,255));


    this->edge_line_width = 2;
    this->contour_line_width = 2;
    Blank();
    GrayBlur();
    Edges();
    Lines();
    Contours();
 //   namedWindow(window_name, WINDOW_AUTOSIZE);
//    Display();
//    SetupTrackbarMaps();

    this->state_ok = !!(this->image.data);  
}

void Decolor::GrayBlur()
{
    cvtColor( image, img_gray, COLOR_BGR2GRAY );
    blur( img_gray, img_blur, Size(this->blur_size, this->blur_size) );
}

void Decolor::Edges()
{
    Canny( img_blur, img_edges, low_edge_threshold, high_edge_threshold);
}

void Decolor::Blank()
{
    output = output_blank;
}


void Decolor::Lines()
{
    HoughLinesP(img_edges, lines, rho, theta, threshold, min_line_length, max_line_gap);

    const int alpha = 1000;
    for(int i = 0; i < lines.size(); i++)
    {
        Vec4i l = lines[i];
        line(output, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 0), edge_line_width);
    }
}

void Decolor::Contours()
{
    findContours( img_edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
    for( size_t i = 0; i< contours.size(); i++ )
    {
        drawContours( output, contours, (int)i, Scalar(0, 0, 0), contour_line_width, LINE_8, hierarchy, 0 );
    }

}
void Decolor::Update()
{
    output_blank.setTo(Scalar(255,255,255));
    Blank();
    GrayBlur();
    Edges();
    Lines();
    Contours();
}

void Decolor::Display()
{
    Mat output2;
    imshow(window_name, image);
    imshow("output", output);
    imshow("controls", controls);
}
void Decolor::DisplayOutput()
{
    imshow("output", output);
}

void my_callback(int v, void* data)
{
    if (v <= 0) {
        return;
    }
    DecolorTracker *t = static_cast<DecolorTracker *>(data);
    *t->value = v;
    t->dec->Update(); 
    t->dec->Display(); 
}

void on_save(int v, void* data)
{
    cout << "clicked" << endl;
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
    Rect button;
    Mat3b canvas;
//    button = Rect(0,0, controls.cols, 50);
  //  canvas = Mat3b(100, 100, Vec3b(0,0,0));
    //canvas(button) = Vec3b(100,100,100);
   // putText(canvas(button), "hello", Point(button.width*0.35, button.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,0));
 //   createButton("Back", on_save, NULL, QT_PUSH_BUTTON,1);
    SetupTrackbar("Blur Size", &this->blur_size, 20);
    SetupTrackbar("Edge Line Width", &this->edge_line_width, 10);
    SetupTrackbar("Contour Line Width", &this->contour_line_width, 10);
    SetupTrackbar("High Edge Threshold", &this->high_edge_threshold, 255);
    SetupTrackbar("Low Edge Threshold", &this->low_edge_threshold, 255);

}

void Decolor::SetTrackbar(string name, int min, int max)
{
}

Decolor::~Decolor()
{
    for (auto t : trackbars) { delete t.second;}
}

#ifdef USE_QT
void Decolor::Display(QImage &qOriginal, QImage &qProcessed)
{
    qOriginal = QImage((uchar*)image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
    qProcessed = QImage((uchar*)output.data, output.cols, output.rows, output.step, QImage::Format_RGB888);
}
void Decolor::DisplayProcessed(QImage &qProcessed)
{
    qProcessed = QImage((uchar*)output.data, output.cols, output.rows, output.step, QImage::Format_RGB888);
}
#endif
void Decolor::SetBlur(int blur)
{
    blur_size = blur;
}
void Decolor::SetLineWidth(int width)
{
    edge_line_width = width;
}
void Decolor::SetContourWidth(int width)
{
    contour_line_width = width;
}
void Decolor::SetHighEdge(int value)
{
    high_edge_threshold = value;
}
void Decolor::SetLowEdge(int value)
{
    low_edge_threshold = value;
}
void Decolor::Save(string filename)
{
    imwrite(filename, output);
}
