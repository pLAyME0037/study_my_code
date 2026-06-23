#include <stdio.h>
#include "linkedlist.h"

int main() {
    Node head = NULL;
    int val[] = {1, 2, 3, 4, 5};

    for (size_t i = 0; i < sizeof(val)/sizeof(val[0]); ++i) {
       head = addTail(head, val[i]);
    }
    head = addHead(head, 9);
    head = addHead(head, 8);
    head = addHead(head, 7);

    printList(head);

    printf("\n----------------\n");
    head = rmHead(head);
    head = rmHead(head);
    head = rmTail(head);

    printList(head);


    return 0;
}
