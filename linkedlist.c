/* A general purpose linked list implementation compatible with C89.
 * Use 'l_create()' to create a new list.
 * All lists are of type 'LinkedList', a struct specified in 'LinkedList.h'.
 * Lists should be freed with 'l_destroy(list)' when they are no longer needed.
 *
 * Dependencies: 'linkedlist.h'.
 * 
 * Author: Alec Dorrington. */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "linkedlist.h"

/* Default destructor used on all lists created with 'l_create()'.
 * Directly frees a value without consideration for any contained pointers.
 * Intended for internal use. */
static void destructor(void* value)
{
    if(value != NULL)
    {
        free(value);
    }
}

/* Predicate for testing equality between two values.
 * Intended for internal use. */
static bool equality(void* element, void* reference)
{
    return element == reference;
}

/* Creates and returns a new linked list with no elements.
 * The list will be created with the default destructor. */
LinkedList* l_create()
{
    /* Allocate memory for list struct. */
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    /* Set default values for list of size 0. */
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    /* The default destructor will simply free any removed elements. */
    list->destructor = &destructor;
    return list;
}

/* Adds the given value to the start of the given list. */
void l_addFirst(LinkedList* list, void* value)
{
    /* Create a new node for the new value.  */
    ListNode* node = (ListNode*) malloc(sizeof(ListNode));
    node->value = value;
    /* If this is the only node, it is also the tail. */
    if(list->size == 0)
    {
        list->tail = node;
        node->next = NULL;
    }
    /* Make this node the new head, and point it at the old head. */
    node->next = list->head;
    list->head = node;
    list->size++;
}

/* Adds the given value to the end of the given list. */
void l_addLast(LinkedList* list, void* value)
{
    /* Create a new node for the new value. */
    ListNode* node = (ListNode*) malloc(sizeof(ListNode));
    node->value = value;
    node->next = NULL;
    /* If there are nodes before this, the old tail should point at this. */
    if(list->size != 0)
    {
        list->tail->next = node;
    }
    /* If this is the only node, it is also the head. */
    else
    {
        list->head = node;
    }
    /* This node is the new tail. */
    list->tail = node;
    list->size++;
}

/* Adds the given value to the given list at the given index.
 * Does nothing if the given index is out of bounds. */
void l_add(LinkedList* list, void* value, int index)
{
    int i;
    ListNode *tracker, *new;
    /* Use addFirst to add an element at the start. */
    if(index == 0)
    {
        l_addFirst(list, value);
    }
    /* Use addLast to add an element at the end. */
    else if(index == list->size)
    {
        l_addLast(list, value);
    }
    /* If the index given isn't at the start or end. */
    else if(index > 0 && index < list->size)
    {
        /* Find the node at the index immediately before the given index. */
        tracker = list->head;
        for(i = 0; i < index - 1; i++)
        {
            tracker = tracker->next;
        }
        /* Create a new node for the new value. */
        new = (ListNode*) malloc(sizeof(ListNode));
        new->value = value;
        /* Insert the new node immediately after this one. */
        new->next = tracker->next;
        tracker->next = new;
        list->size++;
    }
}

/* Adds all the values in the source list to the end of the destination list.
 * Retains the order of all elements, and doesn't modify the source list. */
void l_addAll(LinkedList* dest, LinkedList* src)
{
    Iterator* itr = l_iterator(src);
    /* Add each element in src to dest. */
    while(i_hasNext(itr))
    {
        l_addLast(dest, i_next(itr));
    }
    free(itr);
}

/* Replaces the first element of the list with a new element.
 * Does nothing if the list is empty.
 * Returns the old element, which isn't freed. */
void* l_setFirst(LinkedList* list, void* value)
{
    return l_set(list, value, 0);
}

/* Replaces the last element of the list with a new element.
 * Does nothing if the list is empty.
 * Returns the old element, which isn't freed. */
void* l_setLast(LinkedList* list, void* value)
{
    void* oldValue = NULL;
    if(list->size != 0)
    {
        oldValue = list->tail->value;
        list->tail->value = value;
    }
    return oldValue;
}

/* Replaces the element of the list at an index with a new element.
 * Does nothing if the index is out of bounds.
 * Returns the old element, which isn't freed. */
void* l_set(LinkedList* list, void* value, int index)
{
    int i;
    void* oldValue = NULL;
    ListNode* tracker;
    /* Use setLast to set the last value. */
    if(index == list->size - 1)
    {
        oldValue = l_setLast(list, value);
    }
    /* If the index is in range, and isn't the last element. */
    else if(index >= 0 && index < list->size - 1)
    {
        /* Find the element to be replaced. */
        tracker = list->head;
        for(i = 0; i < index; i++)
        {
            tracker = tracker->next;
        }
        oldValue = tracker->value;
        tracker->value = value;
    }
    return oldValue;
}

/* Returns the value of the first element in the given list,
 * or NULL if the list is empty. */
void* l_getFirst(LinkedList* list)
{
    return l_get(list, 0);
}

/* Returns the value of the last element in the given list,
 * or NULL if the list is empty. */
void* l_getLast(LinkedList* list)
{
    void* value = NULL;
    if(list->size != 0)
    {
        value = list->tail->value;
    }
    return value;
}

/* Returns the value of the element at the given index in the given list,
 * or NULL if the given index is out of bounds. */
void* l_get(LinkedList* list, int index)
{
    int i;
    void* value = NULL;
    ListNode* tracker;
    /* Use getLast to get the last node. */
    if(index == list->size - 1)
    {
        value = l_getLast(list);
    }
    else if(index >= 0 && index < list->size - 1)
    {
        /* Find the node corresponsing to the given index. */
        tracker = list->head;
        for(i = 0; i < index; i++)
        {
            tracker = tracker->next;
        }
        value = tracker->value;
    }
    return value;
}

/* Removes the first element from the given list.
 * Does not free the memory of the element itself.
 * Returns the element which was removed, or NULL if the list was empty. */
void* l_removeFirst(LinkedList* list)
{
    void* value;
    ListNode* old;
    if(list->size != 0)
    {
        /* Get the value of the node to be removed. */
        value = list->head->value;
        old = list->head;
        /* Point the head of the list at the next element. */
        list->head = list->head->next;
        free(old);
        /* If the list is now empty, set the tail to NULL. */
        if(list->size == 1)
        {
            list->tail = NULL;
        }
        list->size--;
    }
    return value;
}

/* Removes the last element from the given list.
 * Does not free the memory of the element itself.
 * Returns the element which was removed, or NULL if the list was empty. */
void* l_removeLast(LinkedList* list)
{
    int i;
    void* value;
    ListNode* tracker;
    if(list->size != 0)
    {
        /* Get the value of the node to be removed. */
        value = list->tail->value;
        free(list->tail);
        /* If the list is now empty, set the head and tail to NULL. */
        if(list->size == 1)
        {
            list->head = NULL;
            list->tail = NULL;
        }
        else
        {
            /* Find the second last node. */
            tracker = list->head;
            for(i = 0; i < list->size - 2; i++)
            {
                tracker = tracker->next;
            }
            /* Set the next pointer of the second last node to NULL. */
            tracker->next = NULL;
            /* Point the tail of the list at the new final element. */
            list->tail = tracker;
        }
        list->size--;
    }
    return value;
}

/* Removes the element at the given index from the given list.
 * Does not free the memory of the element itself.
 * Returns the element which was removed,
 * or NULL if the given index is out of bounds. */
void* l_remove(LinkedList* list, int index)
{
    int i;
    void* value;
    ListNode *tracker, *old;
    /* Use removeFirst to remove the first node. */
    if(index == 0)
    {
        value = l_removeFirst(list);
    }
    /* Use removeLast to remove the last node. */
     else if(index == list->size - 1)
    {
        value = l_removeLast(list);
    }
    /* If the node to be removed isn't the first or last node. */
    else if(index > 0 && index < list->size - 1)
    {
        /* Find the node immediately before the one to be removed. */
        tracker = list->head;
        for(i = 0; i < index - 1; i++)
        {
            tracker = tracker->next;
        }
        old = tracker->next;
        value = old->value;
        /* Bypass and free the old node. */
        tracker->next = old->next;
        free(old);
        list->size--;
    }
    return value;
}

/* Removes the first element from the given list.
 * Also frees the memory of the removed element, using the list's destructor.
 * Does nothing if the list is empty. */
void l_deleteFirst(LinkedList* list)
{
    if(list->size != 0)
    {
        /* Call the destructor on the element to be removed. */
        (*list->destructor)(list->head->value);
        /* Remove the element from the list. */
        l_removeFirst(list);
    }
}

/* Removes the last element from the given list.
 * Also frees the memory of the removed element, using the list's destructor.
 * Does nothing if the list is empty. */
void l_deleteLast(LinkedList* list)
{
    if(list->size != 0)
    {
        /* Call the destructor on the element to be removed. */
        (*list->destructor)(list->tail->value);
        /* Remove the element from the list. */
        l_removeLast(list);
    }
}

/* Removes the element at the given index from the given list.
 * Also frees the memory of the removed element, using the list's destructor.
 * Does nothing if the given index is out of bounds. */
void l_delete(LinkedList* list, int index)
{
    if(index >= 0 && index < list->size)
    {
        /* Call the destructor on the element to be removed. */
        (*list->destructor)(l_get(list, index));
        /* Remove the element form the list. */
        l_remove(list, index);
    }
}

/* Removes all occurences of the given element from the list.
 * Does not free the memory of the elements themselves. */
void l_removeValue(LinkedList* list, void* value)
{
    l_removeIf(list, c_predicate(&equality, &value));
}

/* Removes all elements in the list matching a given predicate.
 * Does not free the memory of the elements themselves. */
void l_removeIf(LinkedList* list, Predicate* selector)
{
    Iterator* itr = l_iterator(list);
    while(i_hasNext(itr))
    {
        /* If this node matches the given predicate. */
        if((*selector->func)(i_next(itr), selector->params))
        {
            /* Remove the node. */
            i_remove(itr);
        }
    }
    free(itr);
    free(selector);
}

/* Removes all occurences of the given element from the list.
 * Also frees the memory of each element, using the list's destructor. */
void l_deleteValue(LinkedList* list, void* value)
{
    l_deleteIf(list, c_predicate(&equality, &value));
}

/* Removes all elements in the list matching a given predicate.
 * Also frees the memory of each element, using the list's destructor. */
void l_deleteIf(LinkedList* list, Predicate* selector)
{
    Iterator* itr = l_iterator(list);
    while(i_hasNext(itr))
    {
        /* If this node matches the given predicate. */
        if((*selector->func)(i_next(itr), selector->params))
        {
            /* Delete the node. */
            i_delete(itr);
        }
    }
    free(itr);
    free(selector);
}

/* Removes all elements from the given list.
 * Does not free the memory of the elements themselves. */
void l_removeAll(LinkedList* list)
{
    ListNode *tracker = list->head, *next;
    /* Free all nodes in the list. */
    while(tracker != NULL)
    {
        next = tracker->next;
        free(tracker);
        tracker = next;
    }
    /* Remove the head and tail pointers. */
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/* Deletes the given list and frees its memory.
 * Does not free the memory of the elements themselves. */
void l_clear(LinkedList* list)
{
    l_removeAll(list);
    free(list);
}

/* Deletes all elements from the given list.
 * Also frees the memory of each element, using the list's destructor. */
void l_deleteAll(LinkedList* list)
{
    Iterator* itr = l_iterator(list);
    /* Call the destructor on each element in the list. */
    while(i_hasNext(itr))
    {
        (*list->destructor)(i_next(itr));
    }
    free(itr);
    /* Remove all elements from the list. */
    l_removeAll(list);
}

/* Deletes the given list and frees its memory.
 * Also frees the memory of each element, using the list's destructor. */
void l_destroy(LinkedList* list)
{
    l_deleteAll(list);
    free(list);
}

/* Returns the index of the first occurence of a value.
 * Returns -1 if there is no such occurence. */
int l_firstIndexOf(LinkedList* list, void* value)
{
    return l_findFirst(list, c_predicate(&equality, &value));
}

/* Returns the index of the first value to match a predicate.
 * Returns -1 if there is no such occurence. */
int l_findFirst(LinkedList* list, Predicate* selector)
{
    int index = -1;
    Iterator* itr = l_iterator(list);
    /* Iterate over the the elements until a match is found. */
    while(i_hasNext(itr) && index == -1)
    {
        /* If this value is accepted by the predicate. */
        if((*selector->func)(i_next(itr), selector->params))
        {
            index = itr->index;
        }
    }
    free(itr);
    free(selector);
    return index;
}

/* Returns the index of the last occurence of a value.
 * Returns -1 if there is no such occurence. */
int l_lastIndexOf(LinkedList* list, void* value)
{
    return l_findLast(list, c_predicate(&equality, &value));
}

/* Returns the index of the last value to match a predicate.
 * Returns -1 if there is no such occurence. */
int l_findLast(LinkedList* list, Predicate* selector)
{
    int index = -1;
    Iterator* itr = l_iterator(list);
    /* Iterate over the elements until the end of the list is reached. */
    while(i_hasNext(itr))
    {
        /* If this value is accepted by the predicate. */
        if((*selector->func)(i_next(itr), selector->params))
        {
            index = itr->index;
        }
    }
    free(itr);
    free(selector);
    return index;
}

/* Returns whether the given list contains the given value. */
bool l_contains(LinkedList* list, void* value)
{
    return l_matches(list, c_predicate(&equality, &value));
}

/* Returns whether a list contains a value accepted by a predicate. */
bool l_matches(LinkedList* list, Predicate* selector)
{
    bool contains = false;
    Iterator* itr = l_iterator(list);
    /* Check each value in the list for acceptance. */
    while(i_hasNext(itr) && !contains)
    {
        contains |= (*selector->func)(i_next(itr), selector->params);
    }
    free(itr);
    free(selector);
    return contains;
}

/* Creates a shallow copy of the given list.
 * Order will be maintained, and the original list remains unmodified. */
LinkedList* l_copy(LinkedList* original)
{
    /* Create a new list. */
    LinkedList* copy = l_create();
    Iterator* itr = l_iterator(original);
    /* Copy each element into the new list. */
    while(i_hasNext(itr))
    {
        l_addLast(copy, i_next(itr));
    }
    free(itr);
    /* The copy should have the same destructor as the original. */
    copy->destructor = original->destructor;
    return copy;
}

/* Performs an action for each element in the list.
 * The action takes the form of a function pointer nested in a Consumer. */
void l_forEach(LinkedList* list, Consumer* action)
{
    Iterator* itr = l_iterator(list);
    i_forEachRemaining(itr, action);
}

/* Maps each element in the list according to the given mapping function.
 * Does not create a new list. Modifies this copy. 
 * Does not free the memory of the original values. */
void l_map(LinkedList* list, Function* mapper)
{
    Iterator* itr = l_iterator(list);
    void* value;
    while(i_hasNext(itr))
    {
        /* Use the mapper on each element. */
        value = (*mapper->func)(i_next(itr), mapper->params);
        itr->current->value = value;
    }
    free(itr);
    free(mapper);
}

/* Maps each element in the list according to the given mapping function.
 * Does not create a new list. Modifies this copy. 
 * Frees the memory of the original values, using the list's destructor. */
void l_dmap(LinkedList* list, Function* mapper)
{
    Iterator* itr = l_iterator(list);
    void* value;
    while(i_hasNext(itr))
    {
        /* Use the mapper on each element. */
        value = (*mapper->func)(i_next(itr), mapper->params);
        /* Call the destructor on the old values. */
        (*list->destructor)(itr->current->value);
        itr->current->value = value;
    }
    free(itr);
    free(mapper);
}

/* Returns whether two given lists are equivalent. To be equivalent,
 * both lists must contain the same values in the same order. */
bool l_equal(LinkedList* list1, LinkedList* list2)
{
    /* Make sure the lists have the same size. */
    bool equal = list1->size == list2->size;
    Iterator* itr1 = l_iterator(list1);
    Iterator* itr2 = l_iterator(list2);
    /* Make sure the lists have the same values. */
    while(i_hasNext(itr1) && i_hasNext(itr2) && equal)
    {
        equal &= i_next(itr1) == i_next(itr2);
    }
    return equal;
}

/* Returns a string representation of a list. Uses the provided string
 * function to convert each individual element in the list to a string, with
 * length being the maximum length string returned by the string function. */
char* l_toString(LinkedList* list, char* (*stringFunction)(void*),
        int length)
{
    /* Allocate space for a string long enough to fit every element. */
    char* string = malloc((list->size * (length + 2) + 1) * sizeof(char));
    char* element;
    Iterator* itr = l_iterator(list);
    strcpy(string, "[");
    /* For each element in the array. */
    while(i_hasNext(itr))
    {
        /* Convert the element into a string. */
        element = (*stringFunction)(i_next(itr));
        /* Add the element's string to the main string. */
        strcat(string, element);
        free(element);
        /* Include commas between elements. */
        if(itr->index != list->size - 1)
        {
            strcat(string, ", ");
        }
    }
    strcat(string, "]");
    return string;
}

/* Creates a new linked list from the contents of an array.
 * Order is maintained, and only a shallow copy of elements is performed. */
LinkedList* l_fromArray(void** array, int size)
{
    LinkedList* list = l_create();
    l_addArray(list, array, size);
    return list;
}

/* Creates and returns a new array from the contents of a list.
 * Order is maintained, and only a shallow copy of elements is performed. */
void** l_toArray(LinkedList* list)
{
    int i = 0;
    Iterator* itr = l_iterator(list);
    /* Allocate memory for the array. */
    void** array = malloc(list->size * sizeof(void*));
    /* Copy the contents of the list into the array. */
    while(i_hasNext(itr))
    {
        array[i++] = i_next(itr);
    }
    free(itr);
    return array;
}

/* Add the contents of an array to the end of an existing list.
 * Order is maintained, and only a shallow copy of elements is performed. */
void l_addArray(LinkedList* list, void** array, int size)
{
    int i;
    /* Copy each element from the array to the end of the list. */
    for(i = 0; i < size; i++)
    {
        l_addLast(list, array[i]);
    }
}

/* Creates and returns a new iterator pointing to the start of the list. */
Iterator* l_iterator(LinkedList* list)
{
    /* Free space for a new iterator. */
    Iterator* itr = (Iterator*) malloc(sizeof(Iterator));
    /* Fill itr with default iterator values. */
    itr->next = list->head;
    itr->current = NULL;
    itr->previous = NULL;
    itr->list = list;
    itr->index = -1;
    return itr;
}

/* Returns whether the given iterator has any remaining elements. */
bool i_hasNext(Iterator* itr)
{
    return itr->next != NULL;
}

/* Returns the next value from the iterator, whilst also incrementing it.
 * Returns NULL if the iterator has no remaining values. */
void* i_next(Iterator* itr)
{
    void* value = NULL;
    if(itr->next != NULL)
    {
        /* Increment the iterator's internal pointers. */
        if(itr->current != NULL)
        {
            itr->previous = itr->current;
        }
        itr->current = itr->next;
        itr->next = itr->next->next;
        /* Get the next value. */
        value = itr->current->value;
        itr->index++;
    }
    return value;
}

/* Removes the iterator's most recently returned element from the list.
 * Does not free the memory of the element itself.
 * Returns the value of the removed element,
 * or NULL if the element was already removed. */
void* i_remove(Iterator* itr)
{
    void* value = NULL;
    if(itr->current != NULL)
    {
        /* Get the value of the element to be removed.. */
        value = itr->current->value;
        /* Bypass and free this element. */
        itr->previous->next = itr->next;
        free(itr->current);
        itr->current = NULL;
        itr->list->size--;
    }
    return value;
}

/* Removes the iterator's most recently returned element from the list.
 * Also frees the memory of the element, using the list's destructor. */
void i_delete(Iterator* itr)
{
    (*itr->list->destructor)(i_remove(itr));
}

/* Execute an action for each remaining element in the list.
 * This function will automatically deplete and free the given iterator.
 * The action takes the form of a function pointer nested in a Consumer. */
void i_forEachRemaining(Iterator* itr, Consumer* action)
{
    while(i_hasNext(itr))
    {
        (*action->func)(i_next(itr), action->params);
    }
    free(itr);
    free(action);
}

/* The following functions are intended to help emulate closures. */

/* Returns a consumer with the given body and context.
 * func: a function pointer to serve as the code body for the consumer.
 * params: an array of variables used to emulate context.
 */
Consumer* c_consumer(void (*func)(void*, void*), void* params)
{
    Consumer* consumer = (Consumer*) malloc(sizeof(Consumer));
    consumer->func = func;
    consumer->params = params;
    return consumer;
}

/* Returns a predicate with the given body and context.
 * func: a function pointer to serve as the code body for the predicate.
 * params: an array of variables used to emulate context.
 */
Predicate* c_predicate(bool (*func)(void*, void*), void* params)
{
    Predicate* predicate = (Predicate*) malloc(sizeof(Predicate));
    predicate->func = func;
    predicate->params = params;
    return predicate;
}

/* Returns a function with the given body and context.
 * func: a function pointer to serve as the code body for the function.
 * params: an array of variables used to emulate context.
 */
Function* c_function(void* (*func)(void*, void*), void* params)
{
    Function* function = (Function*) malloc(sizeof(Function));
    function->func = func;
    function->params = params;
    return function;
}
