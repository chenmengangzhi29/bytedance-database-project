#include "indexed_row_table.h"
#include <cstring>

namespace bytedance_db_project {
IndexedRowTable::IndexedRowTable(){}

IndexedRowTable::IndexedRowTable(int32_t index_column) {
  index_column_ = index_column;  
}

IndexedRowTable::~IndexedRowTable(){
  for(auto it : index_){
    if(it.second != nullptr){
      delete it.second;
      it.second = nullptr;
    }
  }
  if(rows_ != nullptr){
    delete rows_;
    rows_ = nullptr;
  }
}


void IndexedRowTable::Load(BaseDataLoader *loader) {
  // TODO: Implement this!
  num_cols_ = loader->GetNumCols();
  auto rows = loader->GetRows();
  num_rows_ = rows.size();
  rows_ = new char[FIXED_FIELD_LEN * num_rows_ * num_cols_];
  //建立行表
  for(int32_t row_id = 0; row_id < num_rows_; row_id++){
    auto cur_row = rows.at(row_id);
    std::memcpy(rows_ + row_id * (FIXED_FIELD_LEN * num_cols_), cur_row,
                FIXED_FIELD_LEN * num_cols_);
  }
  //对每一列建立空树
  for(int32_t col_id = 0; col_id < num_cols_; col_id++){
    index_[col_id] =  new RBTree();
  }
  //对每一列建立一棵红黑树
  for(int32_t col_id = 0; col_id < num_cols_; col_id++){
    for(int32_t row_id = 0; row_id < num_rows_; row_id++){
      int32_t offset = (row_id * num_cols_ + col_id) * FIXED_FIELD_LEN;
      int32_t field = *(int32_t *)(rows_ + offset);
      RBTreeNode* node = new RBTreeNode(field, row_id);
      index_[col_id]->rb_insert(node);
    }
  }
}

int32_t IndexedRowTable::GetIntField(int32_t row_id, int32_t col_id) {
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

void IndexedRowTable::PutIntField(int32_t row_id, int32_t col_id,
                                  int32_t field) {
  // TODO: Implement this!
  //判断是否越界
  if(row_id < 0 || row_id >= num_rows_ || col_id < 0 || col_id >= num_cols_){
    return;
  }
  //修改行表数据
  auto offset = (row_id * num_cols_ + col_id) * FIXED_FIELD_LEN;
  int32_t cur_field = *(int32_t *)(rows_ + offset);
  *(int32_t *)(rows_ + offset) = field;
  //修改红黑树数据
  RBTreeNode* cur_node = index_[col_id]->find_ptr(cur_field);
  index_[col_id]->rb_delete(cur_node, row_id);
  RBTreeNode* new_node = new RBTreeNode(field, row_id);
  index_[col_id]->rb_insert(new_node);
}

int64_t IndexedRowTable::ColumnSum() {
  // TODO: Implement this!
  int64_t sum = 0;
  //对第0列中的每个字段数据求和
  for(int32_t row_id = 0; row_id < num_rows_; row_id++){
    sum += *(int32_t *)(rows_ + row_id * num_cols_ * FIXED_FIELD_LEN);
  }
  //返回求和结果
  return sum;
}

int64_t IndexedRowTable::PredicatedColumnSum(int32_t threshold1,
                                             int32_t threshold2) {
  // TODO: Implement this!
  //求得的和保存在sum中
  int64_t sum = 0;
  //利用红黑树寻找第一列中比threshold1值大的所有行
  std::unordered_set<int32_t> row_ids_1 = index_[1]->find_greater(threshold1);
  //利用红黑树寻找第二列中比threshold2值小的所有行
  std::unordered_set<int32_t> row_ids_2 = index_[2]->find_less(threshold2);
  //对于既在row_ids_1中，又在row_ids_2中的行，将其第一列中值加起来
  for(auto row_id : row_ids_1){
    if(row_ids_2.count(row_id)){
      int32_t offset = row_id * num_cols_ * FIXED_FIELD_LEN;
      int32_t field = *(int32_t*)(rows_ + offset);
      sum += field;
    }
  }
  return sum;
}

int64_t IndexedRowTable::PredicatedAllColumnsSum(int32_t threshold) {
  // TODO: Implement this!
  int64_t sum = 0;
  std::unordered_set<int32_t> row_ids = index_[0]->find_greater(threshold);
  for(auto row_id : row_ids){
    for(int32_t col_id = 0; col_id < num_cols_; col_id++){
      int32_t offset = (row_id * num_cols_ + col_id) * FIXED_FIELD_LEN;
      int32_t field = *(int32_t*)(rows_ + offset);
      sum += field;
    }
  }
  return sum;
}

int64_t IndexedRowTable::PredicatedUpdate(int32_t threshold) {
  // TODO: Implement this!
  int64_t count = 0;
  std::unordered_set<int32_t> row_ids = index_[0]->find_less(threshold);
  for(int32_t row_id : row_ids){
    int32_t offset = row_id * num_cols_ * FIXED_FIELD_LEN;
    int32_t col2 = *(int32_t*)(rows_ + offset + 2 * FIXED_FIELD_LEN);
    *(int32_t*)(rows_ + offset + 3 * FIXED_FIELD_LEN) += col2;
    count ++;
  }
  return count;
}
} // namespace bytedance_db_project