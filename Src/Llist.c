
#include "../Inc/Llist.h"
#include "../Inc/Sidereal.h"

#include <malloc.h>
#include <stdlib.h>

void insert (Target_t *tail, Target_t target)
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
    tail->next = new;

    /* Set the head to the new element */
    tail = new;

}

void delete (Target_t *head)
{
    Target_t *temp = NULL;

    if (head == NULL) {
        return;
    }

    temp = head;

    head = head->next;

    free(temp);
}

void printList (Target_t *head) {

    // If the pointer is null, we have nothing.
    if (head == NULL){
        printf("\n[-] List is empty\n");


    } else {

        // While we are not at the end of the list
        printf("\n[+] Targets | \n");
        while( head != NULL) {

            // Show the current value.
            printf("[+] %s\n", head->name);

            // Advance the pointer to the next list address.
            head = head->next;

        }

        printf("NULL\n");

    }

}