#include "column_table.h"
#include <cstring>
#include <iostream>

namespace bytedance_db_project {
ColumnTable::ColumnTable() {}

ColumnTable::~ColumnTable() {
  if (columns_ != nullptr) {
    delete columns_;
    columns_ = nullptr;
  }
}

//
// columnTable, which stores data in column-major format.
// That is, data is laid out like
//   col 1 | col 2 | ... | col m.
//
void ColumnTable::Load(BaseDataLoader *loader) {
  //总行数和总列数是存储前的
  num_cols_ = loader->GetNumCols();
  std::vector<char *> rows = loader->GetRows();
  num_rows_ = rows.size();
  columns_ = new char[FIXED_FIELD_LEN * num_rows_ * num_cols_];

  for (int32_t row_id = 0; row_id < num_rows_; row_id++) {
    auto cur_row = rows.at(row_id);
    for (int32_t col_id = 0; col_id < num_cols_; col_id++) {
      int32_t offset = FIXED_FIELD_LEN * ((col_id * num_rows_) + row_id);
      std::memcpy(columns_ + offset, cur_row + FIXED_FIELD_LEN * col_id,
                  FIXED_FIELD_LEN);
    }
  }
}

int32_t ColumnTable::GetIntField(int32_t row_id, int32_t col_id) {
  // TODO: Implement this!
  //判断是否越界
  if(row_id < 0 || row_id >= num_rows_ || col_id < 0 || col_id >= num_cols_){
    return -1;
  }
  // //取出数据
  // char *cur_field = nullptr;
  // cur_field = (char *)malloc(FIXED_FIELD_LEN);
  // std::memcpy(cur_field, columns_ + (col_id * num_rows_ + row_id) * FIXED_FIELD_LEN,
  //            FIXED_FIELD_LEN);
  // int32_t field = (*(int32_t *)cur_field);
  // free(cur_field);
  int32_t offset = (col_id * num_rows_ +row_id) * FIXED_FIELD_LEN;
  int32_t field = *(int32_t*)(columns_ + offset);
  //返回数据
  return field;
}

void ColumnTable::PutIntField(int32_t row_id, int32_t col_id, int32_t field) {
  // TODO: Implement this!
  //判断是否越界
  if(row_id < 0 || row_id >= num_rows_ || col_id < 0 || col_id >= num_cols_){
    return;
  }
  //修改数据
  int32_t offset = (col_id * num_rows_ + row_id) * FIXED_FIELD_LEN;
  *(int32_t *)(columns_ + offset) = field;
}

int64_t ColumnTable::ColumnSum() {
  // TODO: Implement this!
  int64_t sum = 0;
  //对第0列的所有字段数据求和
  for(auto row_id = 0; row_id < num_rows_; row_id++){
    sum += *(int32_t *)(columns_ + row_id * FIXED_FIELD_LEN);
  }
  //返回求和结果
  return sum;
}

int64_t ColumnTable::PredicatedColumnSum(int32_t threshold1,
                                         int32_t threshold2) {
  // TODO: Implement this!
  int64_t sum = 0;
  for(int32_t row_id = 0; row_id < num_rows_; row_id++){
    char* loc = columns_ + row_id * FIXED_FIELD_LEN;
    int32_t col0 = *(int32_t*)loc;
    int32_t col1 = *(int32_t*)(loc + 1 * num_rows_ * FIXED_FIELD_LEN);
    int32_t col2 = *(int32_t*)(loc + 2 * num_rows_ * FIXED_FIELD_LEN);
    if(col1 > threshold1 && col2 < threshold2){
      sum += col0;
    }
  }
  return sum;
}

int64_t ColumnTable::PredicatedAllColumnsSum(int32_t threshold) {
  // TODO: Implement this!
  int64_t sum = 0;
  for(int32_t row_id = 0; row_id < num_rows_; row_id++){
    char* loc = columns_ + row_id * FIXED_FIELD_LEN;
    int32_t col0 = *(int32_t*)loc;
    if(col0 > threshold){
      for(int32_t col_id = 0; col_id < num_cols_; col_id++){
        int32_t col = *(int32_t*)(loc + col_id * num_rows_ * FIXED_FIELD_LEN);
        sum += col;
      }
    }
  }
  return sum;
}

int64_t ColumnTable::PredicatedUpdate(int32_t threshold) {
  // TODO: Implement this!
  int64_t count = 0;
  for(int32_t row_id = 0; row_id < num_rows_; row_id++){
    char* loc = columns_ + row_id * FIXED_FIELD_LEN;
    int32_t col0 = *(int32_t*)loc;
    if(col0 < threshold){
      int32_t col2 = *(int32_t*)(loc + 2 * num_rows_ * FIXED_FIELD_LEN);
      *(int32_t*)(loc + 3 * num_rows_ * FIXED_FIELD_LEN) += col2;
      count ++;
    }
  }
  return count;
}
} // namespace bytedance_db_project