/*
 * main.c - Example program demonstrating the use of the Generic Linked List (GLL) library with different data types.
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
#include <stdlib.h>
#include <string.h>
#include "gll.h"

// Custom structure definition
typedef struct person
{
    int id;
    char name[50];
} person_t;

void person_deallocator(gll_data_t data)
{
    free((person_t*) data);
}

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
        print_func(gll_iterator_forward(it));
    }
    printf("\n");
    gll_iterator_destroy(it);
}

// Print functions for different data types
void print_int(gll_data_t data)
{
    printf("%d ", data);
}

void print_float(gll_data_t data)
{
    printf("%.2f ", GLL_FLOAT(data));
}

void print_string(gll_data_t data)
{
    printf("%s ", (char*) data);
}

void print_person_wrapper(gll_data_t data)
{
    print_person((person_t*) data);
}

int main(void)
{
    // 1. Demonstrating usage with integers
    gll_list_t* int_list = gll_create(NULL);
    gll_append(int_list, GLL_DATA(10));
    gll_append(int_list, GLL_DATA(20));
    gll_append(int_list, GLL_DATA(30));
    printf("Integer list:\n");
    print_list(int_list, print_int);
    gll_destroy(int_list);

    // 2. Demonstrating usage with floats
    gll_list_t* float_list = gll_create(NULL);
    gll_append(float_list, GLL_DATA(1.5f));
    gll_append(float_list, GLL_DATA(2.75f));
    gll_append(float_list, GLL_DATA(3.14f));
    printf("Float list:\n");
    print_list(float_list, print_float);
    gll_destroy(float_list);

    // 3. Demonstrating usage with strings
    gll_list_t* string_list = gll_create(NULL);
    gll_append(string_list, GLL_DATA("Hello"));
    gll_append(string_list, GLL_DATA("World"));
    gll_append(string_list, GLL_DATA("GLL"));
    printf("String list:\n");
    print_list(string_list, print_string);
    gll_destroy(string_list);

    // 4. Demonstrating usage with custom structures
    gll_cfg_t person_list_cfg = {.comparator = NULL, .deallocator = person_deallocator};
    gll_list_t* person_list = gll_create(&person_list_cfg);
    
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

    // Deleting the person list
    gll_destroy(person_list);

    printf("All lists have been processed and deleted.\n");
    
    return 0;
}
