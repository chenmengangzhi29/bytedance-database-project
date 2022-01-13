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
  if(row_id < 0 || row_id >= num_rows_ || col_id < 0 || col_id >= num_cols_){
    return -1;
  }
  char *cur_field = nullptr;
  cur_field = (char *)malloc(FIXED_FIELD_LEN);
  std::memcpy(cur_field, columns_ + (col_id * num_rows_ + row_id) * FIXED_FIELD_LEN,
             FIXED_FIELD_LEN);
  int32_t field = (*(int32_t *)cur_field);
  free(cur_field);
  return field;
}

void ColumnTable::PutIntField(int32_t row_id, int32_t col_id, int32_t field) {
  // TODO: Implement this!
  if(row_id < 0 || row_id >= num_rows_ || col_id < 0 || col_id >= num_cols_){
    return;
  }
  auto offset = (col_id * num_rows_ + row_id) * FIXED_FIELD_LEN;
  *(int32_t *)(columns_ + offset) = field;
}

int64_t ColumnTable::ColumnSum() {
  // TODO: Implement this!
  int64_t sum = 0;
  for(auto row_id = 0; row_id < num_rows_; row_id++){
    sum += *(int32_t *)(columns_ + row_id * FIXED_FIELD_LEN);
  }
  return sum;
}

int64_t ColumnTable::PredicatedColumnSum(int32_t threshold1,
                                         int32_t threshold2) {
  // TODO: Implement this!
  return 0;
}

int64_t ColumnTable::PredicatedAllColumnsSum(int32_t threshold) {
  // TODO: Implement this!
  return 0;
}

int64_t ColumnTable::PredicatedUpdate(int32_t threshold) {
  // TODO: Implement this!
  return 0;
}
} // namespace bytedance_db_project