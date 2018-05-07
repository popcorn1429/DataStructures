#ifndef _priority_queue_h_
#define _priority_queue_h_

#include <vector>

namespace detail {
    template <typename T>
    struct PQElement {
        T      mVal;
        double mPriority;
    
        PQElement(const T& v, double priority) : mVal(v), mPriority(priority) {}

        PQElement& operator=(const PQElement& other) {
            mVal = other.mVal;
            mPriority = other.mPriority;
            return *this;
        }

    };
}

template <typename T>
class PriorityQueue {
public:
    PriorityQueue() {}

    void push(const T& v, double priority) {
        if (empty()) {
            mContainer.push_back(detail::PQElement<T>(v, priority));
        }
        else {
            mContainer.push_back(detail::PQElement<T>(v, priority));
            auto pos = size() - 1;
            auto parent = (pos - 1) / 2;
            while (pos != 0 && mContainer[parent].mPriority < priority) {
                std::swap(mContainer[pos], mContainer[parent]);
                pos = parent;
                parent = (pos - 1) / 2;
            }
        }
    }

    const T& peek() const {
        return mContainer[0].mVal;
    }

    void pop() {
        if (empty())
            throw "Exception:pop element from empty container!";

        auto sz = size();
        if (sz == 1) {
            mContainer.clear();
            return;
        }

        decltype(sz) pos = 0, left = 1, right = 2, lastPos = sz-1;
        while (left < lastPos) {
            if (right < lastPos) {
                if (mContainer[left].mPriority <= mContainer[lastPos].mPriority
                    && mContainer[right].mPriority <= mContainer[lastPos].mPriority) {
                    break;
                }
                else {
                    if (mContainer[left].mPriority <= mContainer[right].mPriority) {
                        mContainer[pos] = mContainer[right];
                        pos = right;
                    }
                    else {
                        mContainer[pos] = mContainer[left];
                        pos = left;
                    }
                }
            }
            else { //no right
                if (mContainer[left].mPriority <= mContainer[lastPos].mPriority) {
                    break;
                }
                else {
                    mContainer[pos] = mContainer[left];
                    pos = left;
                }
            }

            left = 2 * pos + 1;
            right = left + 1;
        }

        mContainer[pos] = mContainer[lastPos];
        mContainer.pop_back();
    }

    bool empty() const { return mContainer.empty(); }
    typename std::vector<detail::PQElement<T>>::size_type size() const { return mContainer.size(); }
    //auto size() const { return mContainer.size(); }  //need c++14 support
private:
    std::vector<detail::PQElement<T>>  mContainer;
};

#endif /*_priority_queue_h_*/