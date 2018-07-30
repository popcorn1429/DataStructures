/*
 A Grid is a two-dimensional array of elements.
 The elements can be queried using the standard [] selection operators.
*/
#ifndef _grid_h_
#define _grid_h_
#include <vector>
#include <algorithm>

template <typename ElementType>
class Grid {
public:
    Grid();
    Grid(size_t numRows, size_t numCols);

    void clear();
    void resize(size_t width, size_t height);

    bool empty() const;
    size_t size() const;
    size_t numRows() const;
    size_t numCols() const;

    typename std::vector<ElementType>::reference getAt(size_t row, size_t col);
    typename std::vector<ElementType>::const_reference getAt(size_t row, size_t col) const;

    typedef typename std::vector<ElementType>::iterator iterator;
    typedef typename std::vector<ElementType>::const_iterator const_iterator;
    //using iterator = std::vector<ElementType>::iterator;
    //using const_iterator = std::vector<ElementType>::const_iterator;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    iterator row_begin(size_t row);
    iterator row_end(size_t row);
    const_iterator row_begin(size_t row) const;
    const_iterator row_end(size_t row) const;

    class MutableReference;
    class ImmutableReference;
    MutableReference operator[](size_t row);
    ImmutableReference operator[](size_t row) const;

    bool operator<(const Grid &other) const;
    bool operator<=(const Grid &other) const;
    bool operator==(const Grid &other) const;
    bool operator!=(const Grid &other) const;
    bool operator>=(const Grid &other) const;
    bool operator>(const Grid &other) const;

private:
    std::vector<ElementType> elements;
    size_t rows;
    size_t cols;
};

template <typename ElementType>
class Grid<ElementType>::MutableReference {
public:
    typename std::vector<ElementType>::reference operator[](size_t col);

private:
    MutableReference(Grid *source, size_t row);
    Grid  *ref;
    size_t row;

    friend class Grid;
};

template <typename ElementType>
class Grid<ElementType>::ImmutableReference {
public:
    typename std::vector<ElementType>::const_reference operator[](size_t col) const;

private:
    ImmutableReference(const Grid *souce, size_t row);
    const Grid *ref;
    size_t      row;

    friend class Grid;
};

template <typename ElementType>
Grid<ElementType>::Grid() : rows(0), cols(0) {}

template <typename ElementType>
Grid<ElementType>::Grid(size_t numRows, size_t numCols) : elements(numRows*numCols), rows(numRows), cols(numCols) {}

template <typename ElementType>
void Grid<ElementType>::clear() {
    elements.clear();
}

template <typename ElementType>
void Grid<ElementType>::resize(size_t width, size_t height) {
    elements.assign(width*height, ElementType());
    rows = width;
    cols = height;
}

template <typename ElementType>
bool Grid<ElementType>::empty() const {
    //return elements.empty();
    return 0 == size();
}

template <typename ElementType>
size_t Grid<ElementType>::size() const {
    //return elements.size();
    return rows * cols;
}

template <typename ElementType>
size_t Grid<ElementType>::numRows() const {
    return rows;
}

template <typename ElementType>
size_t Grid<ElementType>::numCols() const {
    return cols;
}

template <typename ElementType>
typename std::vector<ElementType>::reference Grid<ElementType>::getAt(size_t row, size_t col) {
    return elements[row*cols + col];
}

template <typename ElementType>
typename std::vector<ElementType>::const_reference Grid<ElementType>::getAt(size_t row, size_t col) const {
    return elements[row*cols + col];
}

template <typename ElementType>
typename Grid<ElementType>::iterator Grid<ElementType>::begin() {
    return elements.begin();
}

template <typename ElementType>
typename Grid<ElementType>::iterator Grid<ElementType>::end() {
    return elements.end();
}

template <typename ElementType>
typename Grid<ElementType>::const_iterator Grid<ElementType>::begin() const {
    return elements.begin();
}

template <typename ElementType>
typename Grid<ElementType>::const_iterator Grid<ElementType>::end() const {
    return elements.end();
}

template <typename ElementType>
typename Grid<ElementType>::iterator Grid<ElementType>::row_begin(size_t row) {
    return elements.begin() + row*cols;
}

template <typename ElementType>
typename Grid<ElementType>::iterator Grid<ElementType>::row_end(size_t row) {
    return row_begin(row) + cols;
}

template <typename ElementType>
typename Grid<ElementType>::const_iterator Grid<ElementType>::row_begin(size_t row) const {
    return elements.begin() + row*cols;
}

template <typename ElementType>
typename Grid<ElementType>::const_iterator Grid<ElementType>::row_end(size_t row) const {
    return row_begin(row) + cols;
}

template <typename ElementType>
Grid<ElementType>::MutableReference::MutableReference(Grid *source, size_t row) : ref(source), row(row) {}

template <typename ElementType>
typename std::vector<ElementType>::reference Grid<ElementType>::MutableReference::operator[](size_t col) {
    return ref->getAt(row, col);
}

template <typename ElementType>
Grid<ElementType>::ImmutableReference::ImmutableReference(const Grid *source, size_t row) : ref(source), row(row) {}

template <typename ElementType>
typename std::vector<ElementType>::const_reference Grid<ElementType>::ImmutableReference::operator[](size_t col) const {
    return ref->getAt(row, col);
}

template <typename ElementType>
typename Grid<ElementType>::MutableReference Grid<ElementType>::operator[](size_t row) {
    return MutableReference(this, row);
}

template <typename ElementType>
typename Grid<ElementType>::ImmutableReference Grid<ElementType>::operator[](size_t row) const {
    return ImmutableReference(this, row);
}

template <typename ElementType>
bool Grid<ElementType>::operator<(const Grid &other) const {
    if (rows != other.rows)
        return rows < other.rows;
    if (cols != other.cols)
        return cols < other.cols;

    return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
}

template <typename ElementType>
bool Grid<ElementType>::operator<=(const Grid &other) const {
    return !(other < *this);
}

template <typename ElementType>
bool Grid<ElementType>::operator==(const Grid &other) const {
    return !(*this < other) && !(other < *this);
}

template <typename ElementType>
bool Grid<ElementType>::operator!=(const Grid &other) const {
    return (*this < other) || (other < *this);
}

template <typename ElementType>
bool Grid<ElementType>::operator>=(const Grid &other) const {
    return !(*this < other);
}

template <typename ElementType>
bool Grid<ElementType>::operator>(const Grid &other) const {
    return (other < *this);
}

#endif /*_grid_h_*/