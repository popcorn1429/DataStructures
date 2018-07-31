//single linked list, whose data will be sorted when inserted.

#ifndef __sorted_linked_list_h__
#define __sorted_linked_list_h__
#include <iostream> //import cout/endl

template <typename T>
class SortedLinkedList;

//definition of ListNode<T>
template <typename T>
class ListNode {
    friend class SortedLinkedList<T>;
public:
    ListNode(const T& v) :data(v), next(nullptr){}
    T getData() const { return data; }
private:
    T          data;
    ListNode*  next;
};

//definition of List<T>
template <typename T>
class SortedLinkedList {
public:
    SortedLinkedList() : head(nullptr), tail(nullptr), num(0){}
    ~SortedLinkedList() {
        while (head != nullptr)
        {
            auto p = head;
            head = head->next;
            delete p;
        }
    }

    size_t size() const { return num; }
    bool empty() const { return num == 0; }

    void insert(const T& v) {
        if (num == 0) {
            auto p = new ListNode<T>(v);
            head = p;
            tail = p;
            ++num;
        }
        else {
            if (v < head->data) {
                insert_head(v);
            }
            else if (tail->data < v) {
                insert_tail(v);
            }
            else {
                auto p = head;
                while (p != nullptr && p->data < v && p->next != nullptr && p->next->data < v)
                {
                    p = p->next;
                }

                auto pv = new ListNode<T>(v);
                pv->next = p->next;
                p->next = pv;
                ++num;
            }
        }
    }

    ListNode<T>* search(const T& v) const {
        auto p = head;
        while (p != nullptr && p->data < v) {
            p = p->next;
        }

        if (p->data == v)
            return p;
        else
            return nullptr;
    }

    bool removeOne(const T& v) {
        auto p = head;
        while (p != nullptr && p->data < v && p->next != nullptr && p->next->data < v)
            p = p->next;

        if (p->next != nullptr && p->next->data == v) {
            auto pv = p->next;
            p->next = pv->next;
            delete pv;
            --num;
            return true;
        }
        else {
            return false;
        }
    }

    size_t removeAll(const T& v) {
        auto p = head;
        while (p != nullptr && p->data < v && p->next != nullptr && p->next->data < v)
            p = p->next;

        size_t n = 0;
        while (p->next != nullptr && p->next->data == v) {
            auto pv = p->next;
            p->next = pv->next;
            delete pv;
            --num;
            ++n;
            //p = p->next;
        }

        //specialy, head data is equal to v
        if (head->data == v) {
            p = head;
            head = head->next;
            delete p;
            --num;
            ++n;
        }

        return n;
    }

    void print() const {
        if (empty()) {
            std::cout << "list is empty." << std::endl;
            return;
        }

        std::cout << "list:\n\t";
        auto p = head;
        while (p != nullptr)
        {
            std::cout << "[" << p->data << "]->";
            p = p->next;
        }
        std::cout << "[nullptr]" << std::endl;
    }
private:
    void insert_head(const T& v) {
        auto p = new ListNode<T>(v);
        p->next = head;
        head = p;
        ++num;
    }
    void insert_tail(const T& v) {
        auto p = new ListNode<T>(v);
        tail->next = p;
        tail = p;
        ++num;
    }

private:
    ListNode<T>*   head;
    ListNode<T>*   tail;
    size_t         num;
};

#endif /*__sorted_linked_list_h__*/