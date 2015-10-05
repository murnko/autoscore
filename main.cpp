#include <opencv2/opencv.hpp>
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"


int main(int argc, char *argv[])
{
    cv::Mat inputImage = cv::imread("/home/murnko/Downloads/opencv-3.0.0/samples/data/baboon.jpg");
    cv::imshow("Display Image", inputImage);


    cvWaitKey(0);
}
