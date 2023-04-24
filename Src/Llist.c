
#include "../Inc/Llist.h"
#include "../Inc/Sidereal.h"

#include <malloc.h>
#include <stdlib.h>

void insert (Target_t *head, Target_t target)
{

    /* Allocate memory for a new node in the list */
    Target_t *new = (Target_t *) malloc(sizeof(Target_t));

    /* Set the structure data */
    new->name = target.name;
    new->ra   = target.ra;
    new->decl = target.decl;

    new->next = NULL;

    /* Using a single-ended linked list... */

    /* Point the current head's next element
       to the address of the new element.
    */
    head->next = new;

    /* Set the head to the new element */
    head = new;

}

void delete (Target_t *tail)
{
    Target_t *temp = NULL;

    if (tail == NULL) {
        return;
    }

    temp = tail;

    tail = tail->next;

    free(temp);
}

void printList (Target_t *tail) {

    // If the pointer is null, we have nothing.
    if (tail == NULL){
        printf("\n[-] List is empty\n");


    } else {

        // While we are not at the end of the list
        printf("\n[+] Targets | \n");
        while( tail != NULL) {

            // Show the current value.
            printf("[+] %s\n", tail->name);

            // Advance the pointer to the next list address.
            tail = tail->next;

        }

        printf("NULL\n");

    }

}