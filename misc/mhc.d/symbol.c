/* Adapted from THE UNIX PROGRAMMING ENVIRONMENT by Brian Kernighan & Rob
   Pike. This file is identical to their version.                         */

#include <stdlib.h>
#include "mhc.h"
#include "y.tab.h"

char *emalloc(unsigned n);

static Symbol *symlist = 0;

Symbol *lookup(char *s) 
{
    Symbol *sp;

    for (sp = symlist; sp != NULL; sp = sp->next)
        if (strcmp(sp->name, s) == 0)
            return sp;

    return 0;
}

Symbol *install(char *s, int t, double d) 
{
    Symbol *sp;
    
    sp = (Symbol *) emalloc(sizeof(Symbol));
    sp->name = emalloc(strlen(s)+1);

    strcpy(sp->name, s);

    sp->type = t;
    sp->u.val = d;
    sp->next = symlist;

    symlist = sp;
    
    return sp;
}

char *emalloc(unsigned n)
{
    char *p;

    p = malloc(n);
    if (p == NULL)
        execerror("out of memory", 0);
    return p;
}
