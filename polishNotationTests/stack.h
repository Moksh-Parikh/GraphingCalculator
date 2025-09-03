#ifndef STACK_HEADER // protects against multiple inclusion due to nested #include statements
// multiple inclusion can throw an error, especially when there are type definitions
#define STACK_HEADER

#define INIT_INTEGER_STACK { .type = INTEGER, .top = -1, .integerContents = NULL, }

#define INIT_DOUBLE_STACK { .type = DOUBLE, .top = -1, .floatContents = NULL, }

#define STACK_IS_EMPTY(x) ((x).top < 0) + ((x).top >= 0)

#define STACK_ISNT_EMPTY(x) !STACK_IS_EMPTY(x)

// define the stack's properties
typedef struct integerStack {
    long top;
    long long* contents;
} integerStack;

typedef struct {
    int32_t top;
    char** contents;
} stringStack;

typedef enum {
    INTEGER,
    STRING,
    DOUBLE
} stackType;

typedef struct {
    stackType type;
    int32_t top;
    union {
        char** stringContents;
        int32_t* integerContents;
        double* floatContents;
    };
} stack;

// void emptyIntStack(integerStack *input_stack);
// int pushToIntStack(long long i, integerStack *input_stack);
// int popIntStack(integerStack *input_stack);
// int pushToStringStack(char* string, stringStack *input_stack);
// int popStringStack(char** outputString, stringStack *inputStack);
int pushToStack(void* value, stack* inputStack);
int popStack(void* output, stack* inputStack);
void emptyStack(stack *inputStack);

#endif
