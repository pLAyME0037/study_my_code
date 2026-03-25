Indentation: Use tabs (8 spaces wide), not spaces.  Avoid deep nesting—refactor if more than 3 levels deep.

Line Length: Keep lines at most 80 characters long.  Break long statements logically, aligning with open parentheses if needed. 

Braces: Follow K&R style:

Opening brace on the same line for if, switch, for, while, do.
Opening brace on the next line for functions. 
Use braces for multiple statements, even if single-line branches are used. 
Naming:

Local variables: Short and descriptive (e.g., i, tmp). 
Global variables/functions: Descriptive and clear (e.g., count_active_users), not abbreviated or mixed-case. 
Avoid Hungarian notation and cute names like ThisVariableIsATemporaryCounter.
Typedefs: Use sparingly—only for opaque types (e.g., pte_t) or fixed-width integers (e.g., u32).  Avoid typedefs for structs and pointers.

Functions: Keep them short and focused (1–2 screenfuls).  Break complex logic into helper functions. Use goto for centralized cleanup. 

Comments: Use /* */ for multi-line comments. Explain what and why, not how.  Avoid over-commenting obvious logic.

Macros: Avoid multi-statement macros; use do { } while(0) for safety. Prefer static inline functions. 

Memory Allocation: Use kmalloc(sizeof(*p), ...) over sizeof(type). Avoid casting void* return values.

Return Values:

Predicates (e.g., is_valid()) return 0/1.
Commands (e.g., add_work()) return error codes (0 on success, -E* on failure).

