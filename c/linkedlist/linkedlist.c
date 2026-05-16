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

Node *rmHead(Node *head) {
    if (head == NULL) return NULL;
    Node *newH = head->next;

    free(head);

    return newH;
}

Node *rmTail(Node *head) {
    if (head == NULL) return NULL;
    if (head->next == NULL) {
        free(head);
        return NULL;
    }

    Node *curr = head;
    while (curr->next->next != NULL) { curr = curr->next; }

    free(curr->next);
    curr->next = NULL;

    return head;
}

void printList(Node *head) {
    Node *curr = head;

    printf("{\n");
    while (curr != NULL) {
        printf("    ");
        printf("[%p | %d | %p], \n", (void *)curr, curr->value, (void *)curr->next);
        curr = curr->next;
    }
    printf("}\n");
}

