#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <cstdlib>
#include <ostream>
#include <functional>
#include <algorithm>
#include <functional>
#include <numeric>
#include <sstream>
#include <cmath>
using namespace std;

/*
definition of
    Matrix<Rows, Cols, ElemType>
*/
template <size_t Rows, size_t Cols, typename ElemType = double>
class Matrix {
public:
    //-----constructor-----
    Matrix() {}
    template <typename InputIterator>
    Matrix(InputIterator first, InputIterator last) {
        std::copy(first, last, begin());
    }

    //-----accessor-----
    ElemType& at(size_t row, size_t col) {
        //return *(begin() + row*numCols() + col);
        //a safer way. make it always use the same implementation with the const version (from Effective C++)
        return const_cast<ElemType&>(static_cast<const Matrix<Rows, Cols, ElemType>*>(this)->at(row, col));
    }
    const ElemType& at(size_t row, size_t col) const {
        return *(begin() + row*numCols() + col);
    }

    size_t numRows() const { return Rows; }
    size_t numCols() const { return Cols; }
    size_t size() const { return numRows()*numCols(); }

    class MutableReference;
    class ImmutableReference;
    MutableReference operator[](size_t row) { return MutableReference(this, row); }
    ImmutableReference operator[](size_t row) const { return ImmutableReference(this, row); }

    typedef ElemType* iterator;
    typedef const ElemType* const_iterator;
    iterator begin() { return elems; }
    iterator end() { return begin() + size(); }
    const_iterator begin() const { return elems; }
    const_iterator end() const { return begin() + size(); }

    iterator row_begin(size_t row) { return begin() + row*numCols(); }
    iterator row_end(size_t row) { return row_begin(row) + numCols(); }
    const_iterator row_begin(size_t row) const { return begin() + row*numCols(); }
    const_iterator row_end(size_t row) const { return row_begin(row) + numCols(); }

    //------calculator------
    Matrix& operator+=(const Matrix& rhs) {
        std::transform(begin(), end(), rhs.begin(), begin(), std::plus<ElemType>());
        return *this;
    }
    Matrix& operator-=(const Matrix& rhs) {
        std::transform(begin(), end(), rhs.begin(), begin(), std::minus<ElemType>());
        return *this;
    }
    Matrix& operator*=(const ElemType& scalar) {
        std::transform(begin(), end(), begin(), std::bind2nd(std::multiplies<ElemType>(), scalar));
        return *this;
    }
    Matrix& operator/=(const ElemType& scalar) {
        std::transform(begin(), end(), begin(), std::bind2nd(std::divides<ElemType>(), scalar));
        return *this;
    }
private:
    ElemType elems[Rows*Cols];
};

template <size_t Rows, size_t Cols, typename ElemType>
class Matrix<Rows, Cols, ElemType>::MutableReference {
public:
    ElemType& operator[](size_t col) { return parent->at(row, col); }
private:
    friend class Matrix;
    MutableReference(Matrix* matrix, size_t r) : parent(matrix), row(r) {}
private:
    Matrix* const parent;
    const size_t row;
};

template <size_t Rows, size_t Cols, typename ElemType>
class Matrix<Rows, Cols, ElemType>::ImmutableReference {
public:
    const ElemType& operator[](size_t col) const { return parent->at(row, col); }
private:
    friend class Matrix;
    ImmutableReference(const Matrix* matrix, size_t r) : parent(matrix), row(r) {}
private:
    const Matrix* const parent;
    const size_t row;
};

// M1 + M2
template <size_t Rows, size_t Cols, typename ElemType>
const Matrix<Rows, Cols, ElemType> operator+(const Matrix<Rows, Cols, ElemType>& lhs, const Matrix<Rows, Cols, ElemType>& rhs) {
    return Matrix<Rows, Cols, ElemType>(lhs) += rhs;  //copy-constructor and operator+=
}
// M1 - M2
template <size_t Rows, size_t Cols, typename ElemType>
const Matrix<Rows, Cols, ElemType> operator-(const Matrix<Rows, Cols, ElemType>& lhs, const Matrix<Rows, Cols, ElemType>& rhs) {
    return Matrix<Rows, Cols, ElemType>(lhs) -= rhs;
}
// M * s
template <size_t Rows, size_t Cols, typename ElemType>
const Matrix<Rows, Cols, ElemType> operator*(const Matrix<Rows, Cols, ElemType>& lhs, const ElemType& scalar) {
    return Matrix<Rows, Cols, ElemType>(lhs) *= scalar;
}
// s * M
template <size_t Rows, size_t Cols, typename ElemType>
const Matrix<Rows, Cols, ElemType> operator*(const ElemType& scalar, const Matrix<Rows, Cols, ElemType>& rhs) {
    return Matrix<Rows, Cols, ElemType>(rhs) *= scalar;
}
// M / s
template <size_t Rows, size_t Cols, typename ElemType>
const Matrix<Rows, Cols, ElemType> operator/(const Matrix<Rows, Cols, ElemType>& lhs, const ElemType& scalar) {
    return Matrix<Rows, Cols, ElemType>(lhs) /= scalar;
}
// +M
template <size_t Rows, size_t Cols, typename ElemType>
const Matrix<Rows, Cols, ElemType> operator+(const Matrix<Rows, Cols, ElemType>& operand) {
    return operand;
}
// -M
template <size_t Rows, size_t Cols, typename ElemType>
const Matrix<Rows, Cols, ElemType> operator-(const Matrix<Rows, Cols, ElemType>& operand) {
    return Matrix<Rows, Cols, ElemType>(operand) *= ElemType(-1);
}
// M * N
template <size_t R, size_t S, size_t T, typename ElemType>
const Matrix<R, T, ElemType> operator*(const Matrix<R, S, ElemType>& lhs, const Matrix<S, T, ElemType>& rhs) {
    Matrix<R, T, ElemType> result;
    std::fill(result.begin(), result.end(), ElemType(0));

    for (size_t row = 0; row < result.numRows(); ++row)
        for (size_t col = 0; col < result.numCols(); ++col)
            for (size_t i = 0; i < S; ++i)
                result[row][col] += lhs[row][i] * rhs[i][col];

    return result;
}
// M1 * M2
template <size_t S, typename ElemType>
const Matrix<S, S, ElemType> operator*(Matrix<S, S, ElemType>& lhs, const Matrix<S, S, ElemType>& rhs) {
    return lhs = lhs * rhs;
}

//---------comparasion between M1 and M2-------------
template <size_t Rows, size_t Cols, typename ElemType>
bool operator==(const Matrix<Rows, Cols, ElemType>& lhs, const Matrix<Rows, Cols, ElemType>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <size_t Rows, size_t Cols, typename ElemType>
bool operator!=(const Matrix<Rows, Cols, ElemType>& lhs, const Matrix<Rows, Cols, ElemType>& rhs) {
    return !(lhs == rhs);
}
template <size_t Rows, size_t Cols, typename ElemType>
bool operator<(const Matrix<Rows, Cols, ElemType>& lhs, const Matrix<Rows, Cols, ElemType>& rhs) {
    auto dif = std::mismatch(lhs.begin(), lhs.end(), rhs.begin());
    return dif.first != lhs.end() && *dif.first < *dif.second;
}
template <size_t Rows, size_t Cols, typename ElemType>
bool operator<=(const Matrix<Rows, Cols, ElemType>& lhs, const Matrix<Rows, Cols, ElemType>& rhs) {
    return !(rhs < lhs);
}
template <size_t Rows, size_t Cols, typename ElemType>
bool operator>(const Matrix<Rows, Cols, ElemType>& lhs, const Matrix<Rows, Cols, ElemType>& rhs) {
    return rhs < lhs;
}
template <size_t Rows, size_t Cols, typename ElemType>
bool operator>=(const Matrix<Rows, Cols, ElemType>& lhs, const Matrix<Rows, Cols, ElemType>& rhs) {
    return !(lhs < rhs);
}

/*
Auxiliary Helper
*/
template <size_t S, typename ElemType = double>
Matrix<S, S, ElemType> Identity() {
    Matrix<S, S, ElemType> result;
    for (size_t row = 0; row < result.numRows(); ++row)
        for (size_t col = 0; col < result.numCols(); ++col)
            result[row][col] = (row == col ? ElemType(1) : ElemType(0));
    return result;
}
template <size_t Rows, size_t Cols, typename ElemType>
const Matrix<Rows, Cols, ElemType> Transpose(const Matrix<Cols, Rows, ElemType>& m) {
    Matrix<Rows, Cols, ElemType> result;
    for (size_t row = 0; row < result.numRows(); ++row)
        for (size_t col = 0; col < result.numCols(); ++col)
            result[row][col] = m[col][row];
    return result;
}

/*
Vector<Rows, ElemType>
*/
template <size_t Rows, typename ElemType = double>
class Vector {
public:
    Vector() {}
    template <typename InputIterator>
    Vector(InputIterator first, InputIterator last) {
        std::copy(first, last, begin());
    }

    ElemType& at(size_t row) { return const_cast<ElemType&>(static_cast<const Vector<Rows, ElemType>*>(this)->at(row)); }
    const ElemType& at(size_t row) const { return *(begin() + row); }

    ElemType& operator[](size_t row) { return const_cast<ElemType&>(static_cast<const Vector<Rows, ElemType>&>(*this)[row]); }
    const ElemType& operator[](size_t row) const { return at(row); }

    size_t numRows() const { return Rows; }
    size_t numCols() const { return 1; }
    size_t size() const { return numRows() * numCols(); }

    typedef ElemType* iterator;
    typedef const ElemType* const_iterator;
    iterator begin() { return elems; }
    iterator end() { return begin() + size(); }
    const_iterator begin() const { return elems; }
    const_iterator end() const { return begin() + size(); }
    iterator row_begin(size_t row) { return begin() + row * numCols(); }
    iterator row_end(size_t row) { return row_begin(row) + numCols(); }
    const_iterator row_begin(size_t row) const { return begin() + row * numCols(); }
    const_iterator row_end(size_t row) const { return row_begin(row) + numCols(); }

    Vector& operator+=(const Vector& rhs) {
        std::transform(begin(), end(), rhs.begin(), begin(), std::plus<ElemType>());
        return *this;
    }
    Vector& operator-=(const Vector& rhs) {
        std::transform(begin(), end(), rhs.begin(), begin(), std::minus<ElemType>());
        return *this;
    }
    Vector& operator*=(const ElemType& rhs) {
        std::transform(begin(), end(), begin(), std::bind2nd(std::multiplies<ElemType>(), rhs));
        return *this;
    }
    Vector& operator/=(const ElemType& rhs) {
        std::transform(begin(), end(), begin(), std::bind2nd(std::divides<ElemType>(), rhs));
        return *this;
    }
private:
    ElemType elems[Rows];
};

//V1 + V2
template <size_t Rows, typename ElemType>
const Vector<Rows, ElemType> operator+(const Vector<Rows, ElemType>& lhs, const Vector<Rows, ElemType>& rhs) {
    return Vector<Rows, ElemType>(lhs) += rhs;
}
//V1 - V2
template <size_t Rows, typename ElemType>
const Vector<Rows, ElemType> operator-(const Vector<Rows, ElemType>& lhs, const Vector<Rows, ElemType>& rhs) {
    return Vector<Rows, ElemType>(lhs) -= rhs;
}
//V1 * s
template <size_t Rows, typename ElemType>
const Vector<Rows, ElemType> operator*(const Vector<Rows, ElemType>& lhs, const ElemType& rhs) {
    return Vector<Rows, ElemType>(lhs) *= rhs;
}
//s * V1
template <size_t Rows, typename ElemType>
const Vector<Rows, ElemType> operator*(const ElemType& lhs, const Vector<Rows, ElemType>& rhs) {
    return Vector<Rows, ElemType>(rhs) *= lhs;
}
//V1 / s
template <size_t Rows, typename ElemType>
const Vector<Rows, ElemType> operator/(const Vector<Rows, ElemType>& lhs, const ElemType& rhs) {
    return Vector<Rows, ElemType>(lhs) /= rhs;
}
//+V1
template <size_t Rows, typename ElemType>
const Vector<Rows, ElemType> operator+(const Vector<Rows, ElemType>& operand) {
    return operand;
}
//-V1
template <size_t Rows, typename ElemType>
const Vector<Rows, ElemType> operator-(const Vector<Rows, ElemType>& operand) {
    return Vector<Rows, ElemType>(operand) *= ElemType(-1);
}
//M * V
template <size_t Rows, size_t Cols, typename ElemType>
const Vector<Rows, ElemType> operator*(const Matrix<Rows, Cols, ElemType>& lhs, const Vector<Cols, ElemType>& rhs) {
    Vector<Rows, ElemType> result;
    for (size_t row = 0; row < result.numRows(); ++row)
        result[row] = std::inner_product(lhs.row_begin(), lhs.row_end(), rhs.begin(), ElemType(0));
    return result;
}

//---------comparasion between V1 and V2-------------
template <size_t Rows, typename ElemType>
bool operator==(const Vector<Rows, ElemType>& lhs, const Vector<Rows, ElemType>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <size_t Rows, typename ElemType>
bool operator!=(const Vector<Rows, ElemType>& lhs, const Vector<Rows, ElemType>& rhs) {
    return !(lhs == rhs);
}
template <size_t Rows, typename ElemType>
bool operator<(const Vector<Rows, ElemType>& lhs, const Vector<Rows, ElemType>& rhs) {
    auto dif = std::mismatch(lhs.begin(), lhs.end(), rhs.begin());
    return dif.first != lhs.end() && *dif.first < *dif.second;
}
template <size_t Rows, typename ElemType>
bool operator<=(const Vector<Rows, ElemType>& lhs, const Vector<Rows, ElemType>& rhs) {
    return !(rhs < lhs);
}
template <size_t Rows, typename ElemType>
bool operator>(const Vector<Rows, ElemType>& lhs, const Vector<Rows, ElemType>& rhs) {
    return rhs < lhs;
}
template <size_t Rows, typename ElemType>
bool operator>=(const Vector<Rows, ElemType>& lhs, const Vector<Rows, ElemType>& rhs) {
    return !(lhs < rhs);
}

//ostream << V
template <size_t Rows, typename ElemType, typename charT, typename traits>
std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& out, const Vector<Rows, ElemType>& v) {
    std::basic_stringstream<charT, traits> buffer;
    buffer.imbue(out.getloc()); //Replaces the current locale.   getloc() : Returns the current locale associated with the stream.
    buffer.flags(out.flags());  //replaces current settings with given ones.  flags() : returns current formatting setting
    buffer.precision(out.precision()); //Sets the precision to the given one.  precision() : Returns the current precision.
    const std::ctype<charT>& ccvt = std::use_facet<std::ctype<charT>>(out.getloc());
    for (size_t i = 0; i < v.numRows(); ++i)
        buffer << v[i] << ccvt.widen(' ');

    return out << buffer.str();
}

//dot product : V1.V2 = a1*b1+a2*b2+...an*bn ,the result is a real number.
template <size_t Rows, typename ElemType = double>
ElemType DotProduct(const Vector<Rows, ElemType>& lhs, const Vector<Rows, ElemType>& rhs) {
    return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), ElemType(0));
}
//cross product: V1¡ÄV2 = (l,m,n)^(o,p,q) = (mq-np, no-lq, lp-mo)
template <size_t Rows, typename ElemType = double>
const Vector<3, ElemType> CrossProduct(const Vector<3, ElemType>& lhs, const Vector<3, ElemType>& rhs) {
    Vector<3, ElemType> result;
    result[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    result[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    result[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
}
template <size_t Rows, typename ElemType = double>
ElemType NormSquared(const Vector<Rows, ElemType>& v) {
    return DotProduct(v, v);
}
template <size_t Rows, typename ElemType = double>
ElemType Norm(const Vector<Rows, ElemType>& v) {
    return std::sqrt(NormSquared(v));
}

#endif /*__MATRIX_H__*/