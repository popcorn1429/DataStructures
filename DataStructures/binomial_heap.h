#ifndef __binomial_heap_h__
#define __binomial_heap_h__

#include <vector>
#include <cstdlib>
#include <algorithm>

namespace detail {
    template <typename T> struct BinomialNode;
}

template <typename T>
class BinomialHeap {
public:
    BinomialHeap();
    ~BinomialHeap();
    BinomialHeap(const BinomialHeap&);
    BinomialHeap& operator=(const BinomialHeap&);

    void push(const T&);
    const T& top() const;
    void pop();

    void merge(BinomialHeap& heap);
    void swap(BinomialHeap& heap);

    std::size_t size() const;
    bool empty() const;
private:
    std::vector<detail::BinomialNode<T>*> mTrees;
    std::size_t                           mSize;
};

/*******************Implementation***********************/
namespace detail {
    template <typename T>
    struct BinomialNode{
        T    mValue;
        BinomialNode* mRight;
        BinomialNode* mChild;

        BinomialNode(const T& val, BinomialNode* right, BinomialNode* child) :
            mValue(val), mRight(right), mChild(child) {}
    };

    template <typename T>
    bool CompareNodesByValue(const BinomialNode<T>* _first, const BinomialNode<T>* _second) {
        if (_first == nullptr || _second == nullptr) {
            return _first != nullptr && _second == nullptr;
        }

        return _first->mValue < _second->mValue;
    }

    //_first and _second should be roots of two trees
    //these two trees must have the same depth.
    template <typename T>
    BinomialNode<T>* MergeTrees(BinomialNode<T>* _first, BinomialNode<T>* _second) {        
        if (_first == nullptr && _second == nullptr)
            return nullptr;

        if (_second->mValue < _first->mValue) {
            auto _tmp = _first;
            _first = _second;
            _second = _tmp;
        }
        
        _second->mRight = _first->mChild;
        _first->mChild = _second;
        return _first;
    }

    template <typename T>
    void MergeHeaps(std::vector<BinomialNode<T>*>& _first, std::vector<BinomialNode<T>*>& _second) {
        std::vector<BinomialNode<T>*> result;
        const std::size_t maxCnt = _first.size() < _second.size() ? _second.size() : _first.size();
        _first.resize(maxCnt);
        _second.resize(maxCnt);

        BinomialNode<T>* carry = nullptr;
        std::vector<BinomialNode<T>*> trees;
        for (std::size_t i = 0; i < maxCnt; ++i) {
            trees.clear();
            if (carry != nullptr)
                trees.push_back(carry);
            if (_first[i] != nullptr)
                trees.push_back(_first[i]);
            if (_second[i] != nullptr)
                trees.push_back(_second[i]);

            switch (trees.size())
            {
            case 0:
                result.push_back(nullptr);
                break;
            case 1:
                result.push_back(trees[0]);
                carry = nullptr;
                break;
            case 2:
                result.push_back(nullptr);
                carry = MergeTrees(trees[0], trees[1]);
                break;
            case 3:
                result.push_back(trees[0]);
                carry = MergeTrees(trees[1], trees[2]);
                break;
            default:
            //never happen.
            break;
            }
        }

        if (carry != nullptr)
            result.push_back(carry);

        _first.swap(result);
        _second.clear();
    }

    template <typename T>
    void DestroyBinomialTree(BinomialNode<T>* tree) {
        if (tree == nullptr)
            return;

        DestroyBinomialTree(tree->mRight);
        DestroyBinomialTree(tree->mChild);
        delete tree;
    }

    template <typename T>
    BinomialNode<T>* CloneBinomialTree(const BinomialNode<T>* tree) {
        if (tree == nullptr)
            return nullptr;

        return new BinomialNode<T>(tree->mValue, CloneBinomialTree(tree->mRight), CloneBinomialTree(tree->mChild));
    }
} //end of namespace : detail

template <typename T>
BinomialHeap<T>::BinomialHeap() : mSize(0) {}

template <typename T>
BinomialHeap<T>::~BinomialHeap() {
    std::for_each(mTrees.begin(), mTrees.end(), detail::DestroyBinomialTree<T>);
}

template <typename T>
BinomialHeap<T>::BinomialHeap(const BinomialHeap<T>& other) : mSize(other.mSize) {
    for (const auto& tree : other.mTrees) {
        mTrees.push_back(detail::CloneBinomialTree(tree));
    }
}

template <typename T>
BinomialHeap<T>& BinomialHeap<T>::operator=(const BinomialHeap<T>& other) {
    BinomialHeap<T> heap(other);
    swap(heap);
    return *this;
}

template <typename T>
void BinomialHeap<T>::push(const T& val) {
    std::vector<detail::BinomialNode<T>*> oneTreeForest;
    oneTreeForest.push_back(new detail::BinomialNode<T>(val, nullptr, nullptr));
    detail::MergeHeaps(mTrees, oneTreeForest);
    ++mSize;
}

template <typename T>
const T& BinomialHeap<T>::top() const {
    return (*std::min_element(mTrees.begin(), mTrees.end(), detail::CompareNodesByValue<T>))->mValue;
}

template <typename T>
void BinomialHeap<T>::pop() {
    auto minRootTree = std::min_element(mTrees.begin(), mTrees.end(), detail::CompareNodesByValue<T>);

    std::vector<detail::BinomialNode<T>*> children;
    for (auto child = (*minRootTree)->mChild; child != nullptr; child = child->mRight) {
        children.push_back(child);
    }

    std::reverse(children.begin(), children.end());

    for (std::size_t i = 0; i < children.size(); ++i)
        children[i]->mRight = nullptr;

    delete *minRootTree;
    *minRootTree = nullptr;

    if (minRootTree + 1 == mTrees.end())
        mTrees.pop_back();

    detail::MergeHeaps(mTrees, children);

    --mSize;
}

template <typename T>
void BinomialHeap<T>::merge(BinomialHeap& heap) {
    detail::MergeHeaps(mTrees, heap.mTrees);
    mSize += heap.mSize;
    heap.mSize = 0;
}

template <typename T>
void BinomialHeap<T>::swap(BinomialHeap& heap) {
    mTrees.swap(heap.mTrees);
    std::swap(mSize, heap.mSize);
}

template <typename T>
std::size_t BinomialHeap<T>::size() const {
    return mSize;
}

template <typename T>
bool BinomialHeap<T>::empty() const {
    return size() == 0;
}

#endif /*__binomial_heap_h__*/