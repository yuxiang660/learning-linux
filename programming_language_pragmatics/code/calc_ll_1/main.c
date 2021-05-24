/*

  simple calculator implemented via recursive descent LL(1)

  add_op := + | -
  mul_op := * | /
  digits := {+|-} [0..9] {[0..9]}

  expr         := term term_tail
  term_tail    := add_op term term_tail | null
  term         := factor factor_tail
  fatctor_tail := mul_op factor factor_tail | null
  factor       := digits | '(' expr ')'

 */


double expr(void);

char token;
char END_TOKEN = '\n';

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void error(const char *msg) {
    fprintf(stderr, "Error msg: %s, token %d\n", msg, token);
    exit(1);
}

// match expected and move ahead
void match(char expected) {
    if (token == expected) {
        token = getchar();
        return;
    }

    fprintf(stderr, "Expected %c, got %c\n", expected, token);
    exit(1);
}

double factor() {
    double value;

    if (token == '(') {
        match('(');
        value = expr();
        match(')');

    } else if (isdigit(token) || token == '+' || token == '-') {
        // Get the digital
        ungetc(token, stdin);
        scanf("%lf", &value);
        token = getchar();
    } else {
        error("bad factor");
    }

    return value;
}

double factor_tail() {
    if (token == '*') {
        match('*');
        return factor() * factor_tail();
    } else if (token == '/') {
        match('/');
        return 1 / factor() * factor_tail();
    } else if (token == '+' || token == '-' || token == END_TOKEN) {
        // no factor_tail
        return 1;
    }
    error("bad factor_tail");
}

double term() {
    return factor() * factor_tail();
}

double term_tail() {
    if (token == '+') {
        match('+');
        return term() + term_tail();
    }
    else if (token == '-') {
        match('-');
        return - term() + term_tail();
    } else if (token == END_TOKEN) {
        // no term_tail
        return 0;
    }
    error("bad term_tail");
}

double expr() {
    return term() + term_tail();
}

int main(void) {

    token = getchar();
    double result = expr();
    printf("result: %f\n", result);

    return 0;
}
