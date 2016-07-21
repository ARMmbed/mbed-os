/** @file cfstore_list.h
 *
 * mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _CFSTORE_LIST_H_
#define _CFSTORE_LIST_H_

#include <stddef.h>

/*
 * Doubly linked list implementation based on the following design
 * and psuedo-code:
 *  Introduction to Algorithms, TH Cormen, CE Leiserson, Rl Rivest,
 *  ISBN 0-262-03141-8 (1989), Pages 206-207.
 */
typedef struct cfstore_list_node_t
{
    struct cfstore_list_node_t *next;
    struct cfstore_list_node_t *prev;
} cfstore_list_node_t;


#define CFSTORE_ZERO_NODE(_node_cFStOrE)            \
    do{                                             \
        (_node_cFStOrE)->next=NULL;                 \
        (_node_cFStOrE)->prev=NULL;                 \
    }while(0)

#define CFSTORE_INIT_LIST_HEAD(_node_cFStOrE)       \
    do {                                            \
        (_node_cFStOrE)->next = (_node_cFStOrE);    \
        (_node_cFStOrE)->prev = (_node_cFStOrE);    \
    } while (0)

/* brief insert the new_node between 2 other nodes, the one before being node_before, the one after being node_after */
static inline void cfstore_listAdd(cfstore_list_node_t* node_before, cfstore_list_node_t * new_node, cfstore_list_node_t* node_after)
{
    /* init new node before insertion */
    new_node->next = node_after;
    new_node->prev = node_before;
    node_before->next = new_node;
    node_after->prev = new_node;
}

/* brief remove the node D from the list by making the nodes before and after D point to each other */
static inline void cfstore_listDel(cfstore_list_node_t *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    CFSTORE_ZERO_NODE(node);
}
#endif /* _CFSTORE_LIST_H_ */
