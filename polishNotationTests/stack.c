#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

void emptyFloatStack(stack *inputStack) {
    inputStack->top = -1;
    if (inputStack->floatContents == NULL) return;
    free(inputStack->floatContents);
}

int pushToFloatStack(double i, stack *inputStack) {
    if (inputStack->top < -1) {  // ensure top is valid
        fprintf(stderr, "Error in push(): Top is less than -1 somehow?\nTop is %d\n", inputStack->top);
        return -1;
    }
    else {
        if (inputStack->top >= 0) {
            inputStack->floatContents = (double*)
                        realloc(
                            inputStack->floatContents,
                            ( ( inputStack->top + 1 ) + 1 ) *
                            sizeof(double)
                        );
            
            if (inputStack->floatContents == NULL) {
                fprintf(stderr, "Error in push(): malloc() failed to return a value\n");
            
                return -1;
            }

            inputStack->top += 1;
            inputStack->floatContents[inputStack->top] = i;
            // printf("Value: %g\n", inputStack->floatContents[inputStack->top]);
        }
        else if (inputStack->top == -1) {
            inputStack->floatContents = (double*) malloc(sizeof(double));

            if (inputStack->floatContents == NULL) {
                fprintf(stderr, "Error in push(): malloc() failed to return a value\n");

                return -1;
            }

            inputStack->floatContents[0] = i;
            // printf("Value: %g\n", inputStack->floatContents[0]);
            inputStack->top = 0;
        }
        else {
            fprintf(stderr, "Error in push(): IDFK bro\n");
        }

        return inputStack->top;
    }
}

double popFloatStack(stack *inputStack) {
    if (inputStack->top >= 0) {
        double returnVal = inputStack->floatContents[inputStack->top];
        inputStack->top = inputStack->top - 1;

        if (inputStack->top < 0) {
            inputStack->floatContents = NULL;
            free(inputStack->floatContents);
        }
        else {
            inputStack->floatContents = (double *)
                                    realloc(inputStack->floatContents, 
                                            sizeof(double) * (inputStack->top + 1)
                                    );
            
            if (inputStack->floatContents == NULL) {
                fprintf(stderr, "Error in pop(): realloc() failed to return a value\n");

                return -1;
            }
        }

        return returnVal;
    }
    else {
        fprintf(stderr, "Error in pop(): stack empty, top is: %d\n", inputStack->top);
        
        return -1;
    }
}

void emptyIntStack(stack *inputStack) {
    inputStack->top = -1;
    if (inputStack->integerContents == NULL) return;
    free(inputStack->integerContents);
}

int pushToIntStack(int32_t i, stack *input_stack) {
    if (input_stack->top < -1) {  // ensure top is valid
        fprintf(stderr, "Error in push(): Top is less than -1 somehow?\nTop is %d\n", input_stack->top);
        return -1;
    }
    else {
        if (input_stack->top >= 0) {
            input_stack->integerContents = (int32_t*)
                        realloc(
                            input_stack->integerContents,
                            ( ( input_stack->top + 1 ) + 1 ) *
                            sizeof(int32_t)
                        );
            
            if (input_stack->integerContents == NULL) {
                fprintf(stderr, "Error in push(): malloc() failed to return a value\n");
            
                return -1;
            }

            input_stack->top += 1;
            input_stack->integerContents[input_stack->top] = i;
        }
        else if (input_stack->top == -1) {
            input_stack->integerContents = (int32_t*) malloc(sizeof(int32_t));

            if (input_stack->integerContents == NULL) {
                fprintf(stderr, "Error in push(): malloc() failed to return a value\n");

                return -1;
            }

            input_stack->integerContents[0] = i;
            input_stack->top = 0;
        }
        else {
            fprintf(stderr, "Error in push(): IDFK bro\n");
        }

        return input_stack->top;
    }
}

int popIntStack(stack *inputStack) {
    if (inputStack->top >= 0) {
        int returnVal = inputStack->integerContents[inputStack->top];
        inputStack->top = inputStack->top - 1;

        if (inputStack->top < 0) {
            inputStack->integerContents = NULL;
            free(inputStack->integerContents);
        }
        else {
            inputStack->integerContents = (int32_t *)
                                    realloc(inputStack->integerContents, 
                                            sizeof(int32_t) * (inputStack->top + 1)
                                    );
            
            if (inputStack->integerContents == NULL) {
                fprintf(stderr, "Error in pop(): realloc() failed to return a value\n");

                return -1;
            }
        }
        
        return returnVal;
    }
    else {
        fprintf(stderr, "Error in pop(): stack empty, top is: %d\n", inputStack->top);
        
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

int pushToStack(void* value, stack* inputStack) {
     switch(inputStack->type) {
        case INTEGER:
            return pushToIntStack((int32_t)value, inputStack);
            break;
        case DOUBLE:
            double y  = * ( double * ) &value;
            return pushToFloatStack((double)y, inputStack);
            break;
        // case STRING:
        //     return pushToStringStack((char*)value, STACK_TO_STRING_STACK(inputStack));
        //     break;
    }   
}

int popStack(void* output, stack* inputStack) {
     switch(inputStack->type) {
        case INTEGER:
            return popIntStack(inputStack);
            break;
        case DOUBLE:
            *((double*)output) = popFloatStack(inputStack);
            return 0;
            break;
        // case STRING:
        //     return pushToStringStack((char*)value, STACK_TO_STRING_STACK(inputStack));
        //     break;
    }   
}

void emptyStack(stack *inputStack) {
     switch(inputStack->type) {
        case INTEGER:
            return emptyIntStack(inputStack);
            break;
        // case STRING:
        //     return pushToStringStack((char*)value, STACK_TO_STRING_STACK(inputStack));
        //     break;
    }
}
