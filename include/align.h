#ifndef ALIGN_H
#define ALIGN_H
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

class Align {
 private:
  // 这里的分数均采用正数,只表示得分或者罚分的多少

  double match;     // 匹配时得分
  double mismatch;  // 选择不匹配时罚分

  double gap_open;    // 第一个gap罚分  
  double gap_extend;  //

  int seq_col_len;  // 序列长度
  int seq_row_len;

  int matrix_row;  // 得分矩阵的行数,与纵坐标序列关联
  int matrix_col;  // 得分矩阵的列数,与横坐标的序列关联

  char* seq_col;
  char* seq_row;

  double** score_matrix;  // 得分矩阵

  int same_residence;

  vector<char> stk_col;
  vector<char> stk_row;

 public:
  Align();
  explicit Align(double p_match, double p_mis, double p_gap, char* seq_col,
                 char* seq_row);
  explicit Align(double p_match, double p_mis, double p_gap_open,
                 double p_gap_extend, char* seq_col, char* seq_row);

  ~Align();

  void fill_Matrix();
  void recv(int row, int col);
  void track_back();
  void print_score();

  // 用于分数大小比较
  inline double max_score(const double& a, const double& b, const double& c,
                          int& res) {
    double temp = INT32_MIN;
    temp = max(a, b);
    temp = c > temp ? c : temp;

    // 涉及到策略选择，当三种或者其中两种相同时，返回值的策略

    if (temp == b)
      res = 1;
    else if (temp == c)
      res = 2;
    else
      res = 0;
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
