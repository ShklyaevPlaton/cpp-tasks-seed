#ifndef SORTING_H
#define SORTING_H

// Header-only

// Для swap
#include "collvalue.h"

// ----------------------------- Пузырёк --------------------------------------
template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end)
{
    if (begin == end) return;

    for (Iterator i = begin; i != end; ++i) {
        for (Iterator j = begin; j + 1 != end; ++j) {
            if (*(j + 1) < *j) {
                std::swap(*j, *(j + 1));
            }
        }
    }
}

// --------------------------- QuickSort ---------------------------------------
template <typename Iterator>
Iterator partition(Iterator begin, Iterator end)
{
    auto pivot = *(end - 1);
    Iterator i = begin;

    for (Iterator j = begin; j != end - 1; ++j) {
        if (*j < pivot) {
            std::swap(*i, *j);
            ++i;
        }
    }

    std::swap(*i, *(end - 1));
    return i;
}

template <typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    if (end - begin <= 1) return;

    Iterator pivot = partition(begin, end);

    quick_sort(begin, pivot);
    quick_sort(pivot + 1, end);
}

#endif // SORTING_H
