#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <wchar.h>

typedef enum {
    NONE = 0,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    EXPONENT,
} operationType;

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

typedef struct treeNode {
    struct treeNode** childArray;
    uint32_t childrenNumber;
    mathsOperation valuesAndOperation;
} treeNode;

void initialiseTreeNode(treeNode* node) {
    node->childrenNumber = 0;
    node->childArray = NULL;
    node->valuesAndOperation = (mathsOperation){
        .operationOrValue = OPERATION,
        .operation = NONE,
    };
}

treeNode* insertChild(treeNode* parent, int child, mathsOperation childValue) {
    treeNode* newChild = malloc(sizeof(treeNode));
    if (newChild == NULL) return NULL;

    initialiseTreeNode(newChild);
    newChild->valuesAndOperation = childValue;
    
    newChild->childArray = malloc(sizeof(treeNode*));
    if (newChild->childArray == NULL) return NULL;

    newChild->childArray[0] = parent->childArray[child];
    newChild->childrenNumber += 1;

    parent->childArray[child] = newChild;

    return newChild;
}

treeNode* addChild(treeNode* parent, mathsOperation childValue) {
    treeNode* child = malloc(sizeof(treeNode));
    if (child == NULL) return NULL;

    initialiseTreeNode(child);

    child->valuesAndOperation = childValue;

    parent->childArray = parent->childrenNumber == 0 ? malloc(sizeof(treeNode*)) : realloc(parent->childArray, sizeof(treeNode*) * parent->childrenNumber);
    if (parent->childArray == NULL) return NULL;


    parent->childArray[parent->childrenNumber] = child;
    parent->childrenNumber += 1;
    return child;
}

void printTree(treeNode* treeRoot) {
    if (treeRoot == NULL) return;

    if (treeRoot->childrenNumber > 0 && treeRoot->childArray != NULL) {
        for (int i = 0; i < treeRoot->childrenNumber; i++) {
            printTree(treeRoot->childArray[i]);
        }
    }

    switch (treeRoot->valuesAndOperation.operationOrValue) {
        case VALUE:
            printf("VALUE: %lf\n", treeRoot->valuesAndOperation.value);
            break;
        case OPERATION:
            printf("OPERATION: %d\n", treeRoot->valuesAndOperation.operation);
            break;
    }
}

// double calculate(mathsOperation equation) {
//     switch (equation.operation) {
//         case NONE:
//             return 0;
//         case ADDITION:
//             printf("result: %g\n", equation.value1 + equation.value2);
//             return equation.value1 + equation.value2;
//         case SUBTRACTION:
//             printf("result: %g\n", equation.value1 - equation.value2);
//             return equation.value1 - equation.value2;
//         case MULTIPLICATION:
//             printf("result: %g\n", equation.value1 * equation.value2);
//             return equation.value1 * equation.value2;
//         case DIVISION:
//             printf("result: %g\n", equation.value1 / equation.value2);
//             return equation.value1 / equation.value2;
//         case EXPONENT:
//             return pow(equation.value1, equation.value2);
//     }
// }

double evaluateTree(treeNode root) {
    if (root.valuesAndOperation.operationOrValue == VALUE) return root.valuesAndOperation.value;

    switch(root.valuesAndOperation.operation) {
        case ADDITION:
            return evaluateTree(*(root.childArray[0] ) ) + evaluateTree(*( root.childArray[1] ) );
            break;
        case SUBTRACTION:
            return evaluateTree(*(root.childArray[0] ) ) - evaluateTree(*( root.childArray[1] ) );
            break;
        case MULTIPLICATION:
            return evaluateTree(*(root.childArray[0] ) ) * evaluateTree(*( root.childArray[1] ) );
            break;
        case DIVISION:
            return evaluateTree(*(root.childArray[0] ) ) / evaluateTree(*( root.childArray[1] ) );
            break;
        case EXPONENT:
            return pow( evaluateTree(*(root.childArray[0] ) ), evaluateTree(*( root.childArray[1] ) ) );
            break;
    }

    // double result = calculate(root.valuesAndOperation);

    // if (root.childrenNumber > 0) {
    //     for (int i = 0; i < root.childrenNumber; i++) {
    //         printf("Evaluating %d,", i);
    //         result += evaluateTree(*(root.childArray[i]));
    //     }
    // }

    // return result;
}

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

int main(void) {
    wchar_t* testEquation = L"3*4+69-12/4";
    equationParser(testEquation);

    /*
     *         -
     *       /   \
     *      +     \
     *     / \     /
     *    *  69   / \
     *   / \     12  4
     *  3   4
     */
    treeNode* treeRoot;
    treeRoot = malloc(sizeof(treeNode));
    initialiseTreeNode(treeRoot);
    treeRoot->valuesAndOperation.operationOrValue = OPERATION;
    treeRoot->valuesAndOperation.operation = SUBTRACTION;

    mathsOperation childData = (mathsOperation) {
        .operationOrValue = OPERATION,
        .operation = ADDITION
    };
    mathsOperation childData1_1 = (mathsOperation) {
        .operationOrValue = OPERATION,
        .operation = MULTIPLICATION
    };
    mathsOperation childData2 = (mathsOperation) {
        .operationOrValue = OPERATION,
        .operation = DIVISION
    };
    mathsOperation childData1_2 = (mathsOperation) {
        .operationOrValue = VALUE,
        .value = 69
    };
    mathsOperation childData2_1 = (mathsOperation) {
        .operationOrValue = VALUE,
        .value = 12
    };
    mathsOperation childData2_2 = (mathsOperation) {
        .operationOrValue = VALUE,
        .value = 4
    };
    mathsOperation childData1_1_1 = (mathsOperation) {
        .operationOrValue = VALUE,
        .value = 3
    };
    mathsOperation childData1_1_2 = (mathsOperation) {
        .operationOrValue = VALUE,
        .value = 4
    };

    // treeRoot->valuesAndOperation = (mathsOperation){0, 0, NONE};

    treeNode* _1 = addChild(treeRoot, childData);
    treeNode* _2 = addChild(treeRoot, childData2);
    treeNode* _3 = addChild(_1, childData1_1);
    treeNode* _4 = addChild(_1, childData1_2);
    treeNode* _5 = addChild(_2, childData2_1);
    treeNode* _6 = addChild(_2, childData2_2);
    treeNode* _7 = addChild(_3, childData1_1_1);
    treeNode* _8 = addChild(_3, childData1_1_2);

    // treeNode* multiply = insertChild(treeRoot, 0, (mathsOperation) {
    //     .operationOrValue = OPERATION,
    //     .operation = MULTIPLICATION
    // });
    
    // addChild(multiply, (mathsOperation){VALUE, 4});
    // /*
    //  *          -
    //  *         / \
    //  *        *  32
    //  *       / \
    //  *      42  4
    //  * */

    printf("Inserted Child:\n");
    printTree(treeRoot);

    printf("%g\n", evaluateTree(*treeRoot));

    printf("hii\n");
}
