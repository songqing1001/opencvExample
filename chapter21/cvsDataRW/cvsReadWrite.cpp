/*
 * filestorage_sample demonstrate the usage of the opencv serialization functionality
 */

#include "opencv2/core.hpp"
#include <opencv2/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <string>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::ostream;
using namespace cv;
using namespace cv::ml;
using namespace std;

#define IMG_WIDTH 128
#define IMG_HEIGHT 128
#define IMG_CHANNEL 1

int main(int ac, char** av)
{
  string filename ;
  string path;
  if(ac > 2 )
  {
    filename = av[2];
    path = av[1];
  }
  else
  {
      printf(" should be ./*.out path.txt outputfilename\n");
  }
    printf("%s(%d):%s\n", __FUNCTION__, __LINE__, filename.c_str());

  if (filename.empty())
  {
      printf("Error:fileName NULL\n");
    return 1;
  }

  //write
  {

    ifstream fin;
    string tmp;
    fin.open(path.c_str());
//	FILE* fpt = fopen(filename.c_str(),(const char*)"w");
    ofstream file(filename.c_str());

    cout << "writing images\n";
    int i = 0;
    vector<string> FileVec;

    while(fin >> tmp)
    {
#if 0
        stringstream tmpStr;
        tmpStr<<"image"<<i;
        cout  <<tmpStr.str()<<endl;
        i++;
#endif
        FileVec.push_back(tmp);
    }

    vector<string>::iterator strIt;
    vector<Mat> AllImage;
    Mat MergeOut;
    int Cnt = 0;


    for(strIt = FileVec.begin();strIt != FileVec.end(); strIt++, Cnt++ )
    {
		Mat inputMat = imread(*strIt, 0);
    	int rows = inputMat.rows;
    	int cols = inputMat.cols;
		int channels = inputMat.channels();

        vector<Mat> imgVect;
        imgVect.resize(3);

        printf("%d,%d,%d\n", inputMat.rows, inputMat.cols, inputMat.channels());

        if(rows != IMG_WIDTH || cols != IMG_HEIGHT || channels != IMG_CHANNEL )
        {
            printf("Error %s: rows should be %d, cols should be %d, channels should be %d\n", strIt->c_str(), IMG_WIDTH, IMG_HEIGHT, IMG_CHANNEL);
            return 0;
        }
#if 0
        split(inputMat, imgVect);
        unsigned char *pData = inputMat.data;
        
        ofstream file("test.csv");
        file << format(imgVect[0], Formatter::FMT_CSV);
        file.close();
#endif
        Mat imgResize;
        resize(inputMat, imgResize, Size(64,64), 64, 64, INTER_LINEAR);

       // Mat ImgForTrain(1, inputMat.rows*inputMat.cols,  CV_8UC1, (void*)inputMat.data);
        Mat ImgForTrain(1, imgResize.rows*imgResize.cols,  CV_8UC1, (void*)imgResize.data);
        AllImage.push_back(ImgForTrain);
        if(Cnt == 0)
          MergeOut = ImgForTrain;
        else
            vconcat(ImgForTrain, MergeOut, MergeOut);
        //file << format(ImgForTrain, Formatter::FMT_CSV);
        printf("%d,%d,%d\n", MergeOut.rows, MergeOut.cols, MergeOut.channels());

    }
   // cout <<MergeOut<< endl;

    printf("%d,%d,%d\n", MergeOut.rows, MergeOut.cols, MergeOut.channels());

    Mat ImgLabel(MergeOut.rows, 1, CV_8UC1);
    for(int i = 0; i < MergeOut.rows; i++)
    {
        if(i < MergeOut.rows/2)
        {
            ImgLabel.at<unsigned char>(i, 0) = 1;
        }
        else
            ImgLabel.at<unsigned char>(i, 0) = 0;
    }
    hconcat( MergeOut, ImgLabel,  MergeOut);
    
    file << format(MergeOut, Formatter::FMT_CSV);

    //    fclose(fpt);

    fin.close();
    file.close();



  }

  //read
  {
    cout <<"\n"<<filename<<endl;

    Ptr<TrainData> train_data;
    train_data = TrainData::loadFromCSV(filename.c_str(), 0); 
    Mat m = train_data->getTrainSamples();
    //cout << format(m, Formatter::FMT_CSV) << endl;
    printf("\n%d,%d,%d\n", m.rows, m.cols, m.channels());
//    cout <<m<< endl;

//    cout << m.row(0) << endl;
    
    Mat label = train_data->getTrainResponses();
    printf("\n%d,%d,%d\n", label.rows, label.cols, label.channels());

  }


  return 0;
}
