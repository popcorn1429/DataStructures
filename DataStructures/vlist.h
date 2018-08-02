#ifndef __VLIST_H__
#define __VLIST_H__

template <typename T>
class VList {
private:
    class VListCell;

public:
    VList() : head(nullptr), cnt(0) {}

    void add(const T& data) {
        if (head == nullptr || head->freespace == 0)
            head = new VListCell(head == nullptr ? 1 : 2 * head->length, head);

        head->elems[head->freespace-- - 1] = data;
        ++cnt;
    }
    void push(const T& data) {
        add(data);
    }

    T pop() {
        if (empty())
            throw "no element, can't pop anything.";

        T data = head->elems[head->freespace];
        ++head->freespace;
        --cnt;
        if (head->freespace == head->length) {
            VListCell* tmp = head;
            head = head->next;
            head->prev = nullptr;
            delete tmp;
        }

        return data;
    }

    typedef T& reference;
    typedef const T& const_reference;
    reference front() {
        //get first cell
        VListCell* pCur, *pNext;
        for (pCur = head, pNext = pCur->next; pNext != nullptr; pCur = pNext, pNext = pNext->next)
            ;

        return pCur->elems[pCur->length - 1];
    }
    const_reference front() const {
        //get first cell
        VListCell* pCur, *pNext;
        for (pCur = head, pNext = pCur->next; pNext != nullptr; pCur = pNext, pNext = pNext->next)
            ;

        return pCur->elems[pCur->length - 1];
    }
    reference back() {
        return head->elems[head->freespace];
    }
    const_reference back() const {
        return head->elems[head->freespace];
    }

    size_t size() const { return cnt; }
    bool empty() const { return 0 == cnt; }

    class VListIterator;
    typedef VListIterator iterator;
    typedef const VListIterator const_iterator;
    iterator begin() {
        //get first cell
        VListCell* pCur, *pNext;
        for (pCur = head, pNext = pCur->next; pNext != nullptr; pCur = pNext, pNext = pNext->next)
            ;

        return VListIterator(pCur, pCur->length - 1);
    }
    const_iterator begin() const {
        //get first cell
        VListCell* pCur, *pNext;
        for (pCur = head, pNext = pCur->next; pNext != nullptr; pCur = pNext, pNext = pNext->next)
            ;

        return VListIterator(pCur, pCur->length - 1);
    }
    iterator end() {
        return head->freespace == 0 ? VListIterator(nullptr, 0) : VListIterator(head, head->freespace - 1);
    }
    const_iterator end() const {
        return head->freespace == 0 ? VListIterator(nullptr, 0) : VListIterator(head, head->freespace - 1);
    }

    iterator insert(iterator pos, const T& data) {
        add(T());  //add a default data at the end of list
        if (pos.cell == nullptr) {  //if pos point to null cell, treat it as end()
            back() = data;    //update the data
            return end();
        }
        if (pos.cell == head) { //if pos point to the head cell, just move elements between end() and pos.offset, then update the new data
            memmove(head->elems + head->freespace, head->elems + head->freespace + 1, sizeof(T)*(pos.offset - head->freespace));
            head->elems[pos.offset] = data;
        }
        else {  //otherwise, move elements in all cells between head and pos.cell
            VListCell* pCur = head;
            while (pCur != pos.cell && pCur->next != nullptr) {
                memmove(pCur->elems + pCur->freespace, pCur->elems + pCur->freespace + 1, sizeof(T)* (pCur->length - 1 - pCur->freespace));
                memcpy(pCur->elems + pCur->length - 1, pCur->next->elems, sizeof(T));
                pCur = pCur->next;
            }
            if (pCur == pos.cell) {
                memmove(pCur->elems + pCur->freespace, pCur->elems + pCur->freespace + 1, sizeof(T)*(pos.offset - pCur->freespace));
                pCur->elems[pos.offset] = data;
            }
        }
        return pos;  //return pos (because now pos point to the position of the new data)
    }

public:
    VListCell* head;
    size_t     cnt;
};

template <typename T>
class VList<T>::VListCell {
public:
    VListCell(size_t len, VListCell* cell) : length(len), freespace(len), prev(nullptr), next(cell) {
        elems = new T[len];
        if (next != nullptr)
            next->prev = this;
    }

public:
    T*          elems;
    size_t      length;
    size_t      freespace;
    VListCell*  prev;
    VListCell*  next;
};

//VList<T>::iterator (bidirectional iterator)
template <typename T>
class VList<T>::VListIterator {
public:
    typedef T           value_type;
    typedef ptrdiff_t   difference_type;
    typedef T*          pointer;
    typedef T&          reference;

    friend class VListCell;
public:
    VListIterator() : cell(nullptr), offset(0) {}
    VListIterator(VListCell* c, size_t s) : cell(c), offset(s) {}
    VListIterator(const VListIterator& old) : cell(old.cell), offset(old.offset) {}
    VListIterator& operator=(const VListIterator& old) {
        cell = old.cell;
        offset = old.offset;
        return *this;
    }

    bool operator==(const VListIterator& rhs) const {
        return cell == rhs.cell && offset == rhs.offset;
    }
    bool operator!=(const VListIterator& rhs) const {
        return !(*this == rhs);
    }

    reference operator*() { return cell->elems[offset]; }
    const reference operator*() const { return cell->elems[offset]; }
    pointer   operator->() { return &(cell->elems[offset]); }
    const pointer operator->() const { return &(cell->elems[offset]); }

    VListIterator& operator++() {
        if (offset == 0) {
            cell = cell->prev;
            offset = cell == nullptr?0:cell->length - 1;
        }
        else {
            --offset;
        }
        return *this;
    }
    VListIterator operator++(int) {
        VListIterator bak = *this;
        ++*this;
        return bak;
    }

    VListIterator& operator--() {
        if (cell == nullptr)
            return *this;

        if (offset == cell->length - 1) {
            offset = 0;
            cell = cell->next;
        }
        else {
            ++offset;
        }
        return *this;
    }
    VListIterator operator--(int) {
        VListIterator bak = *this;
        --*this;
        return bak;
    }

public:
    VListCell*  cell;
    size_t      offset;
};

#endif /*__VLIST_H__*/