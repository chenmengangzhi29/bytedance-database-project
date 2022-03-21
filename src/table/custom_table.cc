#include "custom_table.h"
#include <cstring>

namespace bytedance_db_project {
CustomTable::CustomTable() {}

CustomTable::~CustomTable() {}

void CustomTable::Load(BaseDataLoader *loader) {
  // TODO: Implement this!
  int32_t num_cols__ = loader->GetNumCols();
  auto rows_ = loader->GetRows();
  int32_t num_rows__ = rows_.size();
  if(num_cols__ > 2 * num_rows__){
    ct.Load(loader);
    Label = ColumnTable_;
  }else{
    it.Load(loader);
    Label = IndexedRowTable_;
  }
}

int32_t CustomTable::GetIntField(int32_t row_id, int32_t col_id) {
  // TODO: Implement this!
  if(Label == ColumnTable_){
    return ct.GetIntField(row_id, col_id);
  }else{
    return it.GetIntField(row_id, col_id);
  }
}

void CustomTable::PutIntField(int32_t row_id, int32_t col_id, int32_t field) {
  // TODO: Implement this!
  if(Label == ColumnTable_){
    return ct.PutIntField(row_id, col_id, field);
  }else{
    return it.PutIntField(row_id, col_id, field);
  }
}

int64_t CustomTable::ColumnSum() {
  // TODO: Implement this!
  if(Label == ColumnTable_){
    return ct.ColumnSum();
  }else{
    return it.ColumnSum();
  }
}

int64_t CustomTable::PredicatedColumnSum(int32_t threshold1,
                                         int32_t threshold2) {
  // TODO: Implement this!
  if(Label == ColumnTable_){
    return ct.PredicatedColumnSum(threshold1, threshold2);
  }else{
    return it.PredicatedColumnSum(threshold1, threshold2);
  }
}

int64_t CustomTable::PredicatedAllColumnsSum(int32_t threshold) {
  // TODO: Implement this!
  if(Label == ColumnTable_){
    return ct.PredicatedAllColumnsSum(threshold);
  }else{
    return it.PredicatedAllColumnsSum(threshold);
  }
}

int64_t CustomTable::PredicatedUpdate(int32_t threshold) {
  // TODO: Implement this!
  if(Label == ColumnTable_){
    return ct.PredicatedUpdate(threshold);
  }else{
    return it.PredicatedUpdate(threshold);
  }
}
} // namespace bytedance_db_project