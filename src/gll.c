/*
 * gll.c - Generic Linked List implementation
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
    
    gll_iterator_t* iterator = malloc(sizeof(gll_iterator_t));
    iterator->list = list;
    iterator->current = list->head;
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
    
    gll_node_t* current = iterator->current;
    iterator->current = current->next ? current->next : iterator->list->head;
    
    return current->data;
}

gll_data_t gll_iterator_prev(gll_iterator_t* iterator)
{
    if (!iterator) return 0;
    
    gll_node_t* current = iterator->current;
    iterator->current = current->prev ? current->prev : iterator->list->tail;
    
    return current->data;
}

gll_result_t gll_iterator_reset(gll_iterator_t* iterator)
{
    if (!iterator) return -1;
    
    iterator->current = iterator->list->head;
    return 0;
}