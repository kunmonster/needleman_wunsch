#include <getopt.h>

#include <iostream>
#include <vector>

#include "align.h"
#include "common.h"
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
  // dealArgv(argc, argv);
  vector<fasta*> seq_list;
  fileRead(argv[1], seq_list);

  Align obj(1, 1, 1, 1, seq_list[0]->seq, seq_list[1]->seq);
  obj.print_score();
  obj.fill_Matrix();
  obj.print_score();
  obj.track_back();
  cout << endl << endl;

  for (vector<fasta*>::iterator itr = seq_list.begin(); itr < seq_list.end();
       ++itr) {
    if (NULL != *itr) {
      delete (*itr)->id;
      delete (*itr)->seq;
      delete *itr;
    }
  }
  seq_list.clear();
  cout << seq_list.size();
  return 0;
}

void dealArgv(int argc, char** argv) {
  if (argc <= 1) {
    // 输出提示
    cout << "Usage: align [OPTION]  [FILE | SEQ]" << endl;
    cout << "Align two sequences and track back the alginment result with "
            "multiple "
            "option(match,mismatch,gap-open,gap-extending)"
         << endl;
  }
  exit(0);
}