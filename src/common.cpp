#include "common.h"

#include <cstring>
#include <fstream>
#include <regex>
#include <string>
void fileRead(const char* file_path, vector<fasta*>& seq_list) {
  std::cout << file_path << endl;
  ifstream fin;
  fin.open(file_path, ios::in);
  if (!fin.is_open()) {
    std::cerr << "文件打开失败,请检查后重试!";
    exit(0);
  }

  char c;

  string header;
  string seq_detail;

  while ((c = fin.get()) != EOF) {
    if (c == '\n') {
      continue;
    } else if (c == '>') {
      // 新开了一个header
      // 此时需要创建一个fasta struct然后将读取的信息写入进这个结构体
      // 开一个循环读到换行符为止
      // 先将上面的seq和header写入
      if (!seq_detail.empty() && !header.empty()) {
        fasta* seq = new fasta(seq_detail.length());
        seq->id = new char[header.size() + 1];
        seq->seq = new char[seq_detail.size() + 1];
        // seq->len = seq_detail.length();

        strcpy(seq->id, header.c_str());
        strcpy(seq->seq, seq_detail.c_str());
        seq_list.push_back(seq);
        header.clear();
        seq_detail.clear();
        // continue;
      }
      char z;
      while ((z = fin.get()) != '\n') {
        header.push_back(z);
      }

    } else if (c != 'A' && c != 'C' && c != 'G' && c != 'T' && c != 'a' &&
               c != 'c' && c != 'g' && c != 't') {
      std::cerr << "文件中包含非序列字符,请检查序列" << endl;
      exit(0);
    } else {
      // 读取到正常序列,添加进序列string就行
      c = toupper(c);
      seq_detail.push_back(c);
    }
  }
  // 只有一个序列的情况
  if (!seq_detail.empty() && !header.empty()) {
    fasta* seq = new fasta(seq_detail.length());
    seq->id = new char[header.size() + 1];
    seq->seq = new char[seq_detail.size() + 1];
    strcpy(seq->id, header.c_str());
    strcpy(seq->seq, seq_detail.c_str());
    seq_list.push_back(seq);
    header.clear();
    seq_detail.clear();
  }
  if (seq_list.size() < 2) {
    std::cerr << "请确保文件至少有两条序列" << endl;
    exit(0);
  }
  // for (int i = 0; i < seq_list.size(); ++i) {
  //   cout << "序列名" << seq_list[i]->id << endl;
  //   cout << "序列" << seq_list[i]->seq << endl;
  //   cout << "序列长度" << seq_list[i]->len << endl;
  // }
}
