/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NS_LIST_H_
#define NS_LIST_H_

#include "ns_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \file
 * \brief Linked list support library
 *
 * The ns_list.h file provides a doubly-linked list/queue, providing O(1)
 * performance for all insertion/removal operations, and access to either
 * end of the list.
 *
 * Memory footprint is two pointers for the list head, and two pointers in each
 * list entry. It is similar in concept to BSD's TAILQ.
 *
 * Although the API is symmetrical and O(1) in both directions, due to internal
 * pointer design, it is *slightly* more efficient to insert at the end when
 * used as a queue, and to iterate forwards rather than backwards.
 *
 * Example of an entry type that can be stored to this list.
 * ~~~
 *     typedef struct example_entry
 *     {
 *         uint8_t        *data;
 *         uint32_t       data_count;
 *         ns_list_link_t link;
 *     }
 *     example_entry_t;
 *
 *     static NS_LIST_HEAD(example_entry_t, link) my_list;
 *     ns_list_init(&my_list);
 * ~~~
 * OR
 * ~~~
 *     NS_LIST_HEAD(example_entry_t, link) my_list = NS_LIST_INIT(my_list);
 * ~~~
 * OR
 * ~~~
 *     static NS_LIST_DEFINE(my_list, example_entry_t, link);
 * ~~~
 * OR
 * ~~~
 *     typedef NS_LIST_HEAD(example_entry_t, link) example_list_t;
 *     example_list_t NS_LIST_NAME_INIT(my_list);
 * ~~~
 * NOTE: the link field SHALL NOT be accessed by the user.
 *
 * An entry can exist on multiple lists by having multiple link fields.
 *
 * All the list operations are implemented as macros, most of which are backed
 * by optionally-inline functions. The macros do not evaluate any arguments more
 * than once, unless documented.
 *
 * In macro documentation, `list_t` refers to a list type defined using
 * NS_LIST_HEAD(), and `entry_t` to the entry type that was passed to it.
 */

/** \brief Underlying generic linked list head.
 *
 * Users should not use this type directly, but use the NS_LIST_HEAD() macro.
 */
typedef struct ns_list {
    void *first_entry;      ///< Pointer to first entry, or NULL if list is empty
    void **last_nextptr;    ///< Pointer to last entry's `next` pointer, or
    ///< to head's `first_entry` pointer if list is empty
} ns_list_t;

/** \brief Declare a list head type
 *
 * This union stores the real list head, and also encodes as compile-time type
 * information the offset of the link pointer, and the type of the entry.
 *
 * Note that type information is compiler-dependent; this means
 * ns_list_get_first() could return either `void *`, or a pointer to the actual
 * entry type. So `ns_list_get_first()->data` is not a portable construct -
 * always assign returned entry pointers to a properly typed pointer variable.
 * This assignment will be then type-checked where the compiler supports it, and
 * will dereference correctly on compilers that don't support this extension.
 * ~~~
 *     NS_LIST_HEAD(example_entry_t, link) my_list;
 *
 *     example_entry_t *entry = ns_list_get_first(&my_list);
 *     do_something(entry->data);
 * ~~~
 * Each use of this macro generates a new anonymous union, so these two lists
 * have different types:
 * ~~~
 *     NS_LIST_HEAD(example_entry_t, link) my_list1;
 *     NS_LIST_HEAD(example_entry_t, link) my_list2;
 * ~~~
 * If you need to use a list type in multiple places, eg as a function
 * parameter, use typedef:
 * ~~~
 *     typedef NS_LIST_HEAD(example_entry_t, link) example_list_t;
 *
 *     void example_function(example_list_t *);
 * ~~~
 */
#define NS_LIST_HEAD(entry_type, field) \
    NS_LIST_HEAD_BY_OFFSET_(entry_type, offsetof(entry_type, field))

/** \brief Declare a list head type for an incomplete entry type.
 *
 * This declares a list head, similarly to NS_LIST_HEAD(), but unlike that
 * this can be used in contexts where the entry type may be incomplete.
 *
 * To use this, the link pointer must be the first member in the
 * actual complete structure. This is NOT checked - the definition of the
 * element should probably test NS_STATIC_ASSERT(offsetof(type, link) == 0)
 * if outside users are known to be using NS_LIST_HEAD_INCOMPLETE().
 * ~~~
 *     struct opaque;
 *     NS_LIST_HEAD_INCOMPLETE(struct opaque) opaque_list;
 * ~~~
 */
#define NS_LIST_HEAD_INCOMPLETE(entry_type) \
    NS_LIST_HEAD_BY_OFFSET_(entry_type, 0)

/// \privatesection
/** \brief Internal macro defining a list head, given the offset to the link pointer
 * The +1 allows for link_offset being 0 - we can't declare a 0-size array
 */
#define NS_LIST_HEAD_BY_OFFSET_(entry_type, link_offset) \
union \
{ \
    ns_list_t slist; \
    NS_FUNNY_COMPARE_OK \
    NS_STATIC_ASSERT(link_offset <= UINT_FAST8_MAX, "link offset too large") \
    NS_FUNNY_COMPARE_RESTORE \
    char (*offset)[link_offset + 1]; \
    entry_type *type; \
}

/** \brief Get offset of link field in entry.
 * \return `(ns_list_offset_t)` The offset of the link field for entries on the specified list
 */
#define NS_LIST_OFFSET_(list) ((ns_list_offset_t) (sizeof *(list)->offset - 1))

/** \brief Get the entry pointer type.
 * \def NS_LIST_PTR_TYPE_
 *
 * \return An unqualified pointer type to an entry on the specified list.
 *
 * Only available if the compiler provides a "typeof" operator.
 */
#if defined __cplusplus && __cplusplus >= 201103L
#define NS_LIST_PTR_TYPE_(list) decltype((list)->type)
#elif defined __GNUC__
#define NS_LIST_PTR_TYPE_(list) __typeof__((list)->type)
#endif

/** \brief Check for compatible pointer types
 *
 * This test will produce a diagnostic about a pointer mismatch on
 * the == inside the sizeof operator. For example ARM/Norcroft C gives the error:
 *
 *     operand types are incompatible ("entry_t *" and "other_t *")
 */
#ifdef CPPCHECK
#define NS_PTR_MATCH_(a, b, str) ((void) 0)
#else
#define NS_PTR_MATCH_(a, b, str) ((void) sizeof ((a) == (b)))
#endif

/** \brief Internal macro to cast returned entry pointers to correct type.
 *
 * Not portable in C, alas. With GCC or C++11, the "get entry" macros return
 * correctly-typed pointers. Otherwise, the macros return `void *`.
 *
 * The attempt at a portable version would work if the C `?:` operator wasn't
 * broken - `x ? (t *) : (void *)` should really have type `(t *)` in C, but
 * it has type `(void *)`, which only makes sense for C++. The `?:` is left in,
 * in case some day it works. Some compilers may still warn if this is
 * assigned to a different type.
 */
#ifdef NS_LIST_PTR_TYPE_
#define NS_LIST_TYPECAST_(list, val) ((NS_LIST_PTR_TYPE_(list)) (val))
#else
#define NS_LIST_TYPECAST_(list, val) (0 ? (list)->type : (val))
#endif

/** \brief Internal macro to check types of input entry pointer. */
#define NS_LIST_TYPECHECK_(list, entry) \
    (NS_PTR_MATCH_((list)->type, (entry), "incorrect entry type for list"), (entry))

/** \brief Type used to pass link offset to underlying functions
 *
 * We could use size_t, but it would be unnecessarily large on 8-bit systems,
 * where we can be (pretty) confident we won't have next pointers more than
 * 256 bytes into a structure.
 */
typedef uint_fast8_t ns_list_offset_t;

/// \publicsection
/** \brief The type for the link member in the user's entry structure.
 *
 * Users should not access this member directly - just pass its name to the
 * list head macros. The funny prev pointer simplifies common operations
 * (eg insertion, removal), at the expense of complicating rare reverse iteration.
 *
 * NB - the list implementation relies on next being the first member.
 */
typedef struct ns_list_link {
    void *next;     ///< Pointer to next entry, or NULL if none
    void **prev;    ///< Pointer to previous entry's (or head's) next pointer
} ns_list_link_t;

/** \brief "Poison" value placed in unattached entries' link pointers.
 * \internal What are good values for this? Platform dependent, maybe just NULL
 */
#define NS_LIST_POISON ((void *) 0xDEADBEEF)

/** \brief Initialiser for an entry's link member
 *
 * This initialiser is not required by the library, but a user may want an
 * initialiser to include in their own entry initialiser. See
 * ns_list_link_init() for more discussion.
 */
#define NS_LIST_LINK_INIT(name) \
    NS_FUNNY_INTPTR_OK \
    { NS_LIST_POISON, NS_LIST_POISON } \
    NS_FUNNY_INTPTR_RESTORE

/** \hideinitializer \brief Initialise an entry's list link
 *
 * This "initialises" an unattached entry's link by filling the fields with
 * poison. This is optional, as unattached entries field pointers are not
 * meaningful, and it is not valid to call ns_list_get_next or similar on
 * an unattached entry.
 *
 * \param entry Pointer to an entry
 * \param field The name of the link member to initialise
 */
#define ns_list_link_init(entry, field) ns_list_link_init_(&(entry)->field)

/** \hideinitializer \brief Initialise a list
 *
 * Initialise a list head before use. A list head must be initialised using this
 * function or one of the NS_LIST_INIT()-type macros before use. A zero-initialised
 * list head is *not* valid.
 *
 * If used on a list containing existing entries, those entries will
 * become detached. (They are not modified, but their links are now effectively
 * undefined).
 *
 * \param list Pointer to a NS_LIST_HEAD() structure.
 */
#define ns_list_init(list) ns_list_init_(&(list)->slist)

/** \brief Initialiser for an empty list
 *
 * Usage in an enclosing initialiser:
 * ~~~
 *      static my_type_including_list_t x = {
 *          "Something",
 *          23,
 *          NS_LIST_INIT(x),
 *      };
 * ~~~
 * NS_LIST_DEFINE() or NS_LIST_NAME_INIT() may provide a shorter alternative
 * in simpler cases.
 */
#define NS_LIST_INIT(name) { { NULL, &(name).slist.first_entry } }

/** \brief Name and initialiser for an empty list
 *
 * Usage:
 * ~~~
 *      list_t NS_LIST_NAME_INIT(foo);
 * ~~~
 * acts as
 * ~~~
 *      list_t foo = { empty list };
 * ~~~
 * Also useful with designated initialisers:
 * ~~~
 *      .NS_LIST_NAME_INIT(foo),
 * ~~~
 * acts as
 * ~~~
 *      .foo = { empty list },
 * ~~~
 */
#define NS_LIST_NAME_INIT(name) name = NS_LIST_INIT(name)

/** \brief Define a list, and initialise to empty.
 *
 * Usage:
 * ~~~
 *     static NS_LIST_DEFINE(my_list, entry_t, link);
 * ~~~
 * acts as
 * ~~~
 *     static list_type my_list = { empty list };
 * ~~~
 */
#define NS_LIST_DEFINE(name, type, field) \
    NS_LIST_HEAD(type, field) NS_LIST_NAME_INIT(name)

/** \hideinitializer \brief Add an entry to the start of the linked list.
 *
 * ns_list_add_to_end() is *slightly* more efficient than ns_list_add_to_start().
 *
 * \param list  `(list_t *)`           Pointer to list.
 * \param entry `(entry_t * restrict)` Pointer to new entry to add.
 */
#define ns_list_add_to_start(list, entry) \
    ns_list_add_to_start_(&(list)->slist, NS_LIST_OFFSET_(list), NS_LIST_TYPECHECK_(list, entry))

/** \hideinitializer \brief Add an entry to the end of the linked list.
 *
 * \param list  `(list_t *)`           Pointer to list.
 * \param entry `(entry_t * restrict)` Pointer to new entry to add.
 */
#define ns_list_add_to_end(list, entry) \
    ns_list_add_to_end_(&(list)->slist, NS_LIST_OFFSET_(list), NS_LIST_TYPECHECK_(list, entry))

/** \hideinitializer \brief Add an entry before a specified entry.
 *
 * \param list   `(list_t *)`           Pointer to list.
 * \param before `(entry_t *)`          Existing entry before which to place the new entry.
 * \param entry  `(entry_t * restrict)` Pointer to new entry to add.
 */
#define ns_list_add_before(list, before, entry) \
    ns_list_add_before_(NS_LIST_OFFSET_(list), NS_LIST_TYPECHECK_(list, before), NS_LIST_TYPECHECK_(list, entry))

/** \hideinitializer \brief Add an entry after a specified entry.
 *
 * ns_list_add_before() is *slightly* more efficient than ns_list_add_after().
 *
 * \param list  `(list_t *)`           Pointer to list.
 * \param after `(entry_t *)`          Existing entry after which to place the new entry.
 * \param entry `(entry_t * restrict)` Pointer to new entry to add.
 */
#define ns_list_add_after(list, after, entry) \
    ns_list_add_after_(&(list)->slist, NS_LIST_OFFSET_(list), NS_LIST_TYPECHECK_(list, after), NS_LIST_TYPECHECK_(list, entry))

/** \brief Check if a list is empty.
 *
 * \param list `(const list_t *)` Pointer to list.
 *
 * \return     `(bool)`           true if the list is empty.
 */
#define ns_list_is_empty(list) ((bool) ((list)->slist.first_entry == NULL))

/** \brief Get the first entry.
 *
 * \param list `(const list_t *)` Pointer to list.
 *
 * \return     `(entry_t *)`      Pointer to first entry.
 * \return                        NULL if list is empty.
 */
#define ns_list_get_first(list) NS_LIST_TYPECAST_(list, (list)->slist.first_entry)

/** \hideinitializer \brief Get the previous entry.
 *
 * \param list    `(const list_t *)`  Pointer to list.
 * \param current `(const entry_t *)` Pointer to current entry.
 *
 * \return        `(entry_t *)`       Pointer to previous entry.
 * \return                            NULL if current entry is first.
 */
#define ns_list_get_previous(list, current) \
    NS_LIST_TYPECAST_(list, ns_list_get_previous_(&(list)->slist, NS_LIST_OFFSET_(list), NS_LIST_TYPECHECK_(list, current)))

/** \hideinitializer \brief Get the next entry.
 *
 * \param list    `(const list_t *)`  Pointer to list.
 * \param current `(const entry_t *)` Pointer to current entry.
 *
 * \return        `(entry_t *)`       Pointer to next entry.
 * \return                            NULL if current entry is last.
 */
#define ns_list_get_next(list, current) \
    NS_LIST_TYPECAST_(list, ns_list_get_next_(NS_LIST_OFFSET_(list), NS_LIST_TYPECHECK_(list, current)))

/** \hideinitializer \brief Get the last entry.
 *
 * \param list `(const list_t *)` Pointer to list.
 *
 * \return     `(entry_t *)`      Pointer to last entry.
 * \return                        NULL if list is empty.
 */
#define ns_list_get_last(list) \
    NS_LIST_TYPECAST_(list, ns_list_get_last_(&(list)->slist, NS_LIST_OFFSET_(list)))

/** \hideinitializer \brief Remove an entry.
 *
 * \param list  `(list_t *)`  Pointer to list.
 * \param entry `(entry_t *)` Entry on list to be removed.
 */
#define ns_list_remove(list, entry) \
    ns_list_remove_(&(list)->slist, NS_LIST_OFFSET_(list), NS_LIST_TYPECHECK_(list, entry))

/** \hideinitializer \brief Replace an entry.
 *
 * \param list        `(list_t *)`           Pointer to list.
 * \param current     `(entry_t *)`          Existing entry on list to be replaced.
 * \param replacement `(entry_t * restrict)` New entry to be the replacement.
 */
#define ns_list_replace(list, current, replacement) \
    ns_list_replace_(&(list)->slist, NS_LIST_OFFSET_(list), NS_LIST_TYPECHECK_(list, current), NS_LIST_TYPECHECK_(list, replacement))

/** \hideinitializer \brief Concatenate two lists.
 *
 * Attach the entries on the source list to the end of the destination
 * list, leaving the source list empty.
 *
 * \param dst `(list_t *)` Pointer to destination list.
 * \param src `(list_t *)` Pointer to source list.
 *
 */
#define ns_list_concatenate(dst, src) \
        (NS_PTR_MATCH_(dst, src, "concatenating different list types"), \
        ns_list_concatenate_(&(dst)->slist, &(src)->slist, NS_LIST_OFFSET_(src)))

/** \brief Iterate forwards over a list.
 *
 * Example:
 * ~~~
 *     ns_list_foreach(const my_entry_t, cur, &my_list)
 *     {
 *         printf("%s\n", cur->name);
 *     }
 * ~~~
 * Deletion of the current entry is not permitted as its next is checked after
 * running user code.
 *
 * The iteration pointer is declared inside the loop, using C99/C++, so it
 * is not accessible after the loop.  This encourages good code style, and
 * matches the semantics of C++11's "ranged for", which only provides the
 * declaration form:
 * ~~~
 *     for (const my_entry_t cur : my_list)
 * ~~~
 * If you need to see the value of the iteration pointer after a `break`,
 * you will need to assign it to a variable declared outside the loop before
 * breaking:
 * ~~~
 *      my_entry_t *match = NULL;
 *      ns_list_foreach(my_entry_t, cur, &my_list)
 *      {
 *          if (cur->id == id)
 *          {
 *              match = cur;
 *              break;
 *          }
 *      }
 * ~~~
 *
 * The user has to specify the entry type for the pointer definition, as type
 * extraction from the list argument isn't portable. On the other hand, this
 * also permits const qualifiers, as in the example above, and serves as
 * documentation. The entry type will be checked against the list type where the
 * compiler supports it.
 *
 * \param type                    Entry type `([const] entry_t)`.
 * \param e                       Name for iteration pointer to be defined
 *                                inside the loop.
 * \param list `(const list_t *)` Pointer to list - evaluated multiple times.
 */
#define ns_list_foreach(type, e, list) \
    for (type *e = ns_list_get_first(list); e; e = ns_list_get_next(list, e))

/** \brief Iterate forwards over a list, where user may delete.
 *
 * As ns_list_foreach(), but deletion of current entry is permitted as its
 * next pointer is recorded before running user code.
 *
 * Example:
 * ~~~
 *     ns_list_foreach_safe(my_entry_t, cur, &my_list)
 *     {
 *         ns_list_remove(cur);
 *     }
 * ~~~
 * \param type               Entry type `(entry_t)`.
 * \param e                  Name for iteration pointer to be defined
 *                           inside the loop.
 * \param list `(list_t *)`  Pointer to list - evaluated multiple times.
 */
#define ns_list_foreach_safe(type, e, list) \
    for (type *e = ns_list_get_first(list), *_next##e; \
        e && (_next##e = ns_list_get_next(list, e), true); e = _next##e)

/** \brief Iterate backwards over a list.
 *
 * As ns_list_foreach(), but going backwards - see its documentation.
 * Iterating forwards is *slightly* more efficient.
 */
#define ns_list_foreach_reverse(type, e, list) \
    for (type *e = ns_list_get_last(list); e; e = ns_list_get_previous(list, e))

/** \brief Iterate backwards over a list, where user may delete.
 *
 * As ns_list_foreach_safe(), but going backwards - see its documentation.
 * Iterating forwards is *slightly* more efficient.
 */
#define ns_list_foreach_reverse_safe(type, e, list) \
    for (type *e = ns_list_get_last(list), *_next##e; \
        e && (_next##e = ns_list_get_previous(list, e), true); e = _next##e)

/** \hideinitializer \brief Count entries on a list
 *
 * Unlike other operations, this is O(n). Note: if list might contain over
 * 65535 entries, this function **must not** be used to get the entry count.
 *
 * \param list `(const list_t *)` Pointer to list.

 * \return     `(uint_fast16_t)`  Number of entries that are stored in list.
 */
#define ns_list_count(list) ns_list_count_(&(list)->slist, NS_LIST_OFFSET_(list))

/** \privatesection
 *  Internal functions - designed to be accessed using corresponding macros above
 */
NS_INLINE void ns_list_init_(ns_list_t *list);
NS_INLINE void ns_list_link_init_(ns_list_link_t *link);
NS_INLINE void ns_list_add_to_start_(ns_list_t *list, ns_list_offset_t link_offset, void *restrict entry);
NS_INLINE void ns_list_add_to_end_(ns_list_t *list, ns_list_offset_t link_offset, void *restrict entry);
NS_INLINE void ns_list_add_before_(ns_list_offset_t link_offset, void *before, void *restrict entry);
NS_INLINE void ns_list_add_after_(ns_list_t *list, ns_list_offset_t link_offset, void *after, void *restrict entry);
NS_INLINE void *ns_list_get_next_(ns_list_offset_t link_offset, const void *current);
NS_INLINE void *ns_list_get_previous_(const ns_list_t *list, ns_list_offset_t link_offset, const void *current);
NS_INLINE void *ns_list_get_last_(const ns_list_t *list,  ns_list_offset_t offset);
NS_INLINE void ns_list_remove_(ns_list_t *list, ns_list_offset_t link_offset, void *entry);
NS_INLINE void ns_list_replace_(ns_list_t *list, ns_list_offset_t link_offset, void *current, void *restrict replacement);
NS_INLINE void ns_list_concatenate_(ns_list_t *dst, ns_list_t *src, ns_list_offset_t offset);
NS_INLINE uint_fast16_t ns_list_count_(const ns_list_t *list, ns_list_offset_t link_offset);

/* Provide definitions, either for inlining, or for ns_list.c */
#if defined NS_ALLOW_INLINING || defined NS_LIST_FN
#ifndef NS_LIST_FN
#define NS_LIST_FN NS_INLINE
#endif

/* Pointer to the link member in entry e */
#define NS_LIST_LINK_(e, offset) ((ns_list_link_t *)((char *)(e) + offset))

/* Lvalue of the next link pointer in entry e */
#define NS_LIST_NEXT_(e, offset) (NS_LIST_LINK_(e, offset)->next)

/* Lvalue of the prev link pointer in entry e */
#define NS_LIST_PREV_(e, offset) (NS_LIST_LINK_(e, offset)->prev)

/* Convert a pointer to a link member back to the entry;
 * works for linkptr either being a ns_list_link_t pointer, or its next pointer,
 * as the next pointer is first in the ns_list_link_t */
#define NS_LIST_ENTRY_(linkptr, offset) ((void *)((char *)(linkptr) - offset))

NS_LIST_FN void ns_list_init_(ns_list_t *list)
{
    list->first_entry = NULL;
    list->last_nextptr = &list->first_entry;
}

NS_LIST_FN void ns_list_link_init_(ns_list_link_t *link)
{
    NS_FUNNY_INTPTR_OK
    link->next = NS_LIST_POISON;
    link->prev = NS_LIST_POISON;
    NS_FUNNY_INTPTR_RESTORE
}

NS_LIST_FN void ns_list_add_to_start_(ns_list_t *list, ns_list_offset_t offset, void *restrict entry)
{
    void *next;

    NS_LIST_PREV_(entry, offset) = &list->first_entry;
    NS_LIST_NEXT_(entry, offset) = next = list->first_entry;

    if (next) {
        NS_LIST_PREV_(next, offset) = &NS_LIST_NEXT_(entry, offset);
    } else {
        list->last_nextptr = &NS_LIST_NEXT_(entry, offset);
    }

    list->first_entry = entry;
}

NS_LIST_FN void ns_list_add_after_(ns_list_t *list, ns_list_offset_t offset, void *current, void *restrict entry)
{
    void *next;

    NS_LIST_PREV_(entry, offset) = &NS_LIST_NEXT_(current, offset);
    NS_LIST_NEXT_(entry, offset) = next = NS_LIST_NEXT_(current, offset);

    if (next) {
        NS_LIST_PREV_(next, offset) = &NS_LIST_NEXT_(entry, offset);
    } else {
        list->last_nextptr = &NS_LIST_NEXT_(entry, offset);
    }

    NS_LIST_NEXT_(current, offset) = entry;
}

NS_LIST_FN void ns_list_add_before_(ns_list_offset_t offset, void *current, void *restrict entry)
{
    void **prev_nextptr;

    NS_LIST_NEXT_(entry, offset) = current;
    NS_LIST_PREV_(entry, offset) = prev_nextptr = NS_LIST_PREV_(current, offset);
    *prev_nextptr = entry;
    NS_LIST_PREV_(current, offset) = &NS_LIST_NEXT_(entry, offset);
}

NS_LIST_FN void ns_list_add_to_end_(ns_list_t *list, ns_list_offset_t offset, void *restrict entry)
{
    void **prev_nextptr;

    NS_LIST_NEXT_(entry, offset) = NULL;
    NS_LIST_PREV_(entry, offset) = prev_nextptr = list->last_nextptr;
    *prev_nextptr = entry;
    list->last_nextptr = &NS_LIST_NEXT_(entry, offset);
}

NS_LIST_FN void *ns_list_get_next_(ns_list_offset_t offset, const void *current)
{
    return NS_LIST_NEXT_(current, offset);
}

NS_LIST_FN void *ns_list_get_previous_(const ns_list_t *list, ns_list_offset_t offset, const void *current)
{
    if (current == list->first_entry) {
        return NULL;
    }

    // Tricky. We don't have a direct previous pointer, but a pointer to the
    // pointer that points to us - ie &head->first_entry OR &{prev}->next.
    // This makes life easier on insertion and removal, but this is where we
    // pay the price.

    // We have to check manually for being the first entry above, so we know it's
    // a real link's next pointer. Then next is the first field of
    // ns_list_link_t, so we can use the normal offset value.

    return NS_LIST_ENTRY_(NS_LIST_PREV_(current, offset), offset);
}

NS_LIST_FN void *ns_list_get_last_(const ns_list_t *list, ns_list_offset_t offset)
{
    if (!list->first_entry) {
        return NULL;
    }

    // See comments in ns_list_get_previous_()
    return NS_LIST_ENTRY_(list->last_nextptr, offset);
}

NS_LIST_FN void ns_list_remove_(ns_list_t *list, ns_list_offset_t offset, void *removed)
{
    void *next;
    void **prev_nextptr;

    next = NS_LIST_NEXT_(removed, offset);
    prev_nextptr = NS_LIST_PREV_(removed, offset);
    if (next) {
        NS_LIST_PREV_(next, offset) = prev_nextptr;
    } else {
        list->last_nextptr = prev_nextptr;
    }
    *prev_nextptr = next;

    ns_list_link_init_(NS_LIST_LINK_(removed, offset));
}

NS_LIST_FN void ns_list_replace_(ns_list_t *list, ns_list_offset_t offset, void *current, void *restrict replacement)
{
    void *next;
    void **prev_nextptr;

    NS_LIST_PREV_(replacement, offset) = prev_nextptr = NS_LIST_PREV_(current, offset);
    NS_LIST_NEXT_(replacement, offset) = next = NS_LIST_NEXT_(current, offset);

    if (next) {
        NS_LIST_PREV_(next, offset) = &NS_LIST_NEXT_(replacement, offset);
    } else {
        list->last_nextptr = &NS_LIST_NEXT_(replacement, offset);
    }
    *prev_nextptr = replacement;

    ns_list_link_init_(NS_LIST_LINK_(current, offset));
}

NS_LIST_FN void ns_list_concatenate_(ns_list_t *dst, ns_list_t *src, ns_list_offset_t offset)
{
    ns_list_link_t *src_first;

    src_first = src->first_entry;
    if (!src_first) {
        return;
    }

    *dst->last_nextptr = src_first;
    NS_LIST_PREV_(src_first, offset) = dst->last_nextptr;
    dst->last_nextptr = src->last_nextptr;

    ns_list_init_(src);
}

NS_LIST_FN uint_fast16_t ns_list_count_(const ns_list_t *list, ns_list_offset_t offset)
{
    uint_fast16_t count = 0;

    for (void *p = list->first_entry; p; p = NS_LIST_NEXT_(p, offset)) {
        count++;
    }

    return count;
}
#endif /* defined NS_ALLOW_INLINING || defined NS_LIST_FN */

#ifdef __cplusplus
}
#endif

#endif /* NS_LIST_H_ */

