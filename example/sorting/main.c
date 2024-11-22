/*
 * main.c - Example program demonstrating the sorting feature of the Generic Linked List (GLL) library.
 * 
 * Copyright (c) 2024 Laurent Mailloux-Bourassa
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

// Function to print the list
void print_list(gll_list_t* list)
{
    gll_iterator_t* iterator = gll_iterator_create(list);
    gll_data_t data;

    printf("List: ");
    while (gll_iterator_forward(iterator, &data))
    {
        printf("%d ", (int) data);
    }
    printf("\n");

    gll_iterator_destroy(iterator);
}

int main() {
    // Create a new list
    gll_cfg_t cfg = {.comparator = gll_comparator_int32, .deallocator = NULL};
    gll_list_t* list = gll_create(&cfg);

    // Add some unsorted integers to the list
    gll_append(list, GLL_DATA(5));
    gll_append(list, GLL_DATA(2));
    gll_append(list, GLL_DATA(9));
    gll_append(list, GLL_DATA(1));
    gll_append(list, GLL_DATA(6));
    gll_append(list, GLL_DATA(-3));
    gll_append(list, GLL_DATA(12));
    gll_append(list, GLL_DATA(-8));
    gll_append(list, GLL_DATA(4));
    gll_append(list, GLL_DATA(-1));
    gll_append(list, GLL_DATA(15));
    gll_append(list, GLL_DATA(0));
    gll_append(list, GLL_DATA(-10));
    gll_append(list, GLL_DATA(8));
    gll_append(list, GLL_DATA(7));
    gll_append(list, GLL_DATA(-5));
    gll_append(list, GLL_DATA(3));
    gll_append(list, GLL_DATA(11));
    gll_append(list, GLL_DATA(-6));
    gll_append(list, GLL_DATA(13));

    // Print the list before sorting
    printf("Before sorting:\n");
    print_list(list);

    // Sort the list
    gll_sort(list);

    // Print the list after sorting
    printf("After sorting:\n");
    print_list(list);

    // Clean up
    gll_destroy(list);

    return 0;
}
