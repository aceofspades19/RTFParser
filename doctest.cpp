#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
using namespace std;

unsigned short read_16(FILE* file, int place){
  unsigned short x=0;
  void *data = new char[1];
  void *data2 = new char[1];
  unsigned char* cdata1=0;
  unsigned char* cdata2=0;
  char size;
  fseek(file, place, SEEK_SET);
  fread(data, sizeof(size), 1, file);
  fseek(file, (++place), SEEK_SET);
  fread(data2, sizeof(size), 1, file);
  cdata1 = reinterpret_cast<unsigned char*>(data);
  cdata2 = reinterpret_cast<unsigned char*>(data2);
  x = *cdata2;
  x = x << 8;
  x += *cdata1;
  return x;
}

int main()
{
  unsigned short x=0;
  unsigned short x2 =0;
  unsigned long x3=0;
  //void data = new char[10];
  FILE *fp;
  fp=fopen("test.doc", "r");
  if(fp != 0){
    x = read_16(fp, 0x18);
    x2 = read_16(fp, 0x20);
    cout << x << endl;
    cout << x2 << endl;
    x3 = x2;
    x3 = x3 << 16;
    x3 += x;
    cout << x3 << endl;
    //fseek(fp, x3, SEEK_SET);
    //fread(
    fclose(fp);
  }
  else {
    cout << "FAIL";
  }
  
  return 0;
}
