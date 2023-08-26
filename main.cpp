#include <getopt.h>

#include <cstring>
#include <iostream>
#include <vector>

#include "align.h"
#include "common.h"
using namespace std;

/***
 * 替换矩阵,BLAST,symmetric matrix
 *
 * [-  A  T  C  G]
 * [A  5 -4 -4 -4]
 * [T -4  5 -4 -4]
 * [C -4 -4  5 -4]
 * [G -4 -4 -4  5]
 * **/

int match = 5;
int mis = 4;
int gap_open = 10;
int gap_extend = 0.5;

void dealArgv(int, char**, vector<fasta*>&);
void freeSeqList(vector<fasta*>&);

int main(int argc, char** argv) {
  // 序列数组
  vector<fasta*> seq_list;

  // 参数处理
  dealArgv(argc, argv, seq_list);

  // 序列比对
  Align obj(5, 4, 10, 0.5, seq_list[0]->seq, seq_list[1]->seq);
  obj.print_score();
  obj.fill_Matrix();
  obj.print_score();
  obj.track_back();
  cout << endl << endl;

  // 序列数组内存释放
  freeSeqList(seq_list);

  return 0;
}

void dealArgv(int argc, char** argv, vector<fasta*>& seq_list) {
  if (argc <= 1) {
    // 输出提示
    cout << "Usage: align [OPTION]  [FILE | SEQ]" << endl;
    cout << "Align two sequences and track back the alginment result with "
            "multiple "
            "option(match,mismatch,gap-open,gap-extending)"
         << endl;
  }

  /**
   * i:输入文件
   * o:gap open
   * e:gap extend
   */
  int o;
  const char* opt_string = "i:o:e:";
  while ((o = getopt(argc, argv, opt_string)) != -1) {
    switch (o) {
      case 'i':
        // 指定输入文件,指定后打开
        fileRead(optarg, seq_list);
        break;
      case 'o':
        // gap open
        int gap_open = atoi(optarg);
        break;
      case 'e':
        int gap_extend = atoi(optarg);
        break;

      default:
        break;
    }
  }

  // exit(0);
}

void freeSeqList(vector<fasta*>& seq_list) {
  for (vector<fasta*>::iterator itr = seq_list.begin(); itr < seq_list.end();
       ++itr) {
    if (NULL != *itr) {
      delete (*itr)->id;
      delete (*itr)->seq;
      delete *itr;
    }
  }
  seq_list.clear();
}