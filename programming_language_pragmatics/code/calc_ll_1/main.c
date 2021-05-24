/*

  simple calculator implemented via recursive descent LL(1)

  add_op := + | -
  mul_op := * | /
  digits := {+|-} [0..9] {[0..9]}

  expr         := term term_tail
  term_tail    := {add_op term term_tail} | null
  term         := factor factor_tail
  fatctor_tail := {mul_op factor factor_tail} | null
  factor       := digits | '(' expr ')'

 */


int expr(void);

char token;
char END_TOKEN = '\n';
int ERROR = -1;

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void error(const char *msg) {
    fprintf(stderr, "Error msg: %s, token %d", msg, token);
    exit(1);
}

// match expected and move ahead
void match(char expected) {
    if (token == expected) {
        token = getchar();
        return;
    }

    fprintf(stderr, "Expected %c, got %c", expected, token);
    exit(1);
}

int factor() {
    int value;

    if (token == '(') {
        match('(');
        value = expr();
        match(')');

    } else if (isdigit(token) || token == '+' || token == '-') {
        // Get the digital
        ungetc(token, stdin);
        scanf("%d", &value);
        token = getchar();
    } else {
        error("bad factor");
    }

    return value;
}

int factor_tail() {
    if (token == '*') {
        match('*');
        int value = factor();
        int value_mult = factor_tail();
        return value * value_mult;
    } else if (token == '/') {
        match('/');
        int value = factor();
        int value_div = factor_tail();
        return value / value_div;
    } else if (token == '+' || token == '-' || token == END_TOKEN) {
        // no factor_tail
        return 1;
    }
    error("bad factor_tail");
    return ERROR;
}

int term() {
    if (isdigit(token) || token == '(') {
        int value = factor();
        int value_mult = factor_tail();
        return value * value_mult;
    }
    error("bad term");
    return ERROR;
}

int term_tail() {
    if (token == '+') {
        match('+');
        int value = term();
        int value_add = term_tail();
        return value + value_add;
    }
    else if (token == '-') {
        match('-');
        int value = term();
        int value_add = term_tail();
        return -(value + value_add);
    } else if (token == END_TOKEN) {
        // no term_tail
        return 0;
    }
    error("bad term_tail");
    return ERROR;
}

int expr() {
    if (isdigit(token) || token == '(') {
        int value = term();
        int value_add = term_tail();
        return value + value_add;
    }
    error("bad expr");
    return ERROR;
}

int main(void) {

    token = getchar();
    int result = expr();
    printf("result: %d\n", result);

    return 0;
}
