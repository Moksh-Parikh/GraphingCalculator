#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <wchar.h>
#include <string.h>

#include "stack_malloced.c"

#define COMPAREPRECEDENCE(x, y) getOperatorPrecedence((x)) - getOperatorPrecedence((y))

#define INIT_STACK(x) { (x).top = -1; (x).contents = NULL; }

#define STACK_IS_EMPTY(x) ((x).top < 0) + ((x).top >= 0)

#define STACK_ISNT_EMPTY(x) !STACK_IS_EMPTY(x)

typedef enum {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    EXPONENT,
    NONE,
} operationType;

typedef struct {
    uint64_t* array;
    uint32_t arraySize;
} variableLengthIntArray;

typedef enum {
    VALUE,
    OPERATION
} nodeType;

typedef struct {
    nodeType operationOrValue;
    union {
        double value;
        operationType operation;
    };
} mathsOperation;

// typedef struct {
//     double value1;
//     double value2;
//     operationType operation;
// } mathsOperation;

int equationParser(wchar_t* equation) {
    int stringLength = wcslen(equation);

    wchar_t* operatorsArray[5] = {
        L"+",
        L"-",
        L"*",
        L"/",
        L"^"
    };

    wchar_t* tempEquation = malloc((stringLength + 1) * sizeof(wchar_t) );
    if (tempEquation == NULL) return 1;

    wcsncpy_s(tempEquation, stringLength + 1, equation, stringLength);
    wprintf(L"%ls\n", tempEquation);

    for (int i = 0; i < stringLength; i++) {
        if (wcslen(tempEquation) == 0) break;

        for (int j = 0; j < 5; j++) {
            if (wcsstr(tempEquation, operatorsArray[j] ) ) {
                tempEquation = realloc(tempEquation, sizeof(wchar_t) * (stringLength - i + 1));
                if (tempEquation == NULL) return 1;

                wcsncpy_s(tempEquation, stringLength - i + 1, equation + i, stringLength - i);

                wprintf(L"%ls\n", tempEquation);
                break;
            }
        }
    }
}

operationType findOperator(char* equation, char** operatorsArray) { 
    bool breakFlag = false;
    int stringOffset = 0, operatorArrayOffset = 0;

    for (stringOffset = 0; stringOffset < strlen(equation); stringOffset++) {
        if (breakFlag) break;
        for (operatorArrayOffset = 0; operatorArrayOffset < 5; operatorArrayOffset++) {
            if(!strncmp(equation + stringOffset, operatorsArray[operatorArrayOffset], strlen(operatorsArray[operatorArrayOffset]) ) ) {
                breakFlag = true;
                break;
            }
        }
    }

    return operatorArrayOffset;
}

int addArrayDataToIntegerVLA(int* data, int dataSize, variableLengthIntArray* array) {
    // yes, future me, this can be done with a one liner but it looks ugly.
    //
    // like you
    if (array->array == NULL) array->array = malloc(dataSize * sizeof(int64_t));
    else array->array = realloc(array->array, (array->arraySize + dataSize) * sizeof(int64_t) );

    if (array->array == NULL) return 1;

    array->arraySize = array->arraySize + dataSize;

    for (int i = 0; i < array->arraySize; i++) {
        array->array[i] = data[i];
    }

    return 0;
}

int addSingleDataToIntegerVLA(int64_t data, variableLengthIntArray* array) {
    if (array->array == NULL) array->array = malloc(sizeof(int64_t));
    else array->array = realloc(array->array, (array->arraySize + 1) * sizeof(int64_t) );

    if (array->array == NULL) return 1;

    array->array[array->arraySize] = data;
    array->arraySize = array->arraySize + 1;
    
    return 0;
}

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

        default:
            return NONE;
    }
}

double calculate(double value1, double value2, operationType operation) {
    switch(operation) {
        case ADDITION:
            return value1 + value2;
            break;
        case SUBTRACTION:
            return value1 - value2;
            break;
        case MULTIPLICATION:
            return value1 * value2;
            break;
        case DIVISION:
            return value1 / value2;
            break;
        case EXPONENT:
            return pow( value1, value2 );
            break;
        case NONE:
            return 0;
    }
    return 0;
}

int makePolishNotation(char* equation, char** outputString);

int parseEquation(char* inputEquation) {
    if (inputEquation[0] == '\0') { printf("empty dumbass\n"); return 1; }

    char* equation = NULL;
    makePolishNotation(inputEquation, &equation);

    integerStack evaluationStack;
    INIT_STACK(evaluationStack);

    char *nextNumber = equation;
    double currVal;

    for (int64_t i = 0; i < strlen(equation); i++) {
        // printf("%s\n", nextNumber);
        if (nextNumber[0] == '\0') break;

        if (nextNumber[0] >= '0' && nextNumber[0] <= '9') {
            currVal = strtof(nextNumber, &nextNumber);
            // printf("%g, %c\n", currVal, nextNumber[0]);
            pushToIntStack((long long)currVal, &evaluationStack);
        }
        else if (operatorType(nextNumber[0]) != NONE) {
            currVal = calculate(
                        popIntStack(&evaluationStack),
                        popIntStack(&evaluationStack),
                        operatorType(nextNumber[0])
            );

            // printf("%d\n", currVal);
            pushToIntStack(
                 (long long)currVal,
                 &evaluationStack
                );
        }
        nextNumber += 1;
    }

    int returnVal = popIntStack(&evaluationStack);
    emptyIntStack(&evaluationStack);

    printf("%d\n", returnVal);

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

int makePolishNotation(char* equation, char** outputString) {
    integerStack operatorStack;
    INIT_STACK(operatorStack);

    // gives us enough memory for all the extra spaces we will add
    char* outString = calloc( 2 * (strlen(equation) + 1) * sizeof(char), sizeof(char) );
    char* tempString = calloc( (strlen(equation) + 1), sizeof(char) );
    if (outString == NULL || tempString == NULL) return 1;

    int outStringLength = 0, tempStringLength = 0;

    for (int i = 0; i < strlen(equation); i++) {
        if (equation[i] >= '0' && equation[i] <= '9') {
            tempString[tempStringLength] = equation[i];
            tempStringLength++;
        }
        else if (operatorType(equation[i]) != NONE) {
            int length = strlen(tempString);
            strncat(outString, tempString, length);
            memset(tempString, '\0', tempStringLength);

            tempStringLength = 0;
            
            outStringLength += length;
            outString[outStringLength++] = ' ';
            

            if (STACK_ISNT_EMPTY(operatorStack) &&
                COMPAREPRECEDENCE(equation[i], operatorStack.contents[operatorStack.top]) <= 0
               ) {
                outString[outStringLength++] = popIntStack(&operatorStack);
                outString[outStringLength++] = ' ';
                pushToIntStack(equation[i], &operatorStack);
            }
            else {
                pushToIntStack(equation[i], &operatorStack);
            }
        }
    }

    if (tempString[0] != '\0') { 
        int length = strlen(tempString);
        strncat(outString, tempString, length);
        outStringLength += length;
        outString[outStringLength++] = ' ';
        
        while (STACK_ISNT_EMPTY(operatorStack) ) {
            outString[outStringLength++] = popIntStack(&operatorStack);
            if (operatorStack.top >= 0) {
                outString[outStringLength] = ' ';
            }
            outStringLength++;
        }
    }

    *outputString = calloc(outStringLength, sizeof(char));
    snprintf(*outputString, outStringLength, "%s", outString);

    free(outString);
    free(tempString);
    emptyIntStack(&operatorStack);

    return 0;
}

bool isCommutative(operationType operation) {
    switch (operation) {
        case ADDITION:
        case MULTIPLICATION:
            return true;

        case DIVISION:
        case SUBTRACTION:
            return false;
    }

    return false;
}

int main(void) {
    parseEquation("3 * 4 + 69 - 12 / 4");

    // makePolishNotation("3*4+69-12/4");

    printf("hii\n");
}
