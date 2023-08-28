#include <unistd.h>

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

// 默认参数

double match = 5;
double mis = 4;
double gap_open = 10;
double gap_extend = 0.5;

void dealArgv(int, char**, vector<fasta*>&);
void freeSeqList(vector<fasta*>&);

int main(int argc, char** argv) {
  // 序列数组
  vector<fasta*> seq_list;

  // 参数处理
  dealArgv(argc, argv, seq_list);

  // 序列比对
  if (seq_list.size() == 2) {
    Align obj(match, mis, gap_open, gap_extend, seq_list[0]->seq,
              seq_list[1]->seq);
    obj.print_score();
    obj.fill_Matrix();
    obj.print_score();
    obj.track_back();
    cout << endl << endl;

    // 序列数组内存释放
    freeSeqList(seq_list);
  } else {
    cout << "请输入有效序列" << endl;
  }

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

  // 先将argv拆开,拆成一个一个的,然后再对每一个进行getopt的解析
  int c;
  const char* opt_string = ":i:o:e:m:n:";
  while ((c = getopt(argc, argv, opt_string)) != -1) {
    switch (c) {
        // 指定输入文件,指定后打开
      case 'i':
        fileRead(optarg, seq_list);
        break;
        // gap open
      case 'o':
        gap_open = (double)atof(optarg);
        break;
      case 'e':
        gap_extend = (double)atof(optarg);
        break;
      // 指定匹配得分
      case 'm':
        match = (double)atof(optarg);
        break;
      // 指定mismatch罚分
      case 'n':
        mis = (double)atof(optarg);
        break;
      // 未指定输入文件
      case '?':
        cout << "无法识别的选项:\t-" << char(optopt) << endl;
        exit(1);
      case ':':
        cout << "选项-" << char(optopt) << "需参数" << endl;
        exit(1);
      default:
        break;
    }
  }
  if (seq_list.size() == 0) {
    int i = optind;
    for (i; i > 0 && i < argc; i++) {
      fasta* seq = new fasta(strlen(argv[i]));
      seq->id = new char;
      strcpy(seq->id, "#");
      seq->seq = new char[strlen(argv[i]) + 1];
      strcpy(seq->seq, argv[i]);
      seq->seq[strlen(argv[i])] = '\0';
      seq_list.push_back(seq);
    }
  }
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