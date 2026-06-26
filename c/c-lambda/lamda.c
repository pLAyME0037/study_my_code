#include <assert.h>
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

int main(void) {

    String_Builder str_b = {0};

    // Expr *expr = app(fun("x", app(var("x"), var("x"))), var("y"));
    // Expr *expr = app(
    //         fun("x", app(var("x"), var("x"))),
    //         fun("x", app(var("x"), var("x"))));
    // Expr *expr = bind_vars(
    //     app(app(fun("y",fun("x", var("y"))), var("x")), var("pp"))
    // );
    Expr *expr = bind_vars(app(fun("y", app(fun("x", var("x")), var("y"))), var("pp")));

    trace_expr(expr, &str_b);

    Expr *expr1 = eval1(expr);
    while (expr1 != expr) {
        expr = expr1;
        trace_expr(expr, &str_b);
        expr1 = eval1(expr);
    }

    /* asm("int3"); */

    return 0;
}
