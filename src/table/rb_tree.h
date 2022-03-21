#pragma once
#include "table.h"
#include <unordered_set>
#include <queue>

namespace bytedance_db_project{
// typedef int int32_t;
typedef bool RBColorType;
const RBColorType red = false;
const RBColorType black = true;

class RBTreeNode{
public:
  explicit RBTreeNode(int32_t value):value_(value){};

  RBTreeNode(int32_t value, int32_t row_id):value_(value){
    row_ids_.insert(row_id);
  };

  RBColorType color{red};//节点颜色，默认为红
  int32_t value_;//值
  std::unordered_set<int32_t> row_ids_;//所在行数     
  RBTreeNode* parent{nullptr};
  RBTreeNode* left{nullptr};
  RBTreeNode* right{nullptr};
};
class RBTree{
public:
  //构造函数
  RBTree();
  //析构函数
  ~RBTree();

  //左旋
  void left_rotate(RBTreeNode* node);

  //右旋
  void right_rotate(RBTreeNode* node);

  //找出树中值最小的结点
  RBTreeNode* rb_minimum(RBTreeNode* cur);

  //找出树中值最大的结点
  RBTreeNode* rb_maximum(RBTreeNode* cur);

  //寻找后继结点
  RBTreeNode* rb_successor(RBTreeNode* node);

  //插入结点后的颜色修复与结点调整
  void rb_insert_fixup(RBTreeNode* node);

  //插入新结点
  void rb_insert(RBTreeNode* node);

  //删除结点后的颜色修复与结点调整
  void rb_delete_fixup(RBTreeNode* node);

  //删除结点
  RBTreeNode* rb_delete(RBTreeNode* node, int32_t row_id);

  //寻找树中是否有一个值，有则返回指向它的指针，没有则返回nil_；
  RBTreeNode* find_ptr(int32_t value);

  //寻找一个值所在的行。
  std::unordered_set<int32_t> find_row(int32_t value);

  //给出一个值，寻找大于该值的所有行
  std::unordered_set<int32_t> find_greater(int32_t value);

  //给出一个值，寻找小于该值的所有行
  std::unordered_set<int32_t> find_less(int32_t value);

private:
  //根结点指针
  RBTreeNode* root_;

  //哨兵，所有指向null的指针都被替换成指向哨兵nil的指针。
  //作用是便于处理红黑树代码中的边界条件
  RBTreeNode* nil_;
};
}//bytedance_db_project