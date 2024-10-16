/*
 * gll.c - Generic Linked List implementation
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

#include <stdlib.h>
#include "gll.h"

typedef struct gll_node gll_node_t;
typedef struct gll_node
{
    gll_data_t data;
    gll_node_t* next;
    gll_node_t* prev;
} gll_node_t;

typedef struct gll_list
{
    gll_size_t qty;
    gll_node_t* head;
    gll_node_t* tail;
} gll_list_t;

typedef struct gll_iterator
{
    gll_list_t* list;
    gll_node_t* current;
} gll_iterator_t;

gll_data_t _gll_int_to_data(int data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_long_to_data(long data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_float_to_data(float data)
{
    return *(gll_data_t*) &data;
}

gll_data_t _gll_double_to_data(double data)
{
    return *(gll_data_t*) &data;
}

gll_data_t _gll_unknown_to_data(void* data)
{
    return (gll_data_t) data;
}

gll_list_t* gll_create(void)
{
    return calloc(1, sizeof(gll_list_t));
}

gll_list_t* gll_clone(gll_list_t* list)
{
    if (!list) return NULL;
    
    gll_list_t* clone = calloc(1, sizeof(gll_list_t));
    gll_iterator_t* iterator = gll_iterator_create(list);
    uint32_t size = gll_size(list);
    
    for(uint32_t i = 0 ; i < size ; i++)
    {
        gll_append(clone, gll_iterator_next(iterator));
    }
    
    gll_iterator_delete(iterator);
    return clone;
}

gll_result_t gll_delete(gll_list_t* list)
{
    if (gll_clear(list)) return -1;
    
    free(list);
    return 0;
}

gll_result_t gll_append(gll_list_t* list, gll_data_t data)
{
    if (!list) return -1;
    
    gll_node_t* node = calloc(1, sizeof(gll_node_t));
    node->data = data;

    gll_node_t* tail = list->tail;
    list->tail = node;

    if (list->head)
    {
        tail->next = node;
    }
    else
    {
        list->head = node;
    }
    
    node->prev = tail;

    list->qty++;
    return 0;
}

gll_result_t gll_push(gll_list_t* list, gll_data_t data)
{
    if (!list) return -1;
    
    gll_node_t* node = calloc(1, sizeof(gll_node_t));
    node->data = data;

    gll_node_t* head = list->head;
    list->head = node;

    if (list->tail)
    {
        head->prev = node;
    }
    else
    {
        list->tail = node;
    }
    
    node->next = head;

    list->qty++;
    return 0;
}

gll_data_t gll_pop(gll_list_t* list)
{
    if (!list) return 0;
    
    gll_node_t* node = list->head;
    if (!node) return 0;
    
    gll_data_t data = node->data;
    list->head = node->next;
    list->head->prev = NULL;
    list->qty--;
    
    free(node);
    
    return data;
}

gll_data_t gll_trim(gll_list_t* list)
{
    if (!list) return 0;
    
    gll_node_t* node = list->tail;
    if (!node) return 0;
    
    gll_data_t data = node->data;
    list->tail = node->prev;
    list->tail->next = NULL;
    list->qty--;
    
    free(node);
    
    return data;
}

gll_size_t gll_size(gll_list_t* list)
{
    if (!list) return 0;
    return list->qty;
}

gll_data_t gll_peek(gll_list_t* list)
{
    if (!list) return 0;
    
    gll_node_t* node = list->head;
    if (!node) return 0;
    
    return node->data;
}

gll_data_t gll_peek_last(gll_list_t* list)
{
    if (!list) return 0;
    
    gll_node_t* node = list->tail;
    if (!node) return 0;
    
    return node->data;
}

gll_result_t gll_clear(gll_list_t* list)
{
    if (!list) return -1;
    
    gll_node_t* node = list->head;
    
    for (uint32_t i = 0; i < list->qty; i++)
    {
        if (!node) return -1;
        gll_node_t* next = node->next;
        free(node);
        node = next;
    }
    
    list->qty = 0;
    return 0;
}

gll_iterator_t* gll_iterator_create(gll_list_t* list)
{
    if (!list) return NULL;
    
    gll_iterator_t* iterator = calloc(1, sizeof(gll_iterator_t));
    iterator->list = list;
}

gll_result_t gll_iterator_delete(gll_iterator_t* iterator)
{
    if (!iterator) return -1;
    
    free(iterator);
    return 0;
}

gll_data_t gll_iterator_next(gll_iterator_t* iterator)
{
    if (!iterator) return 0;

    if (iterator->current && iterator->current->next)
    {
        iterator->current = iterator->current->next;
    }
    else
    {
        iterator->current = iterator->list->head;
    }

    gll_data_t data = iterator->current ? iterator->current->data : 0;
    
    return data;
}

gll_data_t gll_iterator_prev(gll_iterator_t* iterator)
{
    if (!iterator) return 0;

    if (iterator->current && iterator->current->prev)
    {
        iterator->current = iterator->current->prev;
    }
    else
    {
        iterator->current = iterator->list->tail;
    }

    gll_data_t data = iterator->current ? iterator->current->data : 0;
    
    return data;
}

gll_result_t gll_iterator_reset(gll_iterator_t* iterator)
{
    if (!iterator) return -1;
    
    iterator->current = NULL;
    return 0;
}
