#include <iostream>

#include "include/align.h"
using namespace std;

/***
 * 替换矩阵,BLAST,symmetric matrix
 *
 *
 * [-  A  T  C  G]
 * [A  5 -4 -4 -4]
 * [T -4  5 -4 -4]
 * [C -4 -4  5 -4]
 * [G -4 -4 -4  5]
 * **/

void dealArgv(int, char**);

int main(int argc, char** argv) {
  char* seqA;
  char* seqB;
  // dealArgv(argc, argv);
  char seqb[] = {'A','G','C','C','C','T','T', '\0'};
  char seqa[] = {'A','C','C','T','T', '\0'};
  Align obj(5, 4, 10, 0.5, seqb, seqa);
  obj.print_score();
  obj.fill_Matrix();
  obj.print_score();
  obj.track_back();
  cout << endl << endl;
  return 0;
}

void dealArgv(int argc, char** argv) {
  if (argc <= 1) {
    // 输出提示
    cout << "Usage: align [OPTIONS] [FILE | SEQ_REF] [FILE | SEQ]" << endl;
  }
}