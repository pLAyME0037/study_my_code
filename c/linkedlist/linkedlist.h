#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct Node Node;

Node *addHead(Node *head, int value);
Node *addTail(Node *head, int value);
Node *rmHead(Node *head);
Node *rmTail(Node *head);
void printList(Node *head);

#endif // LINKEDLIST_H

