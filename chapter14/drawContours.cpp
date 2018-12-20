
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

struct AreaCmp{
    AreaCmp(const vector<float>& _areas):areas(&_areas){}
    bool operator()(int a, int b) const{ return (*areas)[a] > (*areas)[b];}
    const vector<float>* areas;
};

int main(int argc, char *argv[])
{
    Mat img, img_edge, img_color;

    //load image or show help if no image was provided
    if(argc != 2 ||(img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE)).empty())
    {
        cout<<"should input image name as parameter"<<endl;
        return -1;
    }

    threshold(img, img_edge, 128, 255, THRESH_BINARY);
 //   imwrite("./binary,jpg", img_edge);
    
    vector< vector< Point> > contours;
    vector< Vec4i> hierarchy;

    findContours(img_edge, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

    cout<< "hit any key to draw the next contour, esc to quit"<<endl;

    cout<<"total contours detected : "<< contours.size() <<endl;

    vector<int> sortIdx(contours.size());
    vector<float> areas(contours.size());

    for(int n = 0; n < (int)contours.size(); n++)
    {
        sortIdx[n] = n;
        areas[n] = contourArea(contours[n], false);
    }

    //sort contours so that the largest contours go first
    //
    sort( sortIdx.begin(), sortIdx.end(), AreaCmp(areas));

    int maxIter= 50;
    if((int) sortIdx.size() < maxIter)
    {
        maxIter = (int) sortIdx.size();
    }
    
    cvtColor(img, img_color, CV_GRAY2RGBA);

    for( int n = 0; n < maxIter; n++)
    {
        int idx = sortIdx[n];

       // cvtColor(img, img_color, CV_GRAY2RGBA);

        cout<<"*********"<<n<<"**********"<<endl;

        drawContours(img_color, contours, idx, Scalar(0, 0, 255), 2, 8,  hierarchy, 0);

        cout << n << " " << "contour #"<<idx<<" :area = "<< areas[idx]<<", nvertices="<<contours[idx].size() <<endl;

   //     imwrite("./contour.jpg", img_color);
        stringstream f;
        f<<"./contour"<<n<<".jpg";
        string filename = f.str();
        imwrite(filename, img_color);

        int k;

       // if((k=cv::waitKey()&255) == 27)
         // break;
        //if(k = waitKey())
       // {
            continue;
        //}
        cout<<"*********"<<n<<"*****end*****"<<endl;
    }
        imwrite("./contour.jpg", img_color);

    cout <<"finished"<<endl;
    
    return 0;
}

