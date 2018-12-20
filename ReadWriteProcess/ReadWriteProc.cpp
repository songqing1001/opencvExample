#include <cv.h>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>

using namespace std;


int main(int argc, char* argv[])
{
    int w = 0, h = 0;
    string fileName1 = argv[1];
    string fileName2 = argv[2];


    cv::Mat src1 = cv::imread(fileName1);
    printf("%d,%d,%d\n", src1.rows, src1.cols, src1.channels());
    cv::imwrite("./1.jpg", src1);

    cv::Mat src2 = cv::imread(fileName2);
    printf("%d,%d,%d\n", src2.rows, src2.cols, src2.channels());
    cv::imwrite("./2.jpg", src2);


   return 0;
}
