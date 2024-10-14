/*
 * gll.h - Generic Linked List interface
 * 
 * Author: Laurent Mailloux-Bourassa
 * 
 * This file is part of the Generic Linked List (GLL) library.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef GLL_H
#define GLL_H

#include <stdint.h>

#define GLL_DATA(data)  ((gll_data_t) data)  // Macro to cast data to gll_data_t type.

typedef struct gll_list gll_list_t;          // Opaque type representing the doubly linked list.
typedef struct gll_iterator gll_iterator_t;  // Opaque type representing an iterator for the list.
typedef uintptr_t gll_data_t;                // Type representing the data stored in the list nodes.

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
 * @brief Deletes the entire list and frees all allocated memory.
 * 
 * @param list The list to delete.
 * @return 0 on success, -1 on failure.
 */
int8_t gll_delete(gll_list_t* list);

/**
 * @brief Appends data to the end of the list.
 * 
 * @param list The list to append to.
 * @param data The data to append.
 * @return 0 on success, -1 on failure.
 */
int8_t gll_append(gll_list_t* list, gll_data_t data);

/**
 * @brief Pushes data to the front of the list.
 * 
 * @param list The list to push data into.
 * @param data The data to push.
 * @return 0 on success, -1 on failure.
 */
int8_t gll_push(gll_list_t* list, gll_data_t data);

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
uint32_t gll_size(gll_list_t* list);

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
 * @brief Clears all elements from the list but keeps the list structure.
 * 
 * @param list The list to clear.
 * @return 0 on success, -1 on failure.
 */
int8_t gll_clear(gll_list_t* list);

/**
 * @brief Creates an iterator for traversing the list.
 * 
 * @param list The list to create an iterator for.
 * @return Pointer to the created iterator or NULL on failure.
 */
gll_iterator_t* gll_iterator_create(gll_list_t* list);

/**
 * @brief Deletes the iterator and frees memory.
 * 
 * @param iterator The iterator to delete.
 * @return 0 on success, -1 on failure.
 */
int8_t gll_iterator_delete(gll_iterator_t* iterator);

/**
 * @brief Moves the iterator to the next element in the list and returns its data.
 * 
 * @param iterator The iterator to advance.
 * @return The data of the next element or 0 if the iterator is NULL.
 */
gll_data_t gll_iterator_next(gll_iterator_t* iterator);

/**
 * @brief Moves the iterator to the previous element in the list and returns its data.
 * 
 * @param iterator The iterator to reverse.
 * @return The data of the previous element or 0 if the iterator is NULL.
 */
gll_data_t gll_iterator_prev(gll_iterator_t* iterator);

/**
 * @brief Resets the iterator to the start of the list.
 * 
 * This function moves the iterator back to the first element (head) of the list,
 * allowing for a new traversal from the beginning.
 * 
 * @param iterator The iterator to reset.
 * @return 0 on success, -1 if the iterator is NULL or invalid.
 */
int8_t gll_iterator_reset(gll_iterator_t* iterator);

#endif /* GLL_H */