extern int yylineno;

void yyerror(char *s, ...);

/* 一种结点，指向两个字节点 */
struct ast {
   int nodetype;
   struct ast *l;
   struct ast *r;
};

/* 另一种结点，包含数值，叶子结点 */
struct numval {
   int nodetype;
   double number;
};

struct ast *newast(int nodetype, struct ast *l, struct ast *r);

struct ast *newnum(double d);

double eval(struct ast *);

void treefree(struct ast *);
