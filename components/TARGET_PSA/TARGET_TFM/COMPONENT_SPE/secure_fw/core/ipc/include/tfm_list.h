/*
 * Copyright (c) 2018, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __TFM_LIST_H__
#define __TFM_LIST_H__

/* List structure */
struct tfm_list_node_t {
    struct tfm_list_node_t *prev;
    struct tfm_list_node_t *next;
};

/**
 * \brief Initialize list head.
 *
 * \param[in] head              List head need to be initialized.
 */
__STATIC_INLINE void tfm_list_init(struct tfm_list_node_t *head)
{
    head->next = head;
    head->prev = head;
}

/**
 * \brief Add one node to list tail.
 *
 * \param[in] head              List head initialized by \ref tfm_list_init.
 * \param[in] node              List node want to be added.
 */
__STATIC_INLINE void
tfm_list_add_tail(struct tfm_list_node_t *head, struct tfm_list_node_t *node)
{
    head->prev->next = node;
    node->prev = head->prev;
    head->prev = node;
    node->next = head;
}

/**
 * \brief Check if a list is empty.
 *
 * \param[in] head              List head initialized by \ref tfm_list_init.
 *
 * \returns                     returns 1 for empty, or 0 for not.
 */
__STATIC_INLINE int32_t tfm_list_is_empty(struct tfm_list_node_t *head)
{
    return (head->next == head);
}

/**
 * \brief Insert one node to list head.
 *
 * \param[in] head              List head initialized by \ref tfm_list_init.
 * \param[in] node              List node want to be inserted.
 */
__STATIC_INLINE void
tfm_list_insert_first(struct tfm_list_node_t *head,
                      struct tfm_list_node_t *node)
{
    node->next = head->next;
    node->prev = head;
    head->next->prev = node;
    head->next = node;
}

/**
 * \brief Retrieve the fist node from list.
 *
 * \param[in] head              List head initialized by \ref tfm_list_init.
 *
 *  \returns                    Returns the pointer to first list node.
 */
__STATIC_INLINE
struct tfm_list_node_t *tfm_list_first_node(struct tfm_list_node_t *head)
{
    return head->next;
}

/**
 * \brief Delete one node from list.
 *
 * \param[in] node              List node want to be deleted.
 */
__STATIC_INLINE void tfm_list_del_node(struct tfm_list_node_t *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

/* Go through each node of a list */
#define TFM_LIST_FOR_EACH(node, head) \
    for (node = (head)->next; node != head; node = node->next)

#endif
