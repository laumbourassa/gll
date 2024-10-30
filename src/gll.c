/*
 * gll.c - Generic Linked List implementation
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

#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
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
    gll_comparator_t comparator;
    gll_deallocator_t deallocator;
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
static gll_status_t _gll_insert(gll_list_t* list, gll_index_t index, gll_data_t data, bool backward);
static gll_data_t _gll_remove(gll_list_t* list, gll_index_t index, bool backward);
static gll_node_t* _gll_split_list(gll_node_t* head);
static gll_node_t* _gll_sorted_merge(gll_node_t* a, gll_node_t* b, gll_comparator_t comparator);
static gll_node_t* _gll_merge_sort(gll_node_t* head, gll_comparator_t comparator);

gll_list_t* gll_create(gll_cfg_t* cfg)
{
    gll_comparator_t comparator;
    gll_deallocator_t deallocator;

    if (cfg)
    {
        comparator = cfg->comparator ? cfg->comparator : _gll_comparator_data;
        deallocator = cfg->deallocator;
    }
    else
    {
        comparator = _gll_comparator_data;
        deallocator = NULL;
    }

    gll_list_t* list = calloc(1, sizeof(gll_list_t));

    if (list)
    {
        list->comparator = comparator;
        list->deallocator = deallocator;
    }

    return list;
}

gll_list_t* gll_clone(gll_list_t* list)
{
    if (!list) return NULL;
    
    gll_cfg_t cfg = {.comparator = list->comparator, .deallocator = list->deallocator};
    gll_list_t* clone = gll_create(&cfg);

    if (!clone) return NULL;

    gll_iterator_t* iterator = gll_iterator_create(list);
    gll_size_t size = gll_size(list);
    
    for(gll_index_t i = 0 ; i < size ; i++)
    {
        gll_append(clone, gll_iterator_forward(iterator));
    }
    
    gll_iterator_destroy(iterator);
    return clone;
}

gll_status_t gll_destroy(gll_list_t* list)
{
    if (gll_clear(list)) return -1;
    
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

gll_index_t gll_find(gll_list_t* list, gll_data_t data)
{
    if (!list) return 0;
    if (!list->head) return 0;

    gll_iterator_t* iterator = gll_iterator_create(list);
    gll_index_t index = 0;

    for (; index < list->size; index++)
    {
        gll_data_t eval = gll_iterator_forward(iterator);

        if (!list->comparator(data, eval))
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
        status = _gll_insert(list, index, data, false);
    }
    else
    {
        // Index is in the second half
        status = _gll_insert(list, index, data, true);
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
        data = _gll_remove(list, index, false);
    }
    else
    {
        // Index is in the second half
        data = _gll_remove(list, index, true);
    }

    return data;
}

gll_status_t gll_clear(gll_list_t* list)
{
    if (!list) return -1;
    
    gll_node_t* node = list->head;
    
    for (gll_index_t i = 0; i < list->size; i++)
    {
        if (!node) return -1;
        gll_node_t* next = node->next;

        if (list->deallocator && node->data)
        {
            list->deallocator(node->data);
        }

        free(node);
        node = next;
    }
    
    list->size = 0;
    return 0;
}

gll_status_t gll_sort(gll_list_t* list)
{
    if (!list || !list->head || list->size < 2) return 0;

    list->head = _gll_merge_sort(list->head, list->comparator);

    gll_node_t* current = list->head;

    while (current->next != NULL)
    {
        current = current->next;
    }

    list->tail = current;

    return 0;
}

gll_iterator_t* gll_iterator_create(gll_list_t* list)
{
    if (!list) return NULL;
    
    gll_iterator_t* iterator = calloc(1, sizeof(gll_iterator_t));

    if (iterator)
    {
        iterator->list = list;
    }

    return iterator;
}

gll_status_t gll_iterator_destroy(gll_iterator_t* iterator)
{
    if (!iterator) return -1;
    
    free(iterator);
    return 0;
}

gll_data_t gll_iterator_forward(gll_iterator_t* iterator)
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

gll_data_t gll_iterator_backward(gll_iterator_t* iterator)
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

gll_result_t gll_comparator_alphabetical(gll_data_t data1, gll_data_t data2)
{
    const char* str1 = (const char*) data1;
    const char* str2 = (const char*) data2;

    while (*str1 && *str2)
    {
        // Convert characters to lowercase before comparing
        char lower1 = tolower((char) *str1);
        char lower2 = tolower((char) *str2);

        if (lower1 != lower2)
        {
            return (lower1 < lower2) ? -1 : 1;
        }

        str1++;
        str2++;
    }
    
    // If the loop completes, strings are identical up to the shorter length.
    return (!(*str1) && !(*str2)) ? 0 : (*str1 ? 1 : -1);
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

static gll_status_t _gll_insert(gll_list_t* list, gll_index_t index, gll_data_t data, bool backward)
{
    gll_iterator_t* iterator = gll_iterator_create(list);

    if (backward)
    {
        for (gll_index_t i = list->size - 1; i >= index; i--)
        {
            gll_iterator_backward(iterator);
        }
    }
    else
    {
        for (gll_index_t i = 0; i <= index; i++)
        {
            gll_iterator_forward(iterator);
        }
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

static gll_data_t _gll_remove(gll_list_t* list, gll_index_t index, bool backward)
{
    gll_data_t data = 0;
    gll_iterator_t* iterator = gll_iterator_create(list);

    if (backward)
    {
        for (gll_index_t i = list->size - 1; i >= index; i--)
        {
            gll_iterator_backward(iterator);
        }
    }
    else
    {
        for (gll_index_t i = 0; i <= index; i++)
        {
            gll_iterator_forward(iterator);
        }
    }

    iterator->current->prev->next = iterator->current->next;
    iterator->current->next->prev = iterator->current->prev;
    list->size--;

    data = iterator->current->data;

    gll_iterator_destroy(iterator);

    return data;
}

static gll_node_t* _gll_split_list(gll_node_t* head)
{
    gll_node_t* slow = head;
    gll_node_t* fast = head->next;

    while (fast)
    {
        fast = fast->next;

        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    gll_node_t* mid = slow->next;
    slow->next = NULL;

    return mid;
}

static gll_node_t* _gll_sorted_merge(gll_node_t* a, gll_node_t* b, gll_comparator_t comparator)
{
    if (!a) return b;
    if (!b) return a;

    gll_node_t* result = NULL;

    if (comparator(a->data, b->data) <= 0)
    {
        result = a;
        result->next = _gll_sorted_merge(a->next, b, comparator);
        result->next->prev = result;
        result->prev = NULL;
    }
    else
    {
        result = b;
        result->next = _gll_sorted_merge(a, b->next, comparator);
        result->next->prev = result;
        result->prev = NULL;
    }

    return result;
}

static gll_node_t* _gll_merge_sort(gll_node_t* head, gll_comparator_t comparator)
{
    if (!head || !head->next) return head;

    gll_node_t* mid = _gll_split_list(head);
    gll_node_t* left = _gll_merge_sort(head, comparator);
    gll_node_t* right = _gll_merge_sort(mid, comparator);

    return _gll_sorted_merge(left, right, comparator);
}