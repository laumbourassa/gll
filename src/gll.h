/*
 * gll.h - Generic Linked List interface
 * 
 * Author: Laurent Mailloux-Bourassa
 * 
 * This file is part of the Generic Linked List (GLL) library.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GLL_H
#define GLL_H

#include <stdint.h>

typedef struct gll_list gll_list_t;          // Opaque type representing the doubly linked list.
typedef struct gll_iterator gll_iterator_t;  // Opaque type representing an iterator for the list.
typedef uintptr_t gll_data_t;                // Type representing the data stored in the list nodes.
typedef int8_t gll_status_t;
typedef int32_t gll_result_t;
typedef size_t gll_size_t;
typedef size_t gll_index_t;

typedef void (*gll_deallocator_t)(gll_data_t data);                             // User-provided deallocator function for custom structures
typedef gll_result_t (*gll_comparator_t)(gll_data_t data1, gll_data_t data2);   // User-provided comparator function for custom structures
                                                                                // Returns positive value if data1 > data2, negative if data1 < data2, 0 if equal

// Conversion functions for various types to gll_data_t
gll_data_t _gll_char_to_data(char data);
gll_data_t _gll_schar_to_data(signed char data);
gll_data_t _gll_uchar_to_data(unsigned char data);
gll_data_t _gll_short_to_data(short data);
gll_data_t _gll_int_to_data(int data);
gll_data_t _gll_uint_to_data(unsigned int data);
gll_data_t _gll_long_to_data(long data);
gll_data_t _gll_ulong_to_data(unsigned long data);
gll_data_t _gll_longlong_to_data(long long data);
gll_data_t _gll_ulonglong_to_data(unsigned long long data);
gll_data_t _gll_float_to_data(float data);
gll_data_t _gll_double_to_data(double data);
//gll_data_t _gll_longdouble_to_data(long double data); // Long double can be larger than gll_data_t
gll_data_t _gll_voidptr_to_data(void* data);

#define GLL_DATA(data)  _Generic((data),                \
        char: _gll_char_to_data,                        \
        signed char: _gll_schar_to_data,                \
        unsigned char: _gll_uchar_to_data,              \
        short: _gll_short_to_data,                      \
        int: _gll_int_to_data,                          \
        unsigned int: _gll_uint_to_data,                \
        long: _gll_long_to_data,                        \
        unsigned long: _gll_ulong_to_data,              \
        long long: _gll_longlong_to_data,               \
        unsigned long long: _gll_ulonglong_to_data,     \
        float: _gll_float_to_data,                      \
        double: _gll_double_to_data,                    \
        default: _gll_voidptr_to_data                   \
        )(data)

#define GLL_FLOAT(data)         (*(float*) &data)
#define GLL_DOUBLE(data)        (*(double*) &data)

/**
 * @brief Creates a new doubly linked list.
 * 
 * @return Pointer to the created gll_list_t or NULL on failure.
 */
gll_list_t* gll_create(void);

/**
 * @brief Clones an existing list by creating a copy.
 * 
 * @param list The list to clone.
 * @return Pointer to the cloned gll_list_t or NULL on failure.
 */
gll_list_t* gll_clone(gll_list_t* list);

/**
 * @brief Destroys the entire list and frees all allocated memory.
 * 
 * @param list The list to destroy.
 * @param deallocator Function to deallocate any custom data, or NULL.
 * @return 0 on success, -1 on failure.
 */
gll_status_t gll_destroy(gll_list_t* list, gll_deallocator_t deallocator);

/**
 * @brief Appends data to the end of the list.
 * 
 * @param list The list to append to.
 * @param data The data to append.
 * @return 0 on success, -1 on failure.
 */
gll_status_t gll_append(gll_list_t* list, gll_data_t data);

/**
 * @brief Pushes data to the front of the list.
 * 
 * @param list The list to push data into.
 * @param data The data to push.
 * @return 0 on success, -1 on failure.
 */
gll_status_t gll_push(gll_list_t* list, gll_data_t data);

/**
 * @brief Removes and returns data from the front of the list.
 * 
 * @param list The list to pop data from.
 * @return The popped data or 0 if the list is empty or NULL.
 */
gll_data_t gll_pop(gll_list_t* list);

/**
 * @brief Removes and returns data from the end of the list.
 * 
 * @param list The list to trim.
 * @return The trimmed data or 0 if the list is empty or NULL.
 */
gll_data_t gll_trim(gll_list_t* list);

/**
 * @brief Returns the number of elements in the list.
 * 
 * @param list The list to get the size of.
 * @return The number of elements in the list, or 0 if the list is NULL.
 */
gll_size_t gll_size(gll_list_t* list);

/**
 * @brief Returns the data at the front of the list without removing it.
 * 
 * @param list The list to peek at.
 * @return The data at the front, or 0 if the list is empty or NULL.
 */
gll_data_t gll_peek(gll_list_t* list);

/**
 * @brief Returns the data at the end of the list without removing it.
 * 
 * @param list The list to peek at the end.
 * @return The data at the end, or 0 if the list is empty or NULL.
 */
gll_data_t gll_peek_last(gll_list_t* list);

/**
 * @brief Finds the index of the first occurrence of data in the list.
 * 
 * @param list The list to search.
 * @param data The data to find.
 * @param comparator Function to compare custom data. If NULL, data will be compared directly.
 * @return The index of the found data, 0 if NULL list, list size if not found.
 */
gll_index_t gll_find(gll_list_t* list, gll_data_t data, gll_comparator_t comparator);

/**
 * @brief Inserts data at a specified index in the list.
 * 
 * @param list The list to insert data into.
 * @param index The index where data should be inserted.
 * @param data The data to insert.
 * @return 0 on success, -1 on failure.
 */
gll_status_t gll_insert(gll_list_t* list , gll_index_t index, gll_data_t data);

/**
 * @brief Removes and returns the data at a specified index in the list.
 * 
 * @param list The list to remove data from.
 * @param index The index of the data to remove.
 * @return The removed data or 0 if the list is empty or the index is invalid.
 */
gll_data_t gll_remove(gll_list_t* list, gll_index_t index);

/**
 * @brief Clears all elements from the list but keeps the list structure.
 * 
 * @param list The list to clear.
 * @param deallocator Function to deallocate any custom data, or NULL.
 * @return 0 on success, -1 on failure.
 */
gll_status_t gll_clear(gll_list_t* list, gll_deallocator_t deallocator);

/**
 * @brief Sorts all elements from the list.
 * 
 * @param list The list to sort.
 * @param comparator Function to compare custom data. If NULL, data will be compared directly.
 * @return 0 on success, -1 on failure.
 */
gll_status_t gll_sort(gll_list_t* list, gll_comparator_t comparator);

/**
 * @brief Creates an iterator for traversing the list.
 * 
 * @param list The list to create an iterator for.
 * @return Pointer to the created iterator or NULL on failure.
 */
gll_iterator_t* gll_iterator_create(gll_list_t* list);

/**
 * @brief Destroys the iterator and frees memory.
 * 
 * @param iterator The iterator to destroy.
 * @return 0 on success, -1 on failure.
 */
gll_status_t gll_iterator_destroy(gll_iterator_t* iterator);

/**
 * @brief Moves the iterator to the next element in the list and returns its data.
 * 
 * @param iterator The iterator to advance.
 * @return The data of the next element or 0 if the iterator is NULL.
 */
gll_data_t gll_iterator_forward(gll_iterator_t* iterator);

/**
 * @brief Moves the iterator to the previous element in the list and returns its data.
 * 
 * @param iterator The iterator to reverse.
 * @return The data of the previous element or 0 if the iterator is NULL.
 */
gll_data_t gll_iterator_backward(gll_iterator_t* iterator);

/**
 * @brief Resets the iterator to the start of the list.
 * 
 * @param iterator The iterator to reset.
 * @return 0 on success, -1 if the iterator is NULL or invalid.
 */
gll_status_t gll_iterator_reset(gll_iterator_t* iterator);

/**
 * @brief Compares two 32-bit integers.
 * 
 * @param data1 The first data value to compare.
 * @param data2 The second data value to compare.
 * @return Positive if data1 > data2, negative if data1 < data2, 0 if equal.
 */
gll_result_t gll_comparator_int32(gll_data_t data1, gll_data_t data2);

/**
 * @brief Compares two 32-bit unsigned integers.
 * 
 * @param data1 The first data value to compare.
 * @param data2 The second data value to compare.
 * @return Positive if data1 > data2, negative if data1 < data2, 0 if equal.
 */
gll_result_t gll_comparator_uint32(gll_data_t data1, gll_data_t data2);

/**
 * @brief Compares two floating-point values.
 * 
 * @param data1 The first data value to compare.
 * @param data2 The second data value to compare.
 * @return Positive if data1 > data2, negative if data1 < data2, 0 if equal.
 */
gll_result_t gll_comparator_float(gll_data_t data1, gll_data_t data2);

/**
 * @brief Compares two double-precision floating-point values.
 * 
 * @param data1 The first data value to compare.
 * @param data2 The second data value to compare.
 * @return Positive if data1 > data2, negative if data1 < data2, 0 if equal.
 */
gll_result_t gll_comparator_double(gll_data_t data1, gll_data_t data2);

/**
 * @brief Compares two strings alphabetically.
 * 
 * @param data1 The first data value to compare.
 * @param data2 The second data value to compare.
 * @return Positive if data1 > data2, negative if data1 < data2, 0 if equal.
 */
gll_result_t gll_comparator_alphabetical(gll_data_t data1, gll_data_t data2);

#endif /* GLL_H */
