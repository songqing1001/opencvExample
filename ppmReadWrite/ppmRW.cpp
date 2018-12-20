#include <cv.h>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>

using namespace std;

#define HEADER_MAXLINE 1024

// 从文件读取PPM图片
void pgm_load(string filename, unsigned char* out_data, int* w, int* h)
{
    char header[1024];
    FILE* fp = NULL;
    int line = 0;
    unsigned char tmpData1, tmpData2;
    int maxValue;

    fp = fopen(filename.c_str(), "rb");

    // 读取图片格式(例如:"P5")
    // 高宽在第二行非注释数据
    while(line < 2){    
        fgets(header, 1024, fp);
        if(header[0] != '#'){
            ++line;
        }
    }
    // 读取宽高
    sscanf(header,"%d %d\n", w, h);

    printf("w %d, h %d\n", *w, *h);

    // 获取最大像素值
    fgets(header, 20, fp);
#if 1
    sscanf(header,"%d", &maxValue);
    printf("max value %d\n", maxValue);
#endif
    printf("%s", header);

    // get rgb data
    //fread(out_data, (*w)*(*h)*3, 1, fp);
    fread(out_data, (*w)*(*h)*2, 1, fp);
   

    fclose(fp);
}


int main(int argc, char **argv)
{
    int w = 0, h = 0;
    string fileList = argv[1];// "./1.pgm";

    unsigned char outdata1[640*480*2];
    unsigned short outdata3[640*480];
    unsigned char outdata2[640*480*1];
    unsigned short valueMax = 0;


    ifstream fin;
    string fileName;
    fin.open(fileList.c_str());
    while(fin >> fileName)
    {
        char outFileName[256];
       pgm_load(fileName, outdata1, &w, &h );
    
       for(int i = 0; i < 640*480; i++)
       {
           outdata3[i] = (unsigned short)(outdata1[i*2]<<8) + outdata1[i*2+1];

           if(outdata3[i] > valueMax)
           {
              valueMax = outdata3[i];
           }
       }
       for(int i = 0; i < 640*480; i++)
       {
           outdata2[i] = outdata3[i]*255/valueMax;
       }

       memset(outFileName, 0, sizeof(outFileName));
       snprintf(outFileName, sizeof(outFileName)-1, "%s.jpg", fileName.c_str() );

       cv::Mat src1(h, w, CV_8UC1, outdata2); 
       cv::imwrite(outFileName, src1);
    }

   return 0;
}
