/*
 * main.c - Example program demonstrating the use of the Generic Linked List (GLL) library.
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

#include <stdio.h>
#include "gll.h"

void print_list(gll_list_t* list)
{
    gll_iterator_t* it = gll_iterator_create(list);
    printf("List: ");
    for (uint32_t i = 0; i < gll_size(list); i++)
    {
        printf("%lu ", (unsigned long)gll_iterator_next(it));
    }
    printf("\n");
    gll_iterator_delete(it);
}

int main(void) {
    // Create a new list
    gll_list_t* list = gll_create();
    if (!list)
    {
        printf("Failed to create list.\n");
        return -1;
    }
    printf("Created a new list.\n");

    // Append elements to the list
    gll_append(list, GLL_DATA(1));
    gll_append(list, GLL_DATA(2));
    gll_append(list, GLL_DATA(3));
    gll_append(list, GLL_DATA(4));
    printf("After appending 1, 2, 3, 4:\n");
    print_list(list);

    // Push elements to the front of the list
    gll_push(list, GLL_DATA(0));
    printf("After pushing 0 to the front:\n");
    print_list(list);

    gll_insert(list, 2, GLL_DATA(42));
    printf("After inserting 42 in index 2:\n");
    print_list(list);

    // Pop element from the front
    gll_data_t popped = gll_pop(list);
    printf("Popped from front: %lu\n", (unsigned long)popped);
    print_list(list);

    // Trim element from the end
    gll_data_t trimmed = gll_trim(list);
    printf("Trimmed from end: %lu\n", (unsigned long)trimmed);
    print_list(list);

    // Find element
    gll_index_t index = gll_find(list, GLL_DATA(42), NULL);
    printf("Found 42 at index: %lu\n", index);

    gll_data_t removed = gll_remove(list, 1);
    printf("Removed from index 1: %lu\n", (unsigned long)removed);
    print_list(list);

    // Peek at the first and last elements
    gll_data_t first = gll_peek(list);
    gll_data_t last = gll_peek_last(list);
    printf("Peek first: %lu, Peek last: %lu\n", (unsigned long)first, (unsigned long)last);

    // Clone the list
    gll_list_t* cloned_list = gll_clone(list);
    printf("Cloned list:\n");
    print_list(cloned_list);

    // Clear the cloned list
    gll_clear(cloned_list, NULL);
    printf("Cleared the cloned list:\n");
    print_list(cloned_list);

    // Create an iterator and traverse the list
    gll_iterator_t* iterator = gll_iterator_create(list);
    printf("Traverse list using iterator (forward):\n");
    for (uint32_t i = 0; i < gll_size(list); i++)
    {
        printf("%lu ", (unsigned long)gll_iterator_next(iterator));
    }
    printf("\n");

    printf("Reset the iterator\n");
    gll_iterator_reset(iterator);

    printf("Traverse list using iterator (backward):\n");
    for (uint32_t i = 0; i < gll_size(list); i++)
    {
        printf("%lu ", (unsigned long)gll_iterator_prev(iterator));
    }
    printf("\n");

    gll_iterator_delete(iterator);

    // Delete the lists
    gll_delete(list, NULL);
    gll_delete(cloned_list, NULL);

    printf("All lists and iterators have been deleted.\n");
    
    return 0;
}
