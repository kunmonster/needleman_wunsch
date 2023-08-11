#ifndef _ALIGN_H_
#define _ALIGN_H_
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

class Align {
 private:
  // 这里的分数均采用正数,只表示得分或者罚分的多少

  int match;     // 匹配时得分
  int mismatch;  // 选择不匹配时罚分
  int gap;       // 空位罚分

  int gap_open;    // 最开始的空位
  int gap_extend;  // 中间的空位

  int seq_col_len;  // 序列长度
  int seq_row_len;

  int matrix_row;  // 得分矩阵的行数,与纵坐标序列关联
  int matrix_col;  // 得分矩阵的列数,与横坐标的序列关联

  char* seq_col;
  char* seq_row;

  int** score_matrix;  // 得分矩阵

  vector<char> stk_col;
  vector<char> stk_row;

 public:
  Align();
  explicit Align(int p_match, int p_mis, int p_gap, char* seq_col,
                 char* seq_row);
  explicit Align(int p_match, int p_mis, int p_gap_open, int p_gap_extend,
                 char* seq_col, char* seq_row);

  ~Align();

  void fill_Matrix();
  void recv(int row, int col);
  void track_back();
  void print_score();

  // 用于分数大小比较
  inline int max_score(const int& a, const int& b, const int& c, int& res) {
    int temp = INT32_MIN;
    temp = max(a, b);
    temp = c > temp ? c : temp;
    if (temp == a)
      res = 0;
    else if (temp == b)
      res = 1;
    else
      res = 2;
    return temp;
  }

  // 在给定序列后,将序列的行列重组，确保列是较短序列
  inline void get_row_col() {
    if (this->seq_col_len > this->seq_row_len) {
      this->swap_ptr(this->seq_col, this->seq_row);
    }
  }

  // 交换行列序列指针
  inline void swap_ptr(char*& a, char*& b) {
    char* temp = a;
    a = b;
    b = temp;
    int len_tmp = seq_col_len;
    seq_col_len = seq_row_len;
    seq_row_len = len_tmp;
  }
};

#endif
