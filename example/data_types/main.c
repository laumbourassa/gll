/*
 * main.c - Example program demonstrating the use of the Generic Linked List (GLL) library with different data types.
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
#include <stdlib.h>
#include <string.h>
#include "gll.h"

// Custom structure definition
typedef struct person
{
    int id;
    char name[50];
} person_t;

// Helper function to print the custom structure
void print_person(person_t* person)
{
    printf("Person ID: %d, Name: %s\n", person->id, person->name);
}

// Function to print the list elements
void print_list(gll_list_t* list, void (*print_func)(gll_data_t))
{
    gll_iterator_t* it = gll_iterator_create(list);
    printf("List: ");
    for (uint32_t i = 0; i < gll_size(list); i++)
    {
        print_func(gll_iterator_next(it));
    }
    printf("\n");
    gll_iterator_delete(it);
}

// Print functions for different data types
void print_int(gll_data_t data)
{
    printf("%d ", (int)data);
}

void print_float(gll_data_t data)
{
    printf("%.2f ", *(float*)data);
}

void print_string(gll_data_t data)
{
    printf("%s ", (char*)data);
}

void print_person_wrapper(gll_data_t data)
{
    print_person((person_t*)data);
}

int main(void)
{
    // 1. Demonstrating usage with integers
    gll_list_t* int_list = gll_create();
    gll_append(int_list, GLL_DATA(10));
    gll_append(int_list, GLL_DATA(20));
    gll_append(int_list, GLL_DATA(30));
    printf("Integer list:\n");
    print_list(int_list, print_int);
    gll_delete(int_list);

    // 2. Demonstrating usage with floats
    gll_list_t* float_list = gll_create();
    float f1 = 1.5, f2 = 2.75, f3 = 3.14;
    gll_append(float_list, GLL_DATA(&f1));
    gll_append(float_list, GLL_DATA(&f2));
    gll_append(float_list, GLL_DATA(&f3));
    printf("Float list:\n");
    print_list(float_list, print_float);
    gll_delete(float_list);

    // 3. Demonstrating usage with strings
    gll_list_t* string_list = gll_create();
    gll_append(string_list, GLL_DATA("Hello"));
    gll_append(string_list, GLL_DATA("World"));
    gll_append(string_list, GLL_DATA("GLL"));
    printf("String list:\n");
    print_list(string_list, print_string);
    gll_delete(string_list);

    // 4. Demonstrating usage with custom structures
    gll_list_t* person_list = gll_create();
    
    // Creating custom structure instances
    person_t* p1 = (person_t*)malloc(sizeof(person_t));
    p1->id = 1;
    strcpy(p1->name, "Alice");

    person_t* p2 = (person_t*)malloc(sizeof(person_t));
    p2->id = 2;
    strcpy(p2->name, "Bob");

    person_t* p3 = (person_t*)malloc(sizeof(person_t));
    p3->id = 3;
    strcpy(p3->name, "Charlie");

    // Adding custom structures to the list using GLL_DATA macro
    gll_append(person_list, GLL_DATA(p1));
    gll_append(person_list, GLL_DATA(p2));
    gll_append(person_list, GLL_DATA(p3));
    
    printf("Custom structure list (Persons):\n");
    print_list(person_list, print_person_wrapper);
    
    // Free allocated memory for custom structures
    free(p1);
    free(p2);
    free(p3);

    // Deleting the person list
    gll_delete(person_list);

    printf("All lists have been processed and deleted.\n");
    
    return 0;
}
