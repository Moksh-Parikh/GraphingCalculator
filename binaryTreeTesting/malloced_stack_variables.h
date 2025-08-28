#ifndef STACK_HEADER // protects against multiple inclusion due to nested #include statements
// multiple inclusion can throw an error, especially when there are type definitions
#define STACK_HEADER

// define the stack's properties
typedef struct integerStack {
    long top;
    long long* contents;
} integerStack;

void emptyIntStack(integerStack *input_stack);
int pushToIntStack(long long i, integerStack *input_stack);
int popIntStack(integerStack *input_stack);

#endif
