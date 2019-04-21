/* A general purpose linked list implementation compatible with C89.
 * Use 'l_create()' to create a new list.
 * All lists are of type 'LinkedList', a struct specified in 'LinkedList.h'.
 * Lists should be freed with 'l_destroy(list)' when they are no longer needed.
 *
 * Dependencies: 'linkedlist.h'.
 * 
 * Author: Alec Dorrington. */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

/* Represents a single element in a linked list. */
typedef struct ListNode
{
    void* value; /* The value attached to this node. */
    struct ListNode* next; /* The next node in the list. */
} ListNode;

/* Represents a double-ended singly-linked list. */
typedef struct LinkedList
{
    ListNode *head, *tail; /* The first and last nodes in the list. */
    void (*destructor)(void*); /* A function used to free the value's memory. */
    int size; /* The current length of the list. */
} LinkedList;

/* Used to iterate over a linked list. */
typedef struct Iterator
{
    ListNode *previous, *current, *next; /* Iteration trackers. */
    LinkedList* list; /* The list this iterator iterates over. */
    int index; /* The index of the most recently returned value. */
} Iterator;

/* A closure which takes a void* and returns nothing. */
typedef struct Consumer
{
    void (*func)(void*, void*); /* Consumer function pointer. */
    void* params; /* Value array to emulate context. */
} Consumer;

/* A closure which takes a void* and returns a boolean. */
typedef struct Predicate
{
    bool (*func)(void*, void*); /* Predicate function pointer. */
    void* params; /* Value array to emulate context. */
} Predicate;

/* A closure which takes a void* and returns another void*. */
typedef struct Function
{
    void* (*func)(void*, void*); /* Function function pointer. */
    void* params; /* Value array to emulate context. */
} Function;

/* Creates and returns a new linked list with no elements.
 * The list will be created with the default destructor. */
LinkedList* l_create();

/* Adds the given value to the start of the given list. */
void l_addFirst(LinkedList* list, void* value);

/* Adds the given value to the end of the given list. */
void l_addLast(LinkedList* list, void* value);

/* Adds the given value to the given list at the given index.
 * Does nothing if the given index is out of bounds. */
void l_add(LinkedList* list, void* value, int index);

/* Adds all the values in the source list to the end of the destination list.
 * Retains the order of all elements, and doesn't modify the source list. */
void l_addAll(LinkedList* dest, LinkedList* src);

/* Replaces the first element of the list with a new element.
 * Does nothing if the list is empty.
 * Returns the old element, which isn't freed. */
void* l_setFirst(LinkedList* list, void* value);

/* Replaces the last element of the list with a new element.
 * Does nothing if the list is empty.
 * Returns the old element, which isn't freed. */
void* l_setLast(LinkedList* list, void* value);

/* Replaces the element of the list at an index with a new element.
 * Does nothing if the index is out of bounds.
 * Returns the old element, which isn't freed. */
void* l_set(LinkedList* list, void* value, int index);

/* Returns the value of the first element in the given list,
 * or NULL if the list is empty. */
void* l_getFirst(LinkedList* list);

/* Returns the value of the last element in the given list,
 * or NULL if the list is empty. */
void* l_getLast(LinkedList* list);

/* Returns the value of the element at the given index in the given list,
 * or NULL if the given index is out of bounds. */
void* l_get(LinkedList* list, int index);

/* Removes the first element from the given list.
 * Does not free the memory of the element itself.
 * Returns the element which was removed, or NULL if the list was empty. */
void* l_removeFirst(LinkedList* list);

/* Removes the last element from the given list.
 * Does not free the memory of the element itself.
 * Returns the element which was removed, or NULL if the list was empty. */
void* l_removeLast(LinkedList* list);

/* Removes the element at the given index from the given list.
 * Does not free the memory of the element itself.
 * Returns the element which was removed,
 * or NULL if the given index is out of bounds. */
void* l_remove(LinkedList* list, int index);

/* Removes the first element from the given list.
 * Also frees the memory of the removed element, using the list's destructor.
 * Does nothing if the list is empty. */
void l_deleteFirst(LinkedList* list);

/* Removes the last element from the given list.
 * Also frees the memory of the removed element, using the list's destructor.
 * Does nothing if the list is empty. */
void l_deleteLast(LinkedList* list);

/* Removes the element at the given index from the given list.
 * Also frees the memory of the removed element, using the list's destructor.
 * Does nothing if the given index is out of bounds. */
void l_delete(LinkedList* list, int index);

/* Removes all occurences of the given element from the list.
 * Does not free the memory of the elements themselves. */
void l_removeValue(LinkedList* list, void* value);

/* Removes all elements in the list matching a given predicate.
 * Does not free the memory of the elements themselves. */
void l_removeIf(LinkedList* list, Predicate* selector);

/* Removes all occurences of the given element from the list.
 * Also frees the memory of each element, using the list's destructor. */
void l_deleteValue(LinkedList* list, void* value);

/* Removes all elements in the list matching a given predicate.
 * Also frees the memory of each element, using the list's destructor. */
void l_deleteIf(LinkedList* list, Predicate* selector);

/* Removes all elements from the given list.
 * Does not free the memory of the elements themselves. */
void l_removeAll(LinkedList* list);

/* Deletes the given list and frees its memory.
 * Does not free the memory of the elements themselves. */
void l_clear(LinkedList* list);

/* Deletes all elements from the given list.
 * Also frees the memory of each element, using the list's destructor. */
void l_deleteAll(LinkedList* list);

/* Deletes the given list and frees its memory.
 * Also frees the memory of each element, using the list's destructor. */
void l_destroy(LinkedList* list);

/* Returns the index of the first occurence of a value.
 * Returns -1 if there is no such occurence. */
int l_firstIndexOf(LinkedList* list, void* value);

/* Returns the index of the first value to match a predicate.
 * Returns -1 if there is no such occurence. */
int l_findFirst(LinkedList* list, Predicate* selector);

/* Returns the index of the last occurence of a value.
 * Returns -1 if there is no such occurence. */
int l_lastIndexOf(LinkedList* list, void* value);

/* Returns the index of the last value to match a predicate.
 * Returns -1 if there is no such occurence. */
int l_findLast(LinkedList* list, Predicate* selector);

/* Returns whether the given list contains the given value. */
bool l_contains(LinkedList* list, void* value);

/* Returns whether a list contains a value accepted by a predicate. */
bool l_matches(LinkedList* list, Predicate* selector);

/* Creates a shallow copy of the given list.
 * Order will be maintained, and the original list remains unmodified. */
LinkedList* l_copy(LinkedList* original);

/* Performs an action for each element in the list.
 * The action takes the form of a function pointer nested in a Consumer. */
void l_forEach(LinkedList* list, Consumer* action);

/* Maps each element in the list according to the given mapping function.
 * Does not create a new list. Modifies this copy. 
 * Does not free the memory of the original values. */
void l_map(LinkedList* list, Function* mapper);

/* Maps each element in the list according to the given mapping function.
 * Does not create a new list. Modifies this copy. 
 * Frees the memory of the original values, using the list's destructor. */
void l_dmap(LinkedList* list, Function* mapper);

/* Returns whether two given lists are equivalent. To be equivalent,
 * both lists must contain the same values in the same order. */
bool l_equal(LinkedList* list1, LinkedList* list2);

/* Returns a string representation of a list. Uses the provided string
 * function to convert each individual element in the list to a string, with
 * length being the maximum length string returned by the string function. */
char* l_toString(LinkedList* list, char* (*stringFunction)(void*),
        int length);

/* Creates a new linked list from the contents of an array.
 * Order is maintained, and only a shallow copy of elements is performed. */
LinkedList* l_fromArray(void** array, int size);

/* Creates and returns a new array from the contents of a list.
 * Order is maintained, and only a shallow copy of elements is performed. */
void** l_toArray(LinkedList* list);

/* Add the contents of an array to the end of an existing list.
 * Order is maintained, and only a shallow copy of elements is performed. */
void l_addArray(LinkedList* list, void** array, int size);

/* Creates and returns a new iterator pointing to the start of the list. */
Iterator* l_iterator(LinkedList* list);

/* Returns whether the given iterator has any remaining elements. */
bool i_hasNext(Iterator* itr);

/* Returns the next value from the iterator, whilst also incrementing it.
 * Returns NULL if the iterator has no remaining values. */
void* i_next(Iterator* itr);

/* Removes the iterator's most recently returned element from the list.
 * Does not free the memory of the element itself.
 * Returns the value of the removed element,
 * or NULL if the element was already removed. */
void* i_remove(Iterator* itr);

/* Removes the iterator's most recently returned element from the list.
 * Also frees the memory of the element, using the list's destructor. */
void i_delete(Iterator* itr);

/* Execute an action for each remaining element in the list.
 * This function will automatically deplete and free the given iterator.
 * The action takes the form of a function pointer nested in a Consumer. */
void i_forEachRemaining(Iterator* itr, Consumer* action);

/* Returns a consumer with the given body and context.
 * func: a function pointer to serve as the code body for the consumer.
 * params: an array of variables used to emulate context.
 */
Consumer* c_consumer(void (*func)(void*, void*), void* params);

/* Returns a predicate with the given body and context.
 * func: a function pointer to serve as the code body for the predicate.
 * params: an array of variables used to emulate context.
 */
Predicate* c_predicate(bool (*func)(void*, void*), void* params);

/* Returns a function with the given body and context.
 * func: a function pointer to serve as the code body for the function.
 * params: an array of variables used to emulate context.
 */
Function* c_function(void* (*func)(void*, void*), void* params);

#endif
