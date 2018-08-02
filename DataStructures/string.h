#ifndef __STRING_H__
#define __STRING_H__
#include <locale>   //import isspace(charType ch, const locale& loc)
#include <iostream> //import istream & ostream
#include <iterator> //import reverse_iterator
#include <algorithm>//import copy, copy_backward, lexicographical_compare, swap

template <typename charType = char>
class String {
public:
    String() : impl.smallString.length(0), isSmallString(true) {}
    String(charType c) : impl.smallString.length(1), isSmallString(true) { 
        impl.smallString.elems[0] = c;
        impl.smallString.elems[1] = charType(0); //null-terminated character like '\0'
    }
    String(const charType* str) {
        const size_t len = char_traits<charType>::length(str);
        isSmallString = (len < uMaxSmallStringSize);
        if (!isSmallString) {
            size_t blocks = len / uMaxSmallStringSize;
            if (blocks * uMaxSmallStringSize <= len)
                ++blocks;

            impl.largeString.allocatedLength = blocks*uMaxSmallStringSize;
            impl.largeString.elems = new charType[impl.largeString.allocatedLength];
            impl.largeString.logicalLength = len;
        }
        else {
            impl.smallString.length = (unsigned char)len;
        }

        std::copy(str, str + len + 1, begin()); //copy the last character (null-terminated character)
    }
    template <typename Iterator>
    String(Iterator first, Iterator last) : impl.smallString.length(0), isSmallString(true) {
        insert(begin(), first, last);
    }
    String(const String& other) : isSmallString(other.isSmallString) {
        if (!isSmallString) {
            impl.largeString.allocatedLength = other.impl.largeString.allocatedLength;
            impl.largeString.elems = new charType[impl.largeString.allocatedLength];
            impl.largeString.logicalLength = other.impl.largeString.logicalLength;
        }
        else {
            impl.smallString.length = other.impl.smallString.length;
        }

        std::copy(other.begin(), other.end() + 1, begin());
    }

    ~String() {
        if (!isSmallString) {
            delete[] impl.largeString.elems;
        }
    }

    String& operator=(const String& other) {
        String t(other);
        swap(t);
        return *this;
    }

    charType& operator[](size_t index) { return *(begin() + index); }
    charType  operator[](size_t index) const { return *(begin() + index); }

    typedef charType* iterator;
    typedef const charType* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    iterator begin() { return isSmallString ? impl.smallString.elems : impl.largeString.elems; }
    const_iterator begin() const { return isSmallString ? impl.smallString.elems : impl.largeString.elems; }
    const_iterator cbegin() const { return const_cast<const String&>(*this).begin(); }
    iterator end() { return begin() + size(); }
    const_iterator end() const { return begin() + size(); }
    const_iterator cend() const { return const_cast<const String&>(*this).end(); }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crbegin() const { return const_cast<const String&>(*this).rbegin(); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(cbegin()); }
    const_reverse_iterator crend() const { return const_cast<const String&>(*this).rend(); }

    size_t size() const { return isSmallString ? impl.smallString.length : impl.largeString.logicalLength; }
    bool empty() const { return size() == 0; }
    size_t capacity() const { return isSmallString ? uMaxSmallStringSize : impl.largeString.allocatedLength; }
    void reserve(size_t spaces) {
        if (spaces <= capacity()) return;

        size_t curSize = size();
        size_t curCapacity = capacity();
        while (curCapacity < spaces)
            curCapacity += uMaxSmallStringSize; //always make capacity be an integral multiple of uMaxSmallStringSize

        charType* newElems = new charType[curCapacity];
        std::copy(begin(), end() + 1, newElems);//copy original string (including the null-terminated character)
        if (!isSmallString)
            delete[] impl.largeString.elems;
        else
            impl.largeString.logicalLength = curSize;

        isSmallString = false;
        impl.largeString.allocatedLength = curCapacity;
        impl.largeString.elems = newElems;
    }

    iterator insert(iterator pos, charType c) {
        ptrdiff_t offset = pos - begin();
        reserve(size() + 2); //original string + c + charType(0)
        pos = begin() + offset;

        std::copy_backward(pos, end() + 1, end() + 2);
        *pos = c;

        if (isSmallString)
            ++impl.smallString.length;
        else
            ++impl.largeString.logicalLength;
        
        return pos;
    }
    template <typename Iterator>
    iterator insert(iterator pos, Iterator first, Iterator last) {
        for (; first != last; ++first, ++pos)
            pos = insert(pos, *first);
        return pos;
    }
    
    iterator erase(iterator pos) { return erase(pos, pos + 1); }
    iterator erase(iterator first, iterator last) {
        const ptrdiff_t num = last - first;
        std::copy(first + num, end() + 1, first);
        if (isSmallString)
            impl.smallString.length -= num;
        else
            impl.largeString.logicalLength -= num;
        return first;
    }

    void swap(String& other) {
        std::swap(isSmallString, other.isSmallString);
        std::swap(impl, other.impl);
    }
    
    const charType* c_str() const { return cbegin(); }
    const charType* data() const { return c_str(); }

    String& operator+=(const charType& c) {
        insert(end(), c);
        return *this;
    }

    String& operator+=(const charType* str) {
        const size_t len = char_traits<charType>::length(str);
        reserve(size() + len + 1);
        std::copy(str, str + len + 1, end());
        if (isSmallString)
            impl.smallString.length += len;
        else
            impl.largeString.logicalLength += len;
        return *this;
    }

    String& operator+=(const String& other) {
        reserve(size() + other.size() + 1); //last one character is null-terminated character
        std::copy(other.begin(), other.end()+1, end()); //copy other's string with the null-terminated character
        if (isSmallString)
            impl.smallString.length += other.size();
        else
            impl.largeString.logicalLength += other.size();
        return *this;
    }

    String& append(const charType& c) { return (*this) += c; }
    String& append(const charType* str) { return (*this) += str; }
    String& append(const String& other) { return (*this) += other; }

private:
    static const size_t uMaxSmallStringSize = 32;

    struct LargeString {
        //logicalLength can't be equal to allocatedLength (logicalLength <= allocatedLength-1)
        charType* elems;
        size_t    logicalLength; //real size of this string (exclude the last null-terminated character)
        size_t    allocatedLength; //capacity of this string
    };

    struct SmallString {
        //length can't be equal to uMaxSmallStringSize (length <= uMaxSmallStringSize-1)
        unsigned char length; //real size of this string (exclude the last null-terminated character)
        charType      elems[uMaxSmallStringSize];
    };

    union StringImpl {
        LargeString largeString;
        SmallString smallString;
    };

private:
    StringImpl    impl;
    bool          isSmallString;
};

template <typename charType>
std::ostream& operator<<(std::ostream& out, const String<charType>& str) { return out << str.c_str(); }
template <typename charType>
std::wostream& operator<<(std::wostream& wout, const String<charType>& str) { return wout << str.c_str(); }

template <typename charType>
std::istream& operator>>(std::istream& in, String<charType>& str) {
    String<charType> tmp;
    std::istream::sentry snty(in);
    if (snty) {
        charType c;
        while (in.get(c)) {
            if (std::isspace(c, in.getloc())) {
                in.unget();
                break;
            }
            tmp += c;
        }
        std::swap(str, tmp);
    }
    return in;
}
template <typename charType>
const String<charType> operator+(const String<charType>& lhs, const String<charType>& rhs) {
    return String<charType>(lhs) += rhs;
}
template <typename charType>
bool operator<(const String<charType>& lhs, const String<charType>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template <typename charType>
bool operator<=(const String<charType>& lhs, const String<charType>& rhs) {
    return !(rhs < lhs);
}
template <typename charType>
bool operator==(const String<charType>& lhs, const String<charType>& rhs) {
    return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename charType>
bool operator!=(const String<charType>& lhs, const String<charType>& rhs) {
    return !(lhs == rhs);
}
template <typename charType>
bool operator>(const String<charType>& lhs, const String<charType>& rhs) {
    return rhs < lhs;
}
template <typename charType>
bool operator>=(const String<charType>& lhs, const String<charType>& rhs) {
    return !(lhs < rhs);
}

namespace std {
    template <typename charType>
    void swap(String<charType>& lhs, String<charType>& rhs) {
        lhs.swap(rhs);
    }
}

#endif /*__STRING_H__*/