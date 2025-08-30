#include <stdio.h>
#include <stdlib.h>

#include "malloced_stack_variables.h"

void emptyIntStack(integerStack *input_stack) {
    input_stack->top = -1;
    if (input_stack->contents == NULL) return;
    free(input_stack->contents);
}

int pushToIntStack(long long i, integerStack *input_stack) {
    if (input_stack->top < -1) {  // ensure top is valid
        fprintf(stderr, "Error in push(): Top is less than -1 somehow?\nTop is %d\n", input_stack->top);
        return -1;
    }
    else {
        if (input_stack->top >= 0) {
            input_stack->contents = (long long*)
                        realloc(
                            input_stack->contents,
                            ( ( input_stack->top + 1 ) + 1 ) *
                            sizeof(long long)
                        );
            
            if (input_stack->contents == NULL) {
                fprintf(stderr, "Error in push(): malloc() failed to return a value\n");
            
                return -1;
            }

            input_stack->top += 1;
            input_stack->contents[input_stack->top] = i;
        }
        else if (input_stack->top == -1) {
            input_stack->contents = (long long*) malloc(sizeof(long long));

            if (input_stack->contents == NULL) {
                fprintf(stderr, "Error in push(): malloc() failed to return a value\n");

                return -1;
            }

            input_stack->contents[0] = i;
            // printf("Value: %d\n", input_stack->contents[0]);
            input_stack->top = 0;
        }
        else {
            fprintf(stderr, "Error in push(): IDFK bro\n");
        }

        return input_stack->top;
    }
}

int popIntStack(integerStack *input_stack) {
    if (input_stack->top >= 0) {
        int returnVal = input_stack->contents[input_stack->top];
        input_stack->top = input_stack->top - 1;

        if (input_stack->top < 0) {
            input_stack->contents = NULL;
            free(input_stack->contents);
        }
        else {
            input_stack->contents = (long long*)
                                    realloc(input_stack->contents, 
                                            sizeof(long long) * (input_stack->top + 1)
                                    );
            
            if (input_stack->contents == NULL) {
                fprintf(stderr, "Error in pop(): realloc() failed to return a value\n");

                return -1;
            }
        }
        
        return returnVal;
    }
    else {
        fprintf(stderr, "Error in pop(): stack empty, top is: %d\n", input_stack->top);
        
        return -1;
    }
}

void emptyStringStack(stringStack *input_stack) {
    if (input_stack->contents == NULL) return;
    for (int i = 0; i <= input_stack->top; i++) {
        if (input_stack->contents[i] == NULL) continue;
        else free(input_stack->contents[i]);
    }
    input_stack->top = -1;
    free(input_stack->contents);
}

int pushToStringStack(char* string, stringStack *input_stack) {
    if (input_stack->top < -1) {  // ensure top is valid
        fprintf(stderr, "Error in push(): Top is less than -1 somehow?\nTop is %d\n", input_stack->top);
        return -1;
    }
    else {
        if (input_stack->top >= 0) {
            // printf("allocing %d items, %d bytes\n", ( ( input_stack->top + 1 ) + 1 ), ( ( input_stack->top + 1 ) + 1 ) * sizeof(char *));
            input_stack->contents = (char **)
                        realloc(
                            input_stack->contents,
                            ( ( input_stack->top + 1 ) + 1 ) *
                            sizeof(char *)
                        );
            
            if (input_stack->contents == NULL) {
                fprintf(stderr, "Error in push(): malloc() failed to return a value\n");
            
                return -1;
            }

            input_stack->top += 1;
            input_stack->contents[input_stack->top] = calloc( strlen(string) + 1, sizeof(char) );
            strncpy_s(input_stack->contents[input_stack->top], strlen(string) + 1, string, strlen(string) + 1);
            // printf("Value: %s vs %s\n", input_stack->contents[input_stack->top], string);
        }
        else if (input_stack->top == -1) {
            input_stack->contents = (char **) malloc(sizeof(char *));

            if (input_stack->contents == NULL) {
                fprintf(stderr, "Error in push(): malloc() failed to return a value\n");

                return -1;
            }

            input_stack->contents[0] = calloc( strlen(string) + 1, sizeof(char) );
            strncpy_s(input_stack->contents[0], strlen(string) + 1, string, strlen(string) + 1);
            // printf("Value: %s\n", input_stack->contents[0]);
            input_stack->top = 0;
        }
        else {
            fprintf(stderr, "Error in push(): IDFK bro\n");
        }

        return input_stack->top;
    }
}

int popStringStack(char** outputString, stringStack *inputStack) {
    if (inputStack->top >= 0) {
        char* returnVal = inputStack->contents[inputStack->top];
        // printf("Top: %d, value: %s\n", inputStack->top, returnVal);
        inputStack->top--;

        if (inputStack->top < 0) {
            inputStack->contents = NULL;
            free(inputStack->contents);
        }
        else {
            inputStack->contents = (char**)
                                    realloc(inputStack->contents, 
                                            sizeof(char*) * (inputStack->top + 1)
                                    );
            
            if (inputStack->contents == NULL) {
                fprintf(stderr, "Error in pop(): realloc() failed to return a value\n");
                return -1;
            }
        }
        *outputString = calloc(strlen(returnVal) + 1, sizeof(char) );
        strncpy_s(*outputString, strlen(returnVal) + 1, returnVal, strlen(returnVal) + 1);
        return 1;
    }
    else {
        fprintf(stderr, "Error in pop(): stack empty, top is: %d\n", inputStack->top);
        
        return -1;
    }
}

