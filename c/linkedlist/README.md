1. *dereference:*
'*' = follow pointer. temp = address. *temp = actual Node at that address.
temp->value = dereference + access.
In C, a pointer is just a variable that holds a memory address (a number like 0x7ff7).
Dereferencing is the act of "following" that address to get the actual data stored there.

2. *Node type = value + next?*
Yes. Every Node has exactly: int value + struct Node *next. Two fields. Always.

3. *next type Node?*
No. next type = struct Node * (pointer). Not Node. Pointer. Points to another
Node elsewhere in memory. Can be NULL.

4. *how Node contain Node inside typedef?*
It doesn't. Contains pointer. Pointer size known (8 bytes). Compiler don't need
full Node size for pointer. If it were Node next (not pointer) → infinite size →
compiler error.

5. *sizeof temp before temp exist?*
sizeof evaluated at compile time. No runtime. Compiler sees temp declared as Node
'*' → sizeof *temp = sizeof(Node). No memory access. Just type math. Line not yet
executed.
