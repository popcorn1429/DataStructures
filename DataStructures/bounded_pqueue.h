#ifndef _bounded_pqueue_h_
#define _bounded_pqueue_h_
#include <cstdlib>  //std::size_t
//need use map to implement this data structure~
//so i need implement map first...
//trust me, i'll complete all of this !
template <typename T>
class BoundedPQueue {
public:
    BoundedPQueue(std::size_t n) : mMaxSize(n) {
        //
    }

    void enqueue(const T& val, double priority) {
        //
    }

    T dequeue() {
        //
    }



    const T& best() const {
        //
    }
    const T& worst() const {
        //
    }

    bool empty() const { return 0 == size(); }
    std::size_t size() const {
        //
    }
    std::size_t maxSize() const { return mMaxSize; }
private:
    std::size_t   mMaxSize;
};


#endif /*_bounded_pqueue_h_*/