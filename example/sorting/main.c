#include <stdio.h>
#include "gll.h"

// Function to print the list
void print_list(gll_list_t* list)
{
    gll_iterator_t* iterator = gll_iterator_create(list);
    gll_size_t size = gll_size(list);

    printf("List: ");
    for (gll_index_t i = 0; i < size; i++)
    {
        gll_data_t data = gll_iterator_next(iterator);
        printf("%d ", (int) data);
    }
    printf("\n");

    gll_iterator_destroy(iterator);
}

int main() {
    // Create a new list
    gll_list_t* list = gll_create();

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

    // Sort the list using gll_sort with gll_comparator_int32
    gll_sort(list, gll_comparator_int32);

    // Print the list after sorting
    printf("After sorting:\n");
    print_list(list);

    // Clean up
    gll_destroy(list, NULL);

    return 0;
}
