/*
 * filestorage_sample demonstrate the usage of the opencv serialization functionality
 */

#include "opencv2/core.hpp"
#include <iostream>
#include <string>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::ostream;
using namespace cv;
using namespace std;

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
    FileStorage fs(filename, FileStorage::WRITE);

    ifstream fin;
    string tmp;
    fin.open(path.c_str());

    cout << "writing images\n";
    int i = 0;
    vector<string> FileVec;

    while(fin >> tmp)
    {
#if 0
        stringstream tmpStr;
        tmpStr<<"image"<<i;
        cout  <<tmpStr.str()<<endl;
        fs<<tmpStr.str()<<tmp;
        i++;
#endif
        FileVec.push_back(tmp);
    }
    fs <<"image"<<"[";

    vector<string>::iterator strIt;
    for(strIt = FileVec.begin();strIt != FileVec.end(); strIt++ )
    {
        fs<<*strIt;
    }
    fs<<"]";


    fs.release();
    fin.close();

  }

  //read
  {
    FileStorage fs(filename, FileStorage::READ);

    if (!fs.isOpened())
    {
      cerr << "failed to open " << filename << endl;
      return 1;
    }

    FileNode n = fs.getFirstTopLevelNode();
#if 1 
    if (n.type() != FileNode::SEQ)
    {
      cerr << "images is not a sequence! FAIL" << n.type()  << endl;
      return 1;
    }
#endif
    cout << "reading images\n";
    FileNodeIterator it = n.begin(), it_end = n.end();
    for (; it != it_end; ++it)
    {
      cout << (string)*it << "\n";
    }
    
    fs.release();

  }


  return 0;
}
