#include "align.h"

#include <iostream>
/*
 *默认构造
 */
Align::Align() {
  this->match = 1;
  this->mismatch = -1;
  this->seq_col_len = -1;
  this->seq_row_len = -1;
  this->seq_col = nullptr;
  this->seq_row = nullptr;
  this->score_matrix = nullptr;
  this->matrix_col = 0;
  this->matrix_row = 0;
}

/**
 * param:位置匹配情况的分数,不匹配时的分数,插入空位的分数,序列A,序列B
 */
Align::Align(double p_match, double p_mis, double p_gap, char* seq_col,
             char* seq_row) {
  this->match = p_match;
  this->mismatch = p_mis;
  this->seq_col = seq_col;
  this->seq_row = seq_row;
  this->seq_col_len = strlen(seq_col);
  this->seq_row_len = strlen(seq_row);
  matrix_col = seq_col_len + 1;
  matrix_row = seq_row_len + 1;
  this->score_matrix = new double*[matrix_row];
  for (int i = 0; i < matrix_row; ++i) {
    *(score_matrix + i) = new double[matrix_col]{0};
  }
  for (int i = 1; i < matrix_row; ++i) {
    score_matrix[i][0] = score_matrix[i - 1][0] - 1;
  }
  for (int i = 1; i < matrix_col; ++i) {
    score_matrix[0][i] = score_matrix[0][i - 1] - 1;
  }
}

Align::Align(double p_match, double p_mis, double p_gap_open,
             double p_gap_extend, char* seq_col, char* seq_row) {
  this->match = p_match;
  this->mismatch = p_mis;
  this->gap_open = p_gap_open;
  this->gap_extend = p_gap_extend;
  this->seq_col = seq_col;
  this->seq_row = seq_row;
  this->seq_col_len = strlen(seq_col);
  this->seq_row_len = strlen(seq_row);
  this->get_row_col();
  matrix_col = seq_col_len + 1;
  matrix_row = seq_row_len + 1;
  this->score_matrix = new double*[matrix_row];
  for (int i = 0; i < matrix_row; ++i) {
    *(score_matrix + i) = new double[matrix_col]{0};
  }
  int row_gap_count = 0;
  for (int i = 1; i < matrix_row; ++i) {
    if (row_gap_count != 0) {
      score_matrix[i][0] = score_matrix[i - 1][0] - gap_extend;
    } else {
      score_matrix[i][0] = score_matrix[i - 1][0] - gap_open;
    }
    ++row_gap_count;
  }

  int col_gap_count = 0;
  for (int i = 1; i < matrix_col; ++i) {
    if (col_gap_count != 0) {
      score_matrix[0][i] = score_matrix[0][i - 1] - gap_extend;
    } else {
      score_matrix[0][i] = score_matrix[0][i - 1] - gap_open;
    }
    ++col_gap_count;
  }
}

/**
 * 填充得分矩阵
 */
void Align::fill_Matrix() {
  // 行gap类型的判断
  int gap_row_flag = 0;

  // 列gap类型
  char* col_gap_flag = new char[matrix_col];
  memset(col_gap_flag, 0, sizeof(char) * matrix_col);

  // 此处索引使用序列的索引,不是得分矩阵的索引,所以下面填充索引时会有+1的操作
  for (int row = 0; row < seq_row_len; ++row) {
    for (int col = 0; col < seq_col_len; ++col) {
      if (*(seq_row + row) == *(seq_col + col)) {
        score_matrix[row + 1][col + 1] = score_matrix[row][col] + match;
        gap_row_flag = 0;
        col_gap_flag[col + 1] = 0;
      } else {
        double mis = score_matrix[row][col] - this->mismatch;

        int res = -1;

        if (gap_row_flag == 0 && col_gap_flag[col + 1] == 0) {
          score_matrix[row + 1][col + 1] =
              max_score(mis, score_matrix[row + 1][col] - gap_open,
                        score_matrix[row][col + 1] - gap_open, res);
        } else if (gap_row_flag != 0 && col_gap_flag[col + 1] == 0) {
          score_matrix[row + 1][col + 1] =
              max_score(mis, score_matrix[row + 1][col] - gap_extend,
                        score_matrix[row][col + 1] - gap_open, res);
        } else if (col_gap_flag[col + 1] != 0 && gap_row_flag == 0) {
          score_matrix[row + 1][col + 1] =
              max_score(mis, score_matrix[row + 1][col] - gap_open,
                        score_matrix[row][col + 1] - gap_extend, res);
        } else {
          score_matrix[row + 1][col + 1] =
              max_score(mis, score_matrix[row + 1][col] - gap_extend,
                        score_matrix[row][col + 1] - gap_extend, res);
        }
        switch (res) {
          case 0:
            gap_row_flag = 0;
            col_gap_flag[col + 1] = 0;
            break;
          case 1:
            gap_row_flag = 1;
            col_gap_flag[col + 1] = 0;
            break;
          case 2:
            gap_row_flag = 0;
            col_gap_flag[col + 1] = 1;
            break;
          default:
            break;
        }

        res = -1;
      }
    }

    // 换行时，置行gap标志为0
    gap_row_flag = 0;
  }
  // 删除列gap标志
  delete col_gap_flag;
}

/**
 * 回溯得分矩阵,得到所有可能的结果
 */
void Align::recv(int row, int col) {
  // 参数中的索引使用得分矩阵的索引,在访问序列时,需要转换一下
  if (row == 0 && col == 0) {
    cout << endl << endl;
    for (vector<char>::iterator itr = stk_col.end() - 1; itr >= stk_col.begin();
         --itr) {
      cout << *itr;
    }
    cout << endl;
    for (vector<char>::iterator itr = stk_row.end() - 1; itr >= stk_row.begin();
         --itr) {
      cout << *itr;
    }
    return;
  }

  if (row >= 1 && col >= 1 &&
      (score_matrix[row][col] == score_matrix[row - 1][col - 1] + match) &&
      seq_col[col - 1] == seq_row[row - 1]) {
    stk_col.push_back(seq_col[col - 1]);
    stk_row.push_back(seq_row[row - 1]);
    recv(row - 1, col - 1);
    stk_col.pop_back();
    stk_row.pop_back();
  }
  bool row_tag = false;

  if (col == 1 &&
      (score_matrix[row][col] == score_matrix[row][col - 1] - gap_open)) {
    row_tag = true;

  } else if (col > 1) {
    if (score_matrix[row][col] == score_matrix[row][col - 1] - gap_open &&
        score_matrix[row][col - 1] != score_matrix[row][col - 2] - gap_open &&
        score_matrix[row][col - 1] != score_matrix[row][col - 2] - gap_extend) {
      // 当前为open,向前检测一个,看是否为gap_open
      // 为open
      row_tag = true;
    } else if (score_matrix[row][col] ==
                   score_matrix[row][col - 1] - gap_extend &&
               (score_matrix[row][col - 1] ==
                    score_matrix[row][col - 2] - gap_open ||
                score_matrix[row][col - 1] ==
                    score_matrix[row][col - 2] - gap_extend)) {
      row_tag = true;
    }
  }
  if (row_tag) {
    // 矩阵中从左边一个gap得到
    stk_row.push_back('-');
    stk_col.push_back(seq_col[col - 1]);
    recv(row, col - 1);
    stk_col.pop_back();
    stk_row.pop_back();
  }

  bool col_tag = false;
  if (row == 1 &&
      score_matrix[row][col] == score_matrix[row - 1][col] - gap_open) {
    col_tag = true;
  }

  // 需要向前检测满不满足当前gap情况
  else if (row > 1) {
    if (score_matrix[row][col] == score_matrix[row - 1][col] - gap_open &&
        score_matrix[row - 1][col] != score_matrix[row - 2][col] - gap_open &&
        score_matrix[row - 1][col] != score_matrix[row - 2][col] - gap_extend)
      col_tag = true;

    else if (score_matrix[row][col] ==
                 score_matrix[row - 1][col] - gap_extend &&
             (score_matrix[row - 1][col] ==
                  score_matrix[row - 2][col] - gap_open ||
              score_matrix[row - 1][col] ==
                  score_matrix[row - 2][col] - gap_extend))
      col_tag = true;
    else
      col_tag = false;
  }
  if (col_tag) {
    // 从上一行该列得到
    stk_row.push_back(seq_row[row - 1]);
    stk_col.push_back('-');
    recv(row - 1, col);
    stk_col.pop_back();
    stk_row.pop_back();
  }

  if (row > 0 && col > 0 &&
      score_matrix[row][col] == score_matrix[row - 1][col - 1] - mismatch) {
    // mismatch
    stk_col.push_back(seq_col[col - 1]);
    stk_row.push_back(seq_row[row - 1]);
    recv(row - 1, col - 1);
    stk_col.pop_back();
    stk_row.pop_back();
  }
  // }
}

void Align::track_back() { recv(seq_row_len, seq_col_len); }

/**
 *
 * 打印分数矩阵
 */
void Align::print_score() {
  for (int i = 0; i < matrix_row; ++i) {
    for (int j = 0; j < matrix_col; ++j) {
      cout << score_matrix[i][j] << "\t";
    }
    cout << endl;
  }
  cout << endl;
}

/**
 * 析构
 */
Align::~Align() {
  if (score_matrix != nullptr) {
    for (int i = 0; i < matrix_row; ++i) {
      delete[] *(score_matrix + i);
    }
    delete[] score_matrix;
  }
}