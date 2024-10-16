# Generic Linked List (GLL) Library

## Overview

The **Generic Linked List (GLL)** library is a versatile C library that implements a doubly linked list. It provides functionalities for list creation, manipulation, iteration, and deletion. It is designed to handle generic data, allowing users to store different data types in the list nodes using a uniform interface.

## Features
- **Generic Data Handling:** Supports storing various data types (integers, floats, doubles, and custom pointers) in list nodes using a unified `gll_data_t` type.
- **Doubly Linked List Structure:** Each node contains references to both the previous and next nodes, facilitating bidirectional traversal.
- **Iterators:** Provides iterators for easy traversal in both forward and reverse directions.
- **Memory Management:** Automatic memory allocation for list nodes, with functions to create, delete, and clear lists.
- **Custom Data Type Support:** Via macros, the library can handle specific data types like integers and floating-point numbers seamlessly.

## Getting Started

### Prerequisites
To use the GLL library, ensure you have a C compiler installed and the necessary tools to compile and link C programs.

### Installation

1. Clone the GLL library source files into your project directory.
2. Include the **gll.h** header file in your project:
   ```c
   #include "gll.h"
   ```

### Compilation
To compile your program with the GLL library, ensure you link both the **gll.c** and **gll.h** files with your program:

```bash
gcc -o your_program your_program.c gll.c
```

### Basic Usage Example

```c
#include <stdio.h>
#include "gll.h"

int main() {
    // Create a new list
    gll_list_t* my_list = gll_create();

    // Append data to the list
    // It is possible to add data of any type to the same list, but GLL does not keep track of the type
    // Use this feature with precaution
    gll_append(my_list, GLL_DATA(42));
    gll_append(my_list, GLL_DATA(3.14));
    
    // Get the size of the list
    printf("List size: %zu\n", gll_size(my_list));
    
    // Peek at the first and last elements
    printf("First element: %ld\n", (long) gll_peek(my_list));
    printf("Last element: %f\n", GLL_FLOAT(gll_peek_last(my_list)));
    
    // Pop the first element
    printf("Popped element: %ld\n", (long) gll_pop(my_list));
    
    // When done, delete the list
    gll_delete(my_list);
    
    return 0;
}
```

### API Documentation

#### List Management
- `gll_list_t* gll_create(void);`  
  Creates and returns a new doubly linked list.

- `gll_list_t* gll_clone(gll_list_t* list);`  
  Copies and returns a new instance of a doubly linked list.
  
- `gll_result_t gll_delete(gll_list_t* list);`  
  Deletes the list and frees all allocated memory.

- `gll_size_t gll_size(gll_list_t* list);`  
  Returns the number of elements in the list.

#### Data Operations
- `gll_result_t gll_append(gll_list_t* list, gll_data_t data);`  
  Appends an element to the end of the list.

- `gll_result_t gll_push(gll_list_t* list, gll_data_t data);`  
  Pushes an element to the front of the list.

- `gll_data_t gll_pop(gll_list_t* list);`  
  Removes and returns the first element of the list.

- `gll_data_t gll_trim(gll_list_t* list);`  
  Removes and returns the last element of the list.

#### Iterators
- `gll_iterator_t* gll_iterator_create(gll_list_t* list);`  
  Creates an iterator for traversing the list.

- `gll_data_t gll_iterator_next(gll_iterator_t* iterator);`  
  Moves the iterator to the next element and returns its data.

- `gll_data_t gll_iterator_prev(gll_iterator_t* iterator);`  
  Moves the iterator to the previous element and returns its data.

## License

The GLL library is released under the **MIT License**. You are free to use, modify, and distribute it under the terms of the license. See the [MIT License](https://opensource.org/licenses/MIT) for more details.

## Author

This library was developed by **Laurent Mailloux-Bourassa**.
