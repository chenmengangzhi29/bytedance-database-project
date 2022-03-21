#include "rb_tree.h"

namespace bytedance_db_project{

//构造函数
RBTree::RBTree(){
    nil_ = new RBTreeNode(0);
    nil_->color = black;
    nil_->parent = nil_;
    nil_->left = nil_;
    nil_->right = nil_;
    root_ = nil_;
}

//析构函数
RBTree::~RBTree(){
    if(root_ != nullptr){
        delete root_;
        root_ = nullptr;
    }
    if(nil_ != nullptr){
        delete nil_;
        nil_ = nullptr;
    }
}

//左旋
void RBTree::left_rotate(RBTreeNode* node){
    //1.设置RightChild指针
    RBTreeNode* RightChild = node->right;
    //2.将RightChild的左子树作为node的右子树
    node->right = RightChild->left;
    RightChild->left->parent = node;
    //3.将node的父节点与RightChild连接起来
    RightChild->parent = node->parent;
    if(node->parent == nil_){
        root_ = RightChild;
    }else if(node == node->parent->left){
        node->parent->left = RightChild;
    }else{
        node->parent->right = RightChild;
    }
    //4.把node节点作为RightChild节点的左子树
    RightChild->left = node;
    node->parent = RightChild;
}

//右旋
void RBTree::right_rotate(RBTreeNode* node){
    //1.设置LeftChild指针
    RBTreeNode* LeftChild = node->left;
    //2.将LeftChild的右子树作为node的左子树
    node->left = LeftChild->right;
    LeftChild->right->parent = node;
    //3.将node的父节点与LeftChild连接起来
    LeftChild->parent = node->parent;
    if(node->parent == nil_){
        root_ = LeftChild;
    }else if(node == node->parent->left){
        node->parent->left = LeftChild;
    }else{
        node->parent->right = LeftChild;
    }
    //4.将node节点作为LeftChild的右子树
    LeftChild->right = node;
    node->parent = LeftChild;
}

//找出树中值最小的结点
RBTreeNode* RBTree::rb_minimum(RBTreeNode* cur){
    while(cur->left != nil_){
        cur = cur->left;
    }
    return cur;
}

//找出树中值最大的结点
RBTreeNode* RBTree::rb_maximum(RBTreeNode* cur){
    while(cur->right != nil_){
        cur = cur->right;
    }
    return cur;
}

//寻找后继结点
RBTreeNode* RBTree::rb_successor(RBTreeNode* node){
    if(node->right != nil_)
        return rb_minimum(node->right);
    RBTreeNode* parent = node->parent;
    while(parent != nil_ && node == parent->right){
        node = parent;
        parent = parent->parent;
    } 
    return parent;
}

//插入结点后的颜色修复与结点调整
void RBTree::rb_insert_fixup(RBTreeNode* node){
    RBTreeNode* uncle = nil_;
    while(node->parent != nil_ && node->parent->color == red){
        if(node->parent == node->parent->parent->left){
            uncle = node->parent->parent->right;
            if(uncle != nil_ && uncle->color == red){
                node->parent->color = black;
                uncle->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }else{
                if(node == node->parent->right){
                    node = node->parent;
                    left_rotate(node);
                }
                node->parent->color = black;
                node->parent->parent->color = red;
                right_rotate(node->parent->parent);
            }
        }
        else{ // if node.parent == node->parent->parent.right
            uncle = node->parent->parent->left;
            if(uncle != nil_ && uncle->color == red){
                node->parent->color = black;
                uncle->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            }else{
                if(node == node->parent->left){
                    node = node->parent;
                    right_rotate(node);
                }
                node->parent->color = black;
                node->parent->parent->color = red;
                left_rotate(node->parent->parent);
            }
        }
    }
    root_->color = black;
}

//插入新结点
void RBTree::rb_insert(RBTreeNode* node){
    RBTreeNode* parent = nil_;
    RBTreeNode* current = root_;
    while(current != nil_){
        parent = current;
        if(node->value_ < current->value_){
            current = current->left;
        }else{
            current = current->right;
        }
    }
    if(parent != nil_ && parent->value_ == node->value_){
        parent->row_ids_.insert(node->row_ids_.begin(),node->row_ids_.end());
        return;
    }
    node->parent = parent;
    if(parent == nil_){
        root_ = node;
    }else if(node->value_ < parent->value_){
        parent->left = node;
    }else{
        parent->right = node;
    }
    node->left = nil_;
    node->right = nil_;
    node->color = red;
    rb_insert_fixup(node);
}

//删除结点后的颜色修复与结点调整
void RBTree::rb_delete_fixup(RBTreeNode* node){
    RBTreeNode* bro = nil_;
    while(node != root_ && node->color == black){
        if(node == node->parent->left){
            bro = node->parent->right;
            if(bro->color == red){
                bro->color = black;
                node->parent->color = red;
                left_rotate(node->parent);
                bro = node->parent->right;
            }
            if(bro->left->color == black && bro->right->color == black){
                bro->color = red;
                node = node->parent;
            }else{
                if(bro->right->color == black){
                    bro->left->color = black;
                    bro->color = red;
                    right_rotate(bro);
                    bro = node->parent->right;
                } 
                bro->color = node->parent->color;
                node->parent->color = black;
                bro->right->color = black;
                left_rotate(node->parent);
                node = root_;
            }
        }else{
            bro = node->parent->left;
            if(bro->color == red){
                bro->color = black;
                node->parent->color = red;
                right_rotate(node->parent);
                bro = node->parent->left;
            }
            if(bro->left->color == black && bro->right->color == black){
                bro->color = red;
                node = node->parent;
            }else{
                if(bro->left->color == black){
                    bro->right->color = black;
                    bro->color = red;
                    left_rotate(bro);
                    bro = node->parent->left;
                }
                bro->color = node->parent->color;
                node->parent->color = black;
                bro->left->color = black;
                right_rotate(node->parent);
                node = root_;
            }

        }
    }
    node->color = black;
}

//删除结点
RBTreeNode* RBTree::rb_delete(RBTreeNode* node, int32_t row_id){
    if(node->row_ids_.size() >= 2){
        node->row_ids_.erase(row_id);
        return node;
    }
    RBTreeNode* del = nil_;//待删除的结点
    RBTreeNode* child = nil_;//待删除结点的孩子结点
    //找到待删除的结点
    if(node->left == nil_ || node->right == nil_){
        del = node;
    }else{
        del = rb_successor(node);
    }
    //找到待删除结点的孩子
    if(del->left != nil_){
        child = del->left;
    }else{
        child = del->right;
    }
    //连接待删除结点的孩子和父亲
    child->parent = del->parent;
    if(del->parent == nil_){
        root_ = child;
    }else if(del == del->parent->left){
        del->parent->left = child;
    }else{
        del->parent->right = child;
    }
    //如果删除的结点与原要删除结点不同，需要拷贝其值
    if(del != node){
        node->value_ = del->value_;
    }
    //如果删除的结点颜色为黑，需要进行修复
    if(del->color == black){
        rb_delete_fixup(child);
    }
    return del;
}


//寻找树中是否有一个值，有则返回指向它的指针，没有则返回nil_；
RBTreeNode* RBTree::find_ptr(int32_t value){
    RBTreeNode* current = root_;
    while(current != nil_ && current->value_ != value){
        if(current->value_ > value){
            current = current->left;
        }else{
            current = current->right;
        }
    }
    if(current == nil_){
        return nullptr;
    }
    return current;
}


//寻找一个值所在的行。
std::unordered_set<int32_t> RBTree::find_row(int32_t value){
    RBTreeNode* current = root_;//当前结点
    while(current != nil_ && current->value_ != value){
        if(current->value_ > value){
            current = current->left;
        }else{
            current = current->right;
        }
    }
    if(current == nil_){
        return std::unordered_set<int32_t>();
    }
    return current->row_ids_;
}

  //给出一个值，寻找大于该值的所有行
std::unordered_set<int32_t> RBTree::find_greater(int32_t value){
    RBTreeNode* current = root_;
    std::queue<RBTreeNode*> q;
    std::unordered_set<int32_t> ans;
    while(current != nil_ && current->value_ <= value){
        current = current->right;
    }
    if(current == nullptr){
        return ans;
    }
    q.push(current);
    while(!q.empty()){
        RBTreeNode* temp = q.front();
        q.pop();
        RBTreeNode* left_node = temp->left;
        while(left_node != nil_){
            if(left_node->value_ > value){
                q.push(left_node);
                break;
            }
            left_node = left_node->right;
        }
        if(temp->right != nil_){
            q.push(temp->right);
        }
        for(auto row_id : temp->row_ids_){
            ans.insert(row_id);
        }
    }
    return ans;
}

  //给出一个值，寻找小于该值的所有行
std::unordered_set<int32_t> RBTree::find_less(int32_t value){
    RBTreeNode* current = root_;
    std::queue<RBTreeNode*> q;
    std::unordered_set<int32_t> ans;
    while(current != nil_ && current->value_ >= value){
        current = current->left;
    }
    if(current == nil_){
        return ans;
    }
    q.push(current);
    while(!q.empty()){
        RBTreeNode* temp = q.front();
        q.pop();
        RBTreeNode* right_node = temp->right;
        if(temp->left != nil_){
            q.push(temp->left);
        }
        while(right_node != nil_){
            if(right_node->value_ < value){
                q.push(right_node);
                break;
            }
            right_node = right_node->left;
        }
        for(auto row_id : temp->row_ids_){
            ans.insert(row_id);
        }
    }
    return ans;
}


}//bytedance_db_project