/* Adapted from THE UNIX PROGRAMMING ENVIRONMENT by Brian Kernighan & Rob
   Pike. My version adds the CONST type of token, used to prevent constants
   from being assigned a new value, & remembers the value of each
   calculation so it can be recalled.                                       */

%{
#include "mhc.h"
extern double Pow();
%}
%union {
    double val;     /* actual value */
    Symbol *sym;    /* symbol table pointer */
}
%token  <val>   NUMBER
%token  <sym>   VAR CONST BLTIN UNDEF
%type   <val>   expr asgn
%right  '='
%left   '+' '-'
%left   '*' '/'
%left   UNARYMINUS
%right  '^'
%%
list:     /* nothing */     
        | list '\n'
        | list asgn '\n'
        | list expr '\n'        { printf("%.8g\n", $2); }
        | list error '\n'       { yyerrok; }
        ;

asgn:     VAR '=' expr          { $$ = ans->u.val = $1->u.val = $3; 
                                  $1->type = VAR; }
        | CONST '=' expr        { execerror("assigning to constant", 
                                            $1->name);                 }
        ;

expr:     NUMBER                { $$ = ans->u.val = $1; }
        | CONST                 { $$ = ans->u.val = $1->u.val; }
        | VAR                   { if ($1->type == UNDEF)
                                      execerror("undefined variable", 
                                                $1->name); 
                                  $$ = ans->u.val = $1->u.val; }
        | asgn
        | BLTIN '(' expr ')'    { $$ = ans->u.val = 
                                  (*($1->u.ptr))($3);         }
        | expr '+' expr         { $$ = ans->u.val = $1 + $3; }
        | expr '-' expr         { $$ = ans->u.val = $1 - $3; }
        | expr '*' expr         { $$ = ans->u.val = $1 * $3; }
        | expr '/' expr         { if ($3 == 0.0)
                                      execerror("division by zero", 0);
                                  $$ = ans->u.val = $1 / $3; }
        | expr '^' expr         { $$ = ans->u.val = Pow($1, $3); }
        | '(' expr ')'          { $$ = ans->u.val = $2; }
        | '-' expr  %prec UNARYMINUS    { $$ = lookup("ANS")->u.val = -$2; }
        ;
%%
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

void fpecatch();

char *progname;
jmp_buf begin;
int  lineno = 0;

char *strip_path(char *s)
{
    int l = strlen(s);
    char *p;
    
    for (p = s + l - 1; *p != '/'; --p)
        ;
        
    return ++p;   
}

int main(int argc, char *argv[])
{
    progname = strip_path(argv[0]);
    init();
    setjmp(begin);
    signal(SIGFPE, fpecatch);
    yyparse();
}

void execerror(char *s, char *t)
{
    warning(s, t);
    longjmp(begin, 0);
}

void fpecatch()
{
    execerror("floating point exception", NULL);
}

int yylex()
{
    int c;
    
    while ((c = getchar()) == ' ' || c == '\t')
        ;
    if (c == EOF)
        return 0;
    if (c == '.' || isdigit(c)) { /* number */
        ungetc(c, stdin);
        scanf("%lf", &yylval.val);
        return NUMBER;
    }
    if (isalpha(c)) {
        Symbol *s;
        char sbuf[100], *p = sbuf;
        do {
            *p++ = c;
        } while ((c = getchar()) != EOF && isalnum(c));
        ungetc(c, stdin);
        *p = '\0';
        if ((s = lookup(sbuf)) == NULL)
            s = install(sbuf, UNDEF, 0.0);
        yylval.sym = s;
        return s->type == UNDEF ? VAR : s->type;
    }
    if (c == '\n')
        lineno++;
    
    return c;
}

void yyerror(char *s)
{
    warning(s, (char *) 0);
}

void warning(char *s, char *t)
{
    fprintf(stderr, "%s: %s", progname, s);
    if (t)
        fprintf(stderr, " %s", t);
    fprintf(stderr, " near line %d\n", lineno);
}
