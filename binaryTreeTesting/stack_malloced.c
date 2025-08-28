#include <stdio.h>
#include <stdlib.h>

#include "malloced_stack_variables.h"

void emptyIntStack(integerStack *input_stack) {
    input_stack->top = -1;
    if (input_stack->contents == NULL) { printf("already empty\n"); return;}
    free(input_stack->contents);
}

int pushToIntStack(long long i, integerStack *input_stack) {
    if (input_stack->top < -1) {  // ensure top is valid
        fprintf(stderr, "Error in push(): Top is less than -1 somehow?\nTop is %d\n", input_stack->top);
        return -1;
    }
    else {
        if (input_stack->top >= 0) {
            printf("allocing %d items, %d bytes\n", ( ( input_stack->top + 1 ) + 1 ), ( ( input_stack->top + 1 ) + 1 ) * sizeof(long long));
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
            printf("Value: %d\n", input_stack->contents[0]);
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
        printf("Top: %d, value: %d\n", input_stack->top, returnVal);
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

// int pushToStringStack(char* string, stringStack *input_stack) {
//     if (input_stack->top < 0) {  // ensure top is valid
//         fprintf(stderr, "Error in push(): Top is less than zero\nTop is %d\n", input_stack->top);
//         return -1;
//     }
//     else {
//         if (input_stack->top) {
//             input_stack->contents = (char**) realloc(input_stack->contents, (sizeof(input_stack->contents) + sizeof(long long)));
            
//             if (input_stack->contents == NULL) {
//                 fprintf(stderr, "Error in push(): malloc() failed to return a value\n");
            
//                 return -1;
//             }

//             input_stack->contents[input_stack->top++] = i;
//         }
//         else if (!(input_stack->top)) {
//             input_stack->contents = (long long*) malloc(sizeof(long long));

//             if (input_stack->contents == NULL) {
//                 fprintf(stderr, "Error in push(): malloc() failed to return a value\n");

//                 return -1;
//             }

//             input_stack->contents[input_stack->top] = i;
//             input_stack->top += 1;
//         }
//         else {
//             fprintf(stderr, "Error in push(): IDFK bro\n");
//         }

//         return input_stack->top;
//     }
// }


// // Always empty the stack at the end of a program using make_empty()
// // take a value from the top of the stack
// int popIntStack(integerStack *input_stack) { // pointer to the stack variable

//     // printf("Top: %d\n", input_stack->top);
//     input_stack->top = (input_stack->top - 1) < 0 ? -1 : input_stack->top - 1;

//     if (input_stack->top >= 0) {
//         // printf("Top: %d\n", input_stack->top);
        
//         input_stack->contents = (long long*) realloc(input_stack->contents, (sizeof(input_stack->top)));
        
//         if (input_stack->contents == NULL) {
//             fprintf(stderr, "Error in pop(): realloc() failed to return a value\n");

//             return -1;
//         }
        
//         return input_stack->contents[input_stack->top];
//     }
//     else {
//         fprintf(stderr, "Error in pop(): Top is less than zero\nTop is %d\n", input_stack->top);
        
//         return -1;
//     }
// }
