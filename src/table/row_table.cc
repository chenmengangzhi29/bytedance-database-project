#include "row_table.h"
#include <cstring>

namespace bytedance_db_project {
RowTable::RowTable() {}

RowTable::~RowTable() {
  if (rows_ != nullptr) {
    delete rows_;
    rows_ = nullptr;
  }
}

void RowTable::Load(BaseDataLoader *loader) {
  num_cols_ = loader->GetNumCols();
  auto rows = loader->GetRows();
  num_rows_ = rows.size();
  rows_ = new char[FIXED_FIELD_LEN * num_rows_ * num_cols_];
  for (auto row_id = 0; row_id < num_rows_; row_id++) {
    auto cur_row = rows.at(row_id);
    std::memcpy(rows_ + row_id * (FIXED_FIELD_LEN * num_cols_), cur_row,
                FIXED_FIELD_LEN * num_cols_);
  }
}

int32_t RowTable::GetIntField(int32_t row_id, int32_t col_id) {
  // TODO: Implement this!
  //判断是否越界
  if(row_id < 0 || row_id >= num_rows_ || col_id < 0 || col_id >= num_cols_){
    return -1;
  }
  //取出数据
  // char *cur_field = nullptr;
  // cur_field = (char *)malloc(FIXED_FIELD_LEN);
  // std::memcpy(cur_field, rows_ + (row_id * num_cols_ + col_id) * FIXED_FIELD_LEN,
  //             FIXED_FIELD_LEN);
  // int32_t field = (*(int32_t *)cur_field);
  // free(cur_field);
  //返回数据
  auto offset = (row_id * num_cols_ + col_id) * FIXED_FIELD_LEN;
  return *(int32_t *)(rows_ + offset);
}

void RowTable::PutIntField(int32_t row_id, int32_t col_id, int32_t field) {
  // TODO: Implement this!
  //判断是否越界
  if(row_id < 0 || row_id >= num_rows_ || col_id < 0 || col_id >= num_cols_){
    return;
  }
  //修改数据
  auto offset = (row_id * num_cols_ + col_id) * FIXED_FIELD_LEN;
  *(int32_t *)(rows_ + offset) = field;
}

int64_t RowTable::ColumnSum() {
  // TODO: Implement this!
  int64_t sum = 0;
  //对第0列中的每个字段数据求和
  for(int32_t row_id = 0; row_id < num_rows_; row_id++){
    sum += *(int32_t *)(rows_ + row_id * num_cols_ * FIXED_FIELD_LEN);
  }
  //返回求和结果
  return sum;
}

int64_t RowTable::PredicatedColumnSum(int32_t threshold1, int32_t threshold2) {
  // TODO: Implement this!
  //求得的和保存在sum中
  int64_t sum = 0;
  //遍历每一行，将满足条件的行的第一列中的数加到sum中
  for(int32_t row_id = 0; row_id < num_rows_; row_id ++){
    char* loc = rows_ + row_id * num_cols_ * FIXED_FIELD_LEN;
    int32_t col0 = *(int32_t *)(loc);
    int32_t col1 = *(int32_t *)(loc + 1 * FIXED_FIELD_LEN);
    int32_t col2 = *(int32_t *)(loc + 2 * FIXED_FIELD_LEN);
    if(col1 > threshold1 && col2 < threshold2){
      sum += col0;
    }
  }
  return sum;
}

int64_t RowTable::PredicatedAllColumnsSum(int32_t threshold) {
  // TODO: Implement this!
  int64_t sum = 0;
  //遍历每一行，找出满足条件的行，将该行中所有列中的数加到sum中
  for(int32_t row_id = 0; row_id < num_rows_; row_id ++){
    char* loc = rows_ + row_id * num_cols_ * FIXED_FIELD_LEN;
    int32_t col0 = *(int32_t *)(loc);
    if(col0 > threshold){
      for(int32_t col_id = 0; col_id < num_cols_; col_id++){
        int32_t col = *(int32_t *)(loc + col_id * FIXED_FIELD_LEN);
        sum += col;
      }
    }
  }
  return sum;
}

int64_t RowTable::PredicatedUpdate(int32_t threshold) {
  // TODO: Implement this!
  int64_t count = 0;
  //遍历每一行，对满足条件的行进行更新，然后计数值加一
  for(int32_t row_id = 0; row_id < num_rows_; row_id++){
    char* loc = rows_ + row_id * num_cols_ * FIXED_FIELD_LEN;
    int32_t col0 = *(int32_t *)(loc);
    if(col0 < threshold){
      int32_t col2 = *(int32_t *)(loc + 2 * FIXED_FIELD_LEN);
      *(int32_t *)(loc + 3 * FIXED_FIELD_LEN) += col2;
      count ++;
    }
  }
  return count;
}
} // namespace bytedance_db_project