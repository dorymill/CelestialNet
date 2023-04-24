/**
 * @file ListTest.c
 * @author Asmod
 * @brief A test of the target linked list implementation.
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include "../Inc/Llist.h"
#include "../Inc/Sidereal.h"

int main (int argc, char *argv[])
{

    Target_t *head;
    Target_t *tail;

    Target_t dummy2;
    Target_t dummy3;
    Target_t dummy4;

    initTarget (&dummy2, "DummyTarget 2", -0.1, 90.2);
    initTarget (&dummy3, "DummyTarget 3", 0.1, 0.2);
    initTarget (&dummy4, "DummyTarget 4", 1.6, -90.1);
    

    head->name = "DummyTarget 1";
    head->ra   = 0.1;
    head->decl = 90.1;

    head->next = NULL;

    tail = head;

    insert (tail, dummy2);
    insert (tail, dummy3);
    insert (tail, dummy4);

    printList (head);
}