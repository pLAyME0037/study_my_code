#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node *addHead(Node *head, int value) {
    Node *temp = malloc(sizeof *temp);
    temp->value = value;
    temp->next = head;

    return temp;
}

Node *addTail(Node *head, int value) {
    Node *temp = malloc(sizeof *temp);
    temp->value = value;
    temp->next = NULL;

    if (head == NULL) { return temp; }

    Node *curr = head;
    while (curr->next != NULL) { curr = curr->next; }
    curr->next = temp;

    return head;
}

void printList(Node *head) {
    Node *curr = head;

    printf("{\n");
    while (curr != NULL) {
        printf("    [%p | %d | %p], \n", (void *)curr, curr->value, (void *)curr->next);
        curr = curr->next;
    }
    printf("}\n");
}

int main() {
    Node *head = NULL;
    int val[] = {1, 2, 3, 4, 5};

    for (int i = 0; i < sizeof(val)/sizeof(val[0]); ++i) {
       head = addTail(head, val[i]);
    }
    head = addHead(head, 9);
    head = addHead(head, 8);
    head = addHead(head, 7);

    printList(head);

    return 0;
}
