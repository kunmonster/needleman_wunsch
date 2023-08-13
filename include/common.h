
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef struct FASTA {
  char* id;
  char* seq;
  int len;

  FASTA(int p_len) : len(p_len){};
} fasta;

// 读取文件下,参数为文件路径
void fileRead(const char*, vector<fasta*>&);