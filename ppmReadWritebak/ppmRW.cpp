#include <cv.h>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>

using namespace std;

#define HEADER_MAXLINE 1024

#if 1
// 从文件读取PPM图片
void ppm_load(string filename, unsigned char* out_data, int* w, int* h)
{
    char header[1024];
    FILE* fp = NULL;
    int line = 0;
    unsigned char tmpData1, tmpData2;

    fp = fopen(filename.c_str(), "rb");

    // 读取图片格式(例如:"P6")
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

    // get rgb data
    //fread(out_data, (*w)*(*h)*3, 1, fp);
    fread(out_data, (*w)*(*h)*2, 1, fp);
   

    fclose(fp);
}

// 写ppm图像到文件
void ppm_save(char* filename, unsigned char* data, int w, int h)
{
    FILE* fp;
    char header[20];

    fp = fopen(filename, "wb");

    // 写图片格式、宽高、最大像素值
    fprintf(fp,"P6\n%d %d\n255\n",w,h);

    // 写RGB数据
    fwrite(data, w*h*3, 1, fp);

    fclose(fp);
}
#endif

unsigned char matppm[640*480*3];

int nextline(char *line, FILE *fp)
{
 char *p;
 do {
  p = fgets(line, HEADER_MAXLINE, fp);
 } while ( p != NULL && *p == '#' );
 if( p==NULL )
  return -1;
 return 0;
}

int read_header(FILE *fin, int *widthp, int *heightp, int *maxvalp, int *comp)
{
 char line[HEADER_MAXLINE];
 int  cols,rows,maxval;
 if(nextline(line, fin)!=0) return -10;
 if(strncmp(line,"P5",2)==0) 
  *comp=1;
 else if (strncmp(line,"P6",2)==0)
  *comp=3;
 else if (strncmp(line,"P7",2)==0)
  *comp=4;
 else
  return -1;
 if(strlen(line)>3)
 {
  if(sscanf(line+2,"%d %d %d",&cols,&rows,&maxval )!=3 )
   return -1;
 }
 else
 {
  if(nextline(line, fin)!=0) return -10;
  if(sscanf(line,"%d %d",&cols,&rows)!=2 )
   return -1;
  if(nextline(line, fin)!=0) return -10;
  if(sscanf(line,"%d",&maxval)!=1)
   return -1;
 }
 *widthp = cols;
 *heightp = rows;
 *maxvalp = maxval;

 printf("%d, %d ,%d ,%d\n",  *widthp,  *heightp,  *maxvalp,  *comp);

 return 0;
}

void read_ppm_image(char * image_name , int * rows, int * cols, unsigned char *data)
{
 FILE * fp;
 int widthp;
 int heightp;
 int maxvalp;
 int comp;
  short max=0;
 fp = fopen(image_name, "rb");
 read_header(fp, &widthp, &heightp, &maxvalp,&comp);
  *rows = heightp;
  *cols = widthp;
  //fread(data,1,widthp * heightp*3,fp);
  fread(data,1,widthp * heightp,fp);
  fclose(fp);
}

void loadPPMFile(char * filename , int * rows, int * cols, unsigned char *data) {

	const int readBufferSize = 20;
	ifstream ifs;
	char buffer[readBufferSize];
	ifs.open(filename, ifstream::in);

	ifs.getline(buffer, readBufferSize);
	ifs.getline(buffer, readBufferSize, ' ');
	 *cols  = atoi(buffer);
	ifs.getline(buffer, readBufferSize);
	 *rows = atoi(buffer);
	ifs.getline(buffer, readBufferSize);
	unsigned int bitDepth = atoi(buffer);
	int bytesPerPix = 0;
	bytesPerPix = (bitDepth == 255) ? 3 : 6;

	unsigned int imagePixs = *rows * *cols;
	unsigned int imageBytes = imagePixs * bytesPerPix;
	
	printf("loadPPMFile:%d , %d , %d, %d\n", bitDepth, bytesPerPix, *cols, *rows);
//	image = new char[imageBytes];
	unsigned int bytesPerLn = *cols * bytesPerPix;
	for (int i = 0; i < int(*rows); i++) {	
		char* tmp = new char[bytesPerLn];
		ifs.getline(tmp, bytesPerLn);
		strcat((char*)data, tmp);
	}
	
	ifs.close();

}


int main()
{
    int w = 0, h = 0;
    unsigned char outdata[1920*1080*3];
    string fileName = "./1.pgm";

   cv::Mat tmp = cv::imread(fileName.c_str());

   cv::imwrite("./bb.jpg", tmp);


    unsigned char outdata1[640*480*2];
    unsigned short outdata3[640*480];
   ppm_load(fileName, outdata1, &w, &h );

    unsigned char outdata2[640*480*1];
          printf("\n");
    
    unsigned short valueMax = 0;
    for(int i = 0; i < 640*480; i++)
    {
        unsigned short a = outdata1[i*2]<<8;
        unsigned short b = outdata1[i*2+1];
        outdata3[i] = (unsigned short)(outdata1[i*2]<<8) + outdata1[i*2+1];
       // outdata3[i] = a + b;
//        printf("%04d ", outdata3[i]);
        //printf("%04d %4d ", outdata1[2*i], outdata1[i*2+1]);
        //printf("%04d ", outdata1[2*i]);

        if(outdata1[i*2] > 3)
        {
            printf("%04d ", outdata1[2*i]);
        }

       // if((i+1)%16 == 0)
         // printf("\n");
        if(outdata3[i] > valueMax)
        {
          printf("%d, %d, %d, %d, %d , %d\n", outdata3[i], valueMax, outdata1[i*2],  outdata1[i*2+1], a, b);
          valueMax = outdata3[i];
        }
    }
    printf("valueMax:%d\n", valueMax);
    for(int i = 0; i < 640*480; i++)
    {
        outdata2[i] = outdata3[i]*255/valueMax;
    }
          printf("\n");

    cv::Mat src1(h, w, CV_8UC1, outdata2); 
    cv::imwrite("./aa.jpg", src1);

    cv::Mat org  = cv::imread("./20180730153613.bmp");
          printf("%d, %d %d\n", org.rows, org.cols, org.channels());

     cv::Mat org1;
    cv::cvtColor(org, org1, CV_BGR2GRAY);
     unsigned char *ptmpOr = org1.data;
     printf("%d, %d %d\n", org1.rows, org1.cols, org1.channels());
#if 0
     for(int i = 0; i < org1.rows*org1.cols* org1.channels(); i++, ptmpOr++)
     {
        printf("%04d ", *ptmpOr); 
        if((i+1)%16 == 0)
          printf("\n");

     }
#endif

   read_ppm_image("./1.ppm" ,  &h,  &w, outdata);

    cv::Mat src2(h, w, CV_8UC1, outdata); 
    cv::Mat src3(h, w, CV_8UC3); 

    cv::cvtColor(src2, src3, CV_GRAY2BGR);

    cv::imwrite("./cc.jpg", src3);

    loadPPMFile("./1.ppm" ,  &h,  &w, outdata);
    cv::Mat src4(h, w, CV_8UC1, outdata); 
    cv::imwrite("./cc.jpg", src4);

     

   return 0;
}
