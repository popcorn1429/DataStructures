/*
* The Levenshtein distance, or "edit distance," is the minimum
* number of edits required to transform one string into another string,
* assuming the only legal edits are insertions, deletions, and substitutions.
* As an example (borrowed from Wikipedia), converting "kitten" into "sitting"
* could be done by transforming:
*
*                      kitten -> sitten -> sittin -> sitting
*
* For an edit distance of three.
*
* More details : https://en.wikipedia.org/wiki/Levenshtein_distance
*
*/

#ifndef _levenshtein_distance_h_
#define _levenshtein_distance_h_

#include "grid.h"
#include <iterator>  //import distance
#include <algorithm> //import min

template <typename ForwardIterator1, typename ForwardIterator2>
size_t LevenshteinDistance(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
    const size_t m = size_t(std::distance(first1, last1));
    const size_t n = size_t(std::distance(first2, last2));

    Grid<size_t> distances(m + 1, n + 1);
    for (size_t i = 0; i < distances.numRows(); ++i)
        distances[i][0] = i;
    for (size_t j = 0; j < distances.numCols(); ++j)
        distances[0][j] = j;

    size_t i = 1;
    for (ForwardIterator1 itr1 = first1; itr1 != last1; ++itr1, ++i) {
        size_t j = 1;
        for (ForwardIterator2 itr2 = first2; itr2 != last2; ++itr2, ++j) {
            if (*itr1 == *itr2)
                distances[i][j] = distances[i - 1][j - 1];
            else
                distances[i][j] = 1 + std::min({ distances[i][j - 1], distances[i - 1][j], distances[i - 1][j - 1] });
        }
    }
    return distances[m][n];
}


#endif /*_levenshtein_distance_h_*/