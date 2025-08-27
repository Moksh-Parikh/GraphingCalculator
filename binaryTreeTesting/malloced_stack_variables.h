#ifndef STACK_HEADER // protects against multiple inclusion due to nested #include statements
// multiple inclusion can throw an error, especially when there are type definitions
#define STACK_HEADER

// define the stack's properties
typedef struct stack {
    long top;
    long long* contents;
} stack;

void make_empty(stack *input_stack);
int push(long long i, stack *input_stack);
int pop(stack *input_stack);

#endif