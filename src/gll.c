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
#include <stdio.h>
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
    gll_size_t size;
    gll_node_t* head;
    gll_node_t* tail;
} gll_list_t;

typedef struct gll_iterator
{
    gll_list_t* list;
    gll_node_t* current;
} gll_iterator_t;

static gll_result_t _gll_comparator_data(gll_data_t data1, gll_data_t data2);
static gll_status_t _gll_insert_from_head(gll_list_t* list, gll_index_t index, gll_data_t data);
static gll_status_t _gll_insert_from_tail(gll_list_t* list, gll_index_t index, gll_data_t data);
static gll_data_t _gll_remove_from_head(gll_list_t* list, gll_index_t index);
static gll_data_t _gll_remove_from_tail(gll_list_t* list, gll_index_t index);

gll_data_t _gll_char_to_data(char data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_schar_to_data(signed char data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_uchar_to_data(unsigned char data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_short_to_data(short data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_int_to_data(int data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_uint_to_data(unsigned int data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_long_to_data(long data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_ulong_to_data(unsigned long data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_longlong_to_data(long long data)
{
    return (gll_data_t) data;
}

gll_data_t _gll_ulonglong_to_data(unsigned long long data)
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

gll_data_t _gll_voidptr_to_data(void* data)
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
    gll_size_t size = gll_size(list);
    
    for(gll_index_t i = 0 ; i < size ; i++)
    {
        gll_append(clone, gll_iterator_next(iterator));
    }
    
    gll_iterator_destroy(iterator);
    return clone;
}

gll_status_t gll_destroy(gll_list_t* list, gll_deallocator_t deallocator)
{
    if (gll_clear(list, deallocator)) return -1;
    
    free(list);
    return 0;
}

gll_status_t gll_append(gll_list_t* list, gll_data_t data)
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

    list->size++;
    return 0;
}

gll_status_t gll_push(gll_list_t* list, gll_data_t data)
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

    list->size++;
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
    list->size--;
    
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
    list->size--;
    
    free(node);
    
    return data;
}

gll_size_t gll_size(gll_list_t* list)
{
    if (!list) return 0;
    return list->size;
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

gll_index_t gll_find(gll_list_t* list, gll_data_t data, gll_comparator_t comparator)
{
    if (!list) return 0;
    if (!list->head) return 0;

    if (!comparator)
    {
        comparator = _gll_comparator_data;
    }

    gll_iterator_t* iterator = gll_iterator_create(list);
    gll_index_t index = 0;

    for (; index < list->size; index++)
    {
        gll_data_t eval = gll_iterator_next(iterator);

        if (!comparator(data, eval))
        {
            break;
        }
    }

    gll_iterator_destroy(iterator);

    return index;
}

gll_status_t gll_insert(gll_list_t* list, gll_index_t index, gll_data_t data)
{
    if (!list) return -1;
    if (index > list->size) return -1;

    gll_status_t status = 0;

    if (index == 0)
    {
        // Insert at the head
        status = gll_push(list, data);
    }
    else if (index == list->size)
    {
        // Insert at the tail
        status = gll_append(list, data);
    }
    else if (list->size/2 >= index)
    {
        // Index is in the first half
        status = _gll_insert_from_head(list, index, data);
    }
    else
    {
        // Index is in the second half
        status = _gll_insert_from_tail(list, index, data);
    }

    return status;
}

gll_data_t gll_remove(gll_list_t* list, gll_index_t index)
{
    if (!list) return -1;
    if (index > list->size) return -1;

    gll_data_t data = 0;

    if (index == 0)
    {
        // Remove at the head
        data = gll_pop(list);
    }
    else if (index == list->size)
    {
        // Remove at the tail
        data = gll_trim(list);
    }
    else if (list->size/2 >= index)
    {
        // Index is in the first half
        data = _gll_remove_from_head(list, index);
    }
    else
    {
        // Index is in the second half
        data = _gll_remove_from_tail(list, index);
    }

    return data;
}

gll_status_t gll_clear(gll_list_t* list, gll_deallocator_t deallocator)
{
    if (!list) return -1;
    
    gll_node_t* node = list->head;
    
    for (gll_index_t i = 0; i < list->size; i++)
    {
        if (!node) return -1;
        gll_node_t* next = node->next;

        if(deallocator && node->data)
        {
            deallocator(node->data);
        }

        free(node);
        node = next;
    }
    
    list->size = 0;
    return 0;
}

gll_iterator_t* gll_iterator_create(gll_list_t* list)
{
    if (!list) return NULL;
    
    gll_iterator_t* iterator = calloc(1, sizeof(gll_iterator_t));
    iterator->list = list;
}

gll_status_t gll_iterator_destroy(gll_iterator_t* iterator)
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

gll_status_t gll_iterator_reset(gll_iterator_t* iterator)
{
    if (!iterator) return -1;
    
    iterator->current = NULL;
    return 0;
}

gll_result_t gll_comparator_int32(gll_data_t data1, gll_data_t data2)
{
    return (int32_t) data1 - (int32_t) data2;
}

gll_result_t gll_comparator_uint32(gll_data_t data1, gll_data_t data2)
{
    if ((uint32_t) data1 > (uint32_t) data2)
    {
        return 1;
    }
    else if ((uint32_t) data1 < (uint32_t) data2)
    {
        return -1;
    }
    
    return 0;
}

gll_result_t gll_comparator_float(gll_data_t data1, gll_data_t data2)
{
    if (GLL_FLOAT(data1) > GLL_FLOAT(data2))
    {
        return 1;
    }
    else if (GLL_FLOAT(data1) < GLL_FLOAT(data2))
    {
        return -1;
    }
    
    return 0;
}

gll_result_t gll_comparator_double(gll_data_t data1, gll_data_t data2)
{
    if (GLL_DOUBLE(data1) > GLL_DOUBLE(data2))
    {
        return 1;
    }
    else if (GLL_DOUBLE(data1) < GLL_DOUBLE(data2))
    {
        return -1;
    }
    
    return 0;
}

static gll_result_t _gll_comparator_data(gll_data_t data1, gll_data_t data2)
{
    if (data1 > data2)
    {
        return 1;
    }
    else if (data1 < data2)
    {
        return -1;
    }
    
    return 0;
}

static gll_status_t _gll_insert_from_head(gll_list_t* list, gll_index_t index, gll_data_t data)
{
    gll_iterator_t* iterator = gll_iterator_create(list);

    for (gll_index_t i = 0; i <= index; i++)
    {
        gll_iterator_next(iterator);
    }

    gll_node_t* node = calloc(1, sizeof(gll_node_t));
    node->data = data;

    node->next = iterator->current;
    node->prev = iterator->current->prev;
    iterator->current->prev->next = node;
    iterator->current->prev = node;
    list->size++;

    gll_iterator_destroy(iterator);

    return 0;
}

static gll_status_t _gll_insert_from_tail(gll_list_t* list, gll_index_t index, gll_data_t data)
{
    gll_iterator_t* iterator = gll_iterator_create(list);

    for (gll_index_t i = list->size - 1; i >= index; i--)
    {
        gll_iterator_prev(iterator);
    }

    gll_node_t* node = calloc(1, sizeof(gll_node_t));
    node->data = data;

    node->next = iterator->current;
    node->prev = iterator->current->prev;
    iterator->current->prev->next = node;
    iterator->current->prev = node;
    list->size++;

    gll_iterator_destroy(iterator);

    return 0;
}

static gll_data_t _gll_remove_from_head(gll_list_t* list, gll_index_t index)
{
    gll_data_t data = 0;
    gll_iterator_t* iterator = gll_iterator_create(list);

    for (gll_index_t i = 0; i <= index; i++)
    {
        gll_iterator_next(iterator);
    }

    iterator->current->prev->next = iterator->current->next;
    iterator->current->next->prev = iterator->current->prev;
    list->size--;

    data = iterator->current->data;

    gll_iterator_destroy(iterator);

    return data;
}

static gll_data_t _gll_remove_from_tail(gll_list_t* list, gll_index_t index)
{
    gll_data_t data = 0;
    gll_iterator_t* iterator = gll_iterator_create(list);

    for (gll_index_t i = list->size - 1; i >= index; i--)
    {
        gll_iterator_prev(iterator);
    }

    iterator->current->prev->next = iterator->current->next;
    iterator->current->next->prev = iterator->current->prev;
    list->size--;

    data = iterator->current->data;

    gll_iterator_destroy(iterator);

    return data;
}
