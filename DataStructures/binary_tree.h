#ifndef __binary_tree_h__
#define __binary_tree_h__

#include <iostream>

template <typename T>
class BinaryTree;

template <typename T>
class BinaryTreeNode {
    friend class BinaryTree<T>;
public:
    BinaryTreeNode(const T& v) : data(v), left(nullptr), right(nullptr){}

    T getData() const { return data; }
    const T& constRefData() const { return data; }
private:
    T           data;
    BinaryTreeNode*   left;
    BinaryTreeNode*   right;
};

/*
二叉树
没有做任何平衡处理，数据输入越随机越好
可能出现严重的不平衡（输入越有序，树越不平衡）
功能很少
*/
template <typename T>
class BinaryTree {
public:
    enum class TravelOrder {
        PRE_ORDER,
        IN_ORDER,
        POST_ORDER
    };

public:
    BinaryTree() :root(nullptr){}
    ~BinaryTree() {
        if (nullptr != root) {
            eraseSubTree(root);
            root = nullptr;
        }
    }

    bool isEmpty() const { return root == nullptr; }

    BinaryTreeNode<T>* search(const T& v) {
        return search(v, root);
    }

    void insert(const T& v) {
        insert(v, root);
    }

    void print(TravelOrder order = TravelOrder::IN_ORDER) const {
        std::cout << "print binary tree in ";
        switch (order)
        {
        case TravelOrder::PRE_ORDER:
        std::cout << "pre_order:\n\t";
        print_pre_order(root);
        break;
        case TravelOrder::IN_ORDER:
        std::cout << "in_order:\n\t";
        print_in_order(root);
        break;
        case TravelOrder::POST_ORDER:
        std::cout << "post_order:\n\t";
        print_post_order(root);
        break;
        default:
        break;
        }
        std::cout << "end" << std::endl;
    }

private:
    void eraseSubTree(BinaryTreeNode<T>* n) {
        if (n->left != nullptr)
            eraseSubTree(n->left);
        if (n->right != nullptr)
            eraseSubTree(n->right);

        delete n;
    }

    BinaryTreeNode<T>* search(const T& v, BinaryTreeNode<T>* pos) const {
        if (pos == nullptr)
            return nullptr;

        if (v == pos->constRefData())
            return pos;
        else if (v < pos->constRefData())
            return search(v, pos->left);
        else
            return search(v, pos->right);
    }

    void insert(const T& v, BinaryTreeNode<T>*& pos) {
        if (nullptr == pos) {
            pos = new BinaryTreeNode<T>(v);
        }
        else {
            if (v < pos->constRefData()) {
                insert(v, pos->left);
            }
            else if (pos->constRefData() < v) {
                insert(v, pos->right);
            }
            else
                return; //already had a duplicate element in the tree
        }
    }

    void print_pre_order(BinaryTreeNode<T>* pos) const {
        if (nullptr != pos) {
            std::cout << pos->constRefData() << ',';
            print_pre_order(pos->left);
            print_pre_order(pos->right);
        }
    }

    void print_in_order(BinaryTreeNode<T>* pos) const {
        if (nullptr != pos) {
            print_in_order(pos->left);
            std::cout << pos->constRefData() << ',';
            print_in_order(pos->right);
        }
    }

    void print_post_order(BinaryTreeNode<T>* pos) const {
        if (nullptr != pos) {
            print_post_order(pos->left);
            print_post_order(pos->right);
            std::cout << pos->constRefData() << ',';
        }
    }

private:
    BinaryTreeNode<T>*  root;
};

#endif /*__binary_tree_h__*/