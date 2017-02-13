#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;



cv::Vec3b computeMeanColor(const cv::Mat& area_){
    cv::Vec3b res(0,0,0);
    float b=0, g=0, r=0;
    for(int x = 0; x < area_.rows; ++x){
        for(int y = 0; y < area_.cols; ++y){
            b += area_.at<cv::Vec3b>(x,y)[0];
            g += area_.at<cv::Vec3b>(x,y)[1];
            r += area_.at<cv::Vec3b>(x,y)[2];
        }
    }
    int size = area_.rows * area_.cols;
    b /= size;
    g /= size;
    r /= size;
    res.val[0] = int(b);
    res.val[1] = int(g);
    res.val[2] = int(r);
    return res;
}

bool isBlueArea(const cv::Vec3b& color){
    return (float(color.val[2]) / float(color.val[0]+color.val[1]+color.val[2]) > 0.45);
}

int MAX_SIZEAREA = 40;

int main(int argc, char *argv[])
{

    cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    cv::Mat edges;
    cv::namedWindow("edges",1);
    int sizeArea = MAX_SIZEAREA;
    int counter = 0;
    for(;;)
    {
        cv::Mat frame;
        cap >> frame; // get a new frame from camera

        for(int i = 0; i < frame.rows-sizeArea; i+=sizeArea)
        {
            for(int j = 0; j < frame.cols-sizeArea; j+=sizeArea){
                if(isBlueArea(computeMeanColor(frame(cv::Rect(j, i, sizeArea, sizeArea))))){
                    cv::rectangle(frame, cv::Point(j,i), cv::Point(j+sizeArea, i+sizeArea), cv::Scalar(255,0,0));
                }
            }
        }

        cv::imshow("edges", frame);
        if(cv::waitKey(30) == 0) break;

        if(!(counter++ % 3) && !--sizeArea)
            sizeArea = MAX_SIZEAREA;

    }
    return 0;
}
