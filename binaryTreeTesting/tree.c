#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef enum {
    NONE = 0,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    EXPONENT,
} operationType;

typedef struct {
    double value1;
    double value2;
    operationType operation;
} mathsOperation;

typedef struct treeNode {
    struct treeNode** childArray;
    uint32_t childrenNumber;
    mathsOperation valuesAndOperation;
} treeNode;

void initialiseTreeNode(treeNode* node) {
    node->childrenNumber = 0;
    node->childArray = NULL;
    node->valuesAndOperation = (mathsOperation){0, 0, 0};
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

    printf("%lf, %lf, %d\n", treeRoot->valuesAndOperation.value1, treeRoot->valuesAndOperation.value2, treeRoot->valuesAndOperation.operation);
    
    if (treeRoot->childrenNumber > 0 && treeRoot->childArray != NULL) {
        for (int i = 0; i < treeRoot->childrenNumber; i++) {
            printTree(treeRoot->childArray[i]);
        }
    }
}

double calculate(mathsOperation equation) {
    switch (equation.operation) {
        case NONE:
            return 0;
        case ADDITION:
            printf("result: %g\n", equation.value1 + equation.value2);
            return equation.value1 + equation.value2;
        case SUBTRACTION:
            printf("result: %g\n", equation.value1 - equation.value2);
            return equation.value1 - equation.value2;
        case MULTIPLICATION:
            printf("result: %g\n", equation.value1 * equation.value2);
            return equation.value1 * equation.value2;
        case DIVISION:
            printf("result: %g\n", equation.value1 / equation.value2);
            return equation.value1 / equation.value2;
        case EXPONENT:
            return pow(equation.value1, equation.value2);
    }
}

double evaluateTree(treeNode root) {
    double result = calculate(root.valuesAndOperation);

    if (root.childrenNumber > 0) {
        for (int i = 0; i < root.childrenNumber; i++) {
            printf("Evaluating %d,", i);
            result += evaluateTree(*(root.childArray[i]));
        }
    }

    return result;
}

int main(void) {
    treeNode* treeRoot;
    treeRoot = malloc(sizeof(treeNode));
    mathsOperation childData = (mathsOperation){23, 42, ADDITION};
    mathsOperation childData2 = (mathsOperation){27, 49, SUBTRACTION};
    initialiseTreeNode(treeRoot);

    treeRoot->valuesAndOperation = (mathsOperation){0, 0, NONE};

    addChild(treeRoot, childData);
    addChild(treeRoot, childData2);
    printTree(treeRoot);

    insertChild(treeRoot, 0, (mathsOperation){34, 89, MULTIPLICATION});
    
    printf("Inserted Child:\n");
    printTree(treeRoot);

    printf("%g\n", evaluateTree(*treeRoot));

    printf("hii\n");
}
