/* Adapted from THE UNIX PROGRAMMING ENVIRONMENT by Brian Kernighan & Rob 
   Pike. My version adds a pointer used to keep track of the value of the 
   previous calculation.                                                  */

typedef struct Symbol {
    char *name;
    short type;
    union {
        double val;
        double (*ptr)();
    } u;
    struct Symbol *next;
} Symbol;

Symbol *install(), *lookup();

Symbol *ans;
