#include <iostream>
#include <fstream>
using namespace std;

int main(){
  string line;
  ifstream file("borat");
  while(!file.eof()){
    getline(file, line, '\n');
    cout << line << endl;
  }
  return 0;
}
