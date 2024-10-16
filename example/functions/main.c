/*
 * main.c - Example program demonstrating the use of the Generic Linked List (GLL) library.
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

    // Pop element from the front
    gll_data_t popped = gll_pop(list);
    printf("Popped from front: %lu\n", (unsigned long)popped);
    print_list(list);

    // Trim element from the end
    gll_data_t trimmed = gll_trim(list);
    printf("Trimmed from end: %lu\n", (unsigned long)trimmed);
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
    gll_clear(cloned_list);
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
    gll_delete(list);
    gll_delete(cloned_list);

    printf("All lists and iterators have been deleted.\n");
    
    return 0;
}
