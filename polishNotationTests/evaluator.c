#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <wchar.h>
#include <string.h>

#include "stack.c"

#define COMPAREPRECEDENCE(x, y) getOperatorPrecedence((x)) - getOperatorPrecedence((y))

#define LEFT 0
#define RIGHT 1

typedef enum {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    EXPONENT,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    DIGIT,
    DECIMAL_POINT,
    LETTER,
    NONE,
} operationType;

operationType operatorType(char character) {
    switch (character) {
        case '+':
            return ADDITION;
        
        case '-':
            return SUBTRACTION;
        
        case '*':
            return MULTIPLICATION;
        
        case '/':
            return DIVISION;
        
        case '^':
            return EXPONENT;
        
        case '(':
            return OPEN_BRACKET;
        
        case ')':
            return CLOSE_BRACKET;

        case '.':
            return DECIMAL_POINT;

        default:
            if (character >= '0' && character <= '9') return DIGIT;
            if ( (character >= 'a' && character <= 'z') ||
                 (character >= 'A' && character <= 'Z')) return LETTER;
            else return NONE;
    }
}

double calculate(double value1, double value2, operationType operation) {
    switch(operation) {
        case ADDITION:
            printf("%g + %g = %g\n", value1, value2, value1 + value2);
            return value1 + value2;
            break;
        case SUBTRACTION:
            printf("%g - %g = %g\n", value1, value2, value1 - value2);
            return value1 - value2;
            break;
        case MULTIPLICATION:
            printf("%g * %g = %g\n", value1, value2, value1 * value2);
            return value1 * value2;
            break;
        case DIVISION:
            printf("%g / %g = %g\n", value1, value2, value1 / value2);
            return value1 / value2;
            break;
        case EXPONENT:
            printf("%g ^ %g = %g\n", value1, value2, pow( value1, value2 ));
            return pow( value1, value2 );
            break;
        case NONE:
            return 0;
    }
    return 0;
}

int makePolishNotation(char* equation, char** outputString);

double parseEquation(char* inputEquation) {
    if (inputEquation[0] == '\0') { printf("empty dumbass\n"); return 1; }

    char* equation = NULL;
    makePolishNotation(inputEquation, &equation);

    stack evaluationStack = INIT_DOUBLE_STACK;

    char *nextNumber = equation;
    double currVal;

    for (int64_t i = 0; i < strlen(equation); i++) {
        // printf("%s\n", nextNumber);
        if (nextNumber[0] == '\0') break;

        if (nextNumber[0] >= '0' && nextNumber[0] <= '9') {
            currVal = strtof(nextNumber, &nextNumber);
            uint64_t i = *(uint64_t*)&currVal;
            pushToStack((void*)i, &evaluationStack);
        }
        else if (operatorType(nextNumber[0]) != NONE) {
            double val1, val2;

            popStack((void*)(&val1), &evaluationStack);
            popStack((void*)(&val2), &evaluationStack);
            currVal = calculate(
                        val2,
                        val1,
                        operatorType(nextNumber[0])
            );

            uint64_t i = *(uint64_t*)&currVal;
            pushToStack(
                 (void*)i,
                 &evaluationStack
                );
        }
        nextNumber += 1;
    }

    double returnVal;
    popStack(&returnVal, &evaluationStack);
    // emptyStack(&evaluationStack);

    printf("Result: %g\n", returnVal);

    return returnVal;
}

int getOperatorPrecedence(char operator) {
    switch (operator) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
    }

    return 0;
}

int getOperatorAssociativeness(char operator) {
    switch (operator) {
        case '^':
            return RIGHT;

        default:
            return LEFT;
    }
}

int makePolishNotation(char* equation, char** outputString) {
    stack operatorStack = INIT_INTEGER_STACK;

    // gives us enough memory for all the extra spaces we will add
    char* outString = calloc( 2 * (strlen(equation) + 1) * sizeof(char), sizeof(char) );
    char* tempString = calloc( (strlen(equation) + 1), sizeof(char) );
    if (outString == NULL || tempString == NULL) return 1;

    int outStringLength = 0, tempStringLength = 0;
    bool doublePop = false;

    for (int i = 0; i < strlen(equation); i++) {
        switch (operatorType(equation[i] ) ) {
            case NONE:
                break;

            case DECIMAL_POINT:
            case DIGIT:
                tempString[tempStringLength] = equation[i];
                tempStringLength++;
                break;

            case OPEN_BRACKET:
                pushToStack((void*)equation[i], &operatorStack);
                break;

            case ADDITION:
            case SUBTRACTION:
            case MULTIPLICATION:
            case DIVISION:
            case EXPONENT:
            case CLOSE_BRACKET:
            {
                int length = strlen(tempString);
                strncat(outString, tempString, length);
                memset(tempString, '\0', tempStringLength);

                tempStringLength = 0;
                
                outStringLength += length;
                outString[outStringLength++] = ' ';

                switch (operatorType(equation[i] ) ) {
                    case ADDITION:
                    case SUBTRACTION:
                    case MULTIPLICATION:
                    case DIVISION:
                    case EXPONENT:
                        if (STACK_ISNT_EMPTY(operatorStack) &&
                            getOperatorAssociativeness(equation[i]) == RIGHT &&
                            getOperatorAssociativeness(operatorStack.integerContents[operatorStack.top]) == RIGHT
                        ) {
                            pushToStack((void*)equation[i], &operatorStack);
                            doublePop = true;
                        }
                        else if (doublePop) {
                            outString[outStringLength++] = popStack(NULL, &operatorStack);
                            outString[outStringLength++] = ' ';
                            outString[outStringLength++] = popStack(NULL, &operatorStack);
                            outString[outStringLength++] = ' ';

                            doublePop = false;
                            
                            pushToStack((void*)equation[i], &operatorStack);
                        }
                        else if (STACK_ISNT_EMPTY(operatorStack) &&
                            COMPAREPRECEDENCE(equation[i], operatorStack.integerContents[operatorStack.top]) <= 0
                           ) {
                            outString[outStringLength++] = popStack(NULL, &operatorStack);
                            outString[outStringLength++] = ' ';
                            pushToStack((void*)equation[i], &operatorStack);
                        }
                        else {
                            pushToStack((void*)equation[i], &operatorStack);
                        }
                        break;

                    case CLOSE_BRACKET:
                        for (int i = 0; i < 100; i++) {
                            char operatorTemp = popStack(NULL, &operatorStack);
                            if (operatorTemp == '(') break;

                            outString[outStringLength++] = operatorTemp;
                            outString[outStringLength++] = ' ';
                        }
                        break;
                }
            }
            break;
        }

    }

    if (tempString[0] != '\0') { 
        int length = strlen(tempString);
        strncat(outString, tempString, length);
        outStringLength += length;
        outString[outStringLength++] = ' ';
        
        while (STACK_ISNT_EMPTY(operatorStack) ) {
            outString[outStringLength++] = popStack(NULL, &operatorStack);
            if (operatorStack.top >= 0) {
                outString[outStringLength] = ' ';
            }
            outStringLength++;
        }
    }

    *outputString = calloc(outStringLength, sizeof(char));
    snprintf(*outputString, outStringLength, "%s", outString);
    printf("%s, %s\n", equation, outString);

    free(outString);
    free(tempString);
    emptyStack(&operatorStack);

    return 0;
}

int main(void) {
    parseEquation("3 ^ 2 ^ 2 - 2 * 4");

    printf("hii\n");
}
