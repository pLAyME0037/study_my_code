#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

typedef enum {
    EXPR_VAR,
    EXPR_FUN,
    EXPR_APP,
} Expr_Kind;

typedef struct Expr Expr;

typedef struct {
    const char *name;
    size_t id;
} Var_Name;

Var_Name var_name_bound(const char *name, size_t id) {
    Var_Name var = {
        .name = name,
        .id = id
    };

    return var;
}

Var_Name var_name_free(const char *name) {
    var_name_bound(name, 0);
}

typedef struct {
    Var_Name arg;
    Expr *body;
} Expr_Fun;

struct Expr {
    Expr_Kind kind;
    union {
        Var_Name var;
        Expr_Fun fun;
        struct {
            Expr *lhs;
            Expr *rhs;
        } app;
    } as;
};

Expr *var(const char *name) {
    Expr *expr = malloc(sizeof(*expr));
    assert(expr != NULL);
    expr->kind = EXPR_VAR;
    expr->as.var = var_name_free(name);
    return expr;
}

Expr *fun(const char *arg, Expr *body) {
    Expr *expr = malloc(sizeof(*expr));
    assert(expr != NULL);
    expr->kind = EXPR_FUN;
    expr->as.fun.arg = var_name_free(arg);
    expr->as.fun.body = body;
    return expr;
}

Expr *fun_bound(Var_Name arg, Expr *body) {
    Expr *expr = malloc(sizeof(*expr));
    assert(expr != NULL);
    expr->kind = EXPR_FUN;
    expr->as.fun.arg = arg;
    expr->as.fun.body = body;
    return expr;
}

Expr *app(Expr *lhs, Expr *rhs) {
    Expr *expr = malloc(sizeof(*expr));
    assert(expr != NULL);
    expr->kind = EXPR_APP;
    expr->as.app.lhs = lhs;
    expr->as.app.rhs = rhs;
    return expr;
}

void expr_display(Expr *expr, String_Builder *builder) {
    switch (expr->kind) {
    case EXPR_VAR:
        sb_appendf(builder, "%s", expr->as.var.name);
        if (expr->as.var.id > 0) {
            sb_appendf(builder, "%zu", expr->as.var.id);
        }
    break;
    case EXPR_FUN:
        sb_appendf(builder, "(λ%s%zu.", expr->as.fun.arg.name, expr->as.fun.arg.id);
        expr_display(expr->as.fun.body, builder);
        sb_appendf(builder, ")");
    break;
    case EXPR_APP:
        sb_appendf(builder, "(");
        expr_display(expr->as.app.lhs, builder);
        sb_appendf(builder, " ");
        expr_display(expr->as.app.rhs, builder);
        sb_appendf(builder, ")");
    break;
    default: UNREACHABLE("Expr_Kind");
    }
}

Expr *replace(Var_Name arg, Expr *body, Expr *val) {
    switch (body->kind) {
    case EXPR_VAR:
        if (strcmp(body->as.var.name, arg.name) == 0 && body->as.var.id == arg.id) {
            return val;
        }
        return body;
    case EXPR_FUN:
        return fun_bound(body->as.fun.arg, replace(arg, body->as.fun.body, val));
    case EXPR_APP:
        return app(
            replace(arg, body->as.app.lhs, val),
            replace(arg, body->as.app.rhs, val));
    default: UNREACHABLE("Expr_Kind");
    }
}

Expr *apply(Expr_Fun fun, Expr *val) {
    return replace(fun.arg, fun.body, val);
}

Expr *eval1(Expr *expr) {
    switch (expr->kind) {
    case EXPR_VAR: return expr;
    case EXPR_FUN: 
        Expr *body = eval1(expr->as.fun.body);
        if (body != expr->as.fun.body) {
            return fun_bound(expr->as.fun.arg, body);
        }
        return expr;
    case EXPR_APP:
        Expr *lhs = eval1(expr->as.app.lhs);

        if (lhs != expr->as.app.lhs) {
            return app(lhs, expr->as.app.rhs);
        }

        if (lhs->kind == EXPR_FUN) {
            return apply(lhs->as.fun, expr->as.app.rhs);
        }

        Expr *rhs = eval1(expr->as.app.rhs);
        if (rhs != expr->as.app.rhs) {
            return app(lhs, rhs);
        } 

        return expr;
    default: UNREACHABLE("Expr_Kind");
    }

}

void trace_expr(Expr *expr, String_Builder *str_b) {
    str_b->count = 0;
    expr_display(expr, str_b);
    sb_append_null(str_b);
    printf("%s\n", str_b->items);
}


void bind_var(Expr *body, Var_Name var) {
    switch (body->kind) {
    case EXPR_VAR: {
        if (strcmp(body->as.var.name, var.name) == 0) {
            body->as.var.id = var.id;
        }
    } break;
    case EXPR_FUN: {
        bind_var(body->as.fun.body, var);
    } break;
    case EXPR_APP: {
        bind_var(body->as.app.lhs, var);
        bind_var(body->as.app.rhs, var);
    } break;
    default: UNREACHABLE("Expr_Kind");
    }
}

Expr *bind_vars(Expr *expr) {
    switch (expr->kind) {
    case EXPR_VAR: return expr;
    case EXPR_FUN: {
        static size_t id_counter = 1;

        assert(expr->as.fun.arg.id == 0);
        expr->as.fun.arg.id = id_counter++;
        bind_var(expr->as.fun.body, expr->as.fun.arg);
        bind_vars(expr->as.fun.body);
    } break;
    case EXPR_APP: {
        bind_vars(expr->as.app.lhs);
        bind_vars(expr->as.app.rhs);
        return expr;
    }
    default: UNREACHABLE("Expr_Kind");
    }
}

typedef enum {
    TOKEN_INVALID,
    TOKEN_END,
    TOKEN_OPAREN,
    TOKEN_CPAREN,
    TOKEN_LAMBDA,
    TOKEN_DOT,
    TOKEN_NAME,
} Token_Kind;

const char *token_kind_display(Token_Kind kind) {
    switch (kind) {
    case TOKEN_INVALID:printf("TOKEN_INVALID"); break;
    case TOKEN_END:    printf("TOKEN_END");     break;
    case TOKEN_OPAREN: printf("TOKEN_OPAREN");  break;
    case TOKEN_CPAREN: printf("TOKEN_CPAREN");  break;
    case TOKEN_LAMBDA: printf("TOKEN_LAMBDA");  break;
    case TOKEN_DOT:    printf("TOKEN_DOT");     break;
    case TOKEN_NAME:   printf("TOKEN_NAME");    break;
    default: UNREACHABLE("Token_Kind");
    }
}

typedef struct {
    size_t pos, bol, col;
} Cursor;

typedef struct {
    const char *content;
    size_t count;

    Cursor cur;
    
    Token_Kind token;
    String_Builder name;
} Lexer;

char lexer_curr_char(Lexer *l) {
    if (l->cur.pos >= l->count) return 0;
    return l->content[l->cur.pos];
}

char lexer_next_char(Lexer *l) {
    if (l->cur.pos >= l->count) return 0;
    char x = l->content[l->cur.pos++];
    if (x == '\n') {
        l->cur.col += 1;
        l->cur.bol = l->cur.pos;
    }
    return x;
}

bool lexer_next(Lexer *l) {
    while (isspace(lexer_curr_char(l))) {
        lexer_next_char(l);
    }
    
    char x = lexer_next_char(l);
    if (x == '\0') {
        l->token = TOKEN_END;
        return false;
    }

    switch (x) {
    case '(':  l->token = TOKEN_OPAREN; return true;
    case ')':  l->token = TOKEN_CPAREN; return true;
    case '\\': l->token = TOKEN_LAMBDA; return true;
    case '.':  l->token = TOKEN_DOT;    return true;
    }

    if (isalnum(x)) {
        l->token = TOKEN_NAME;
        l->name.count = 0;
        da_append(&l->name, x);
        while (isalnum(lexer_curr_char(l))) {
            x = lexer_next_char(l);
            da_append(&l->name, x);
        }
        sb_append_null(&l->name);
        return true;
    }

    l->token = TOKEN_INVALID;
    fprintf(stderr, "ERROR: Unknown token starts with `%c`", x);
    return false;
}
bool lexer_expect(Lexer *l, Token_Kind expected) {
    if (!lexer_next(l)) return false;
    if (l->token != expected) {
         fprintf(stderr, "Unknown Token Type: %s", token_kind_display(l->token));
         return false;
    }
    return true;
}

Expr *parse_expr(Lexer *l);

Expr *parse_app(Lexer *l) {
    Expr *lhs = parse_expr(l);
    Expr *rhs = parse_expr(l);
    if (!lexer_expect(l, TOKEN_CPAREN)) return NULL;
    return app(lhs, rhs);
}

Expr *parse_fun(Lexer *l) {
    if (!lexer_expect(l, TOKEN_NAME)) return NULL;
    const char *arg = strdup(l->name.items);
    if (!lexer_expect(l, TOKEN_DOT)) return NULL;
    Expr *body = parse_expr(l);
    if (body == NULL) return NULL;
    if (!lexer_expect(l, TOKEN_CPAREN)) return NULL;
    return fun(arg, body);
}

Expr *parse_expr(Lexer *l) {
    if (!lexer_next(l)) return NULL;
    switch (l->token) {
    case TOKEN_OPAREN: {
        Cursor saved = l->cur;
        if (!lexer_next(l)) return NULL;
        if (l->token == TOKEN_LAMBDA) {
            return parse_fun(l);
        } else {
            l->cur = saved;
            return parse_app(l);
        }
    } break;
    case TOKEN_NAME: { return var(strdup(l->name.items)); } break;
    default: 
         fprintf(stderr, "Unknown Token Type: %s", token_kind_display(l->token));
         return NULL;
    }
}

char buffer[1000];

int main(void) {

    String_Builder str_b = {0};

    for (;;) {
        printf("λ> ");
        fflush(stdout);
        if (!fgets(buffer, sizeof(buffer), stdin)) break;

        const char *source = buffer;

        Lexer l = {
            .content = source,
            .count = strlen(source)
        };

        while (lexer_next(&l)) {
            switch (l.token) {
                case TOKEN_INVALID:printf("TOKEN_INVALID\n"); break;
                case TOKEN_END:    printf("TOKEN_END\n");     break;
                case TOKEN_OPAREN: printf("TOKEN_OPAREN\n");  break;
                case TOKEN_CPAREN: printf("TOKEN_CPAREN\n");  break;
                case TOKEN_LAMBDA: printf("TOKEN_LAMBDA\n");  break;
                case TOKEN_DOT:    printf("TOKEN_DOT\n");     break;
                case TOKEN_NAME:   printf("TOKEN_NAME: %s\n", l.name.items);    break;
                default: UNREACHABLE("Token_Kind");
            }
        }

        Expr *expr = parse_expr(&l);
        if (!expr) continue;
        bind_vars(expr);

        trace_expr(expr, &str_b);

        Expr *expr1 = eval1(expr);
        while (expr1 != expr) {
            expr = expr1;
            trace_expr(expr, &str_b);
            expr1 = eval1(expr);
        }
    }



    // Expr *expr = app(fun("x", app(var("x"), var("x"))), var("y"));
    // Expr *expr = app(
    //         fun("x", app(var("x"), var("x"))),
    //         fun("x", app(var("x"), var("x"))));
    // Expr *expr = bind_vars(
    //     app(app(fun("y",fun("x", var("y"))), var("x")), var("pp"))
    // );
    // Expr *expr = bind_vars(app(fun("y", app(fun("x", var("x")), var("y"))), var("pp")));
    //

    /* asm("int3"); */

    return 0;
}
