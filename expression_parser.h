#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STACK_SIZE 100

// Stack structure for operators
typedef struct {
    char data[STACK_SIZE];
    int top;
} Stack;

// Initialize the stack
void stackInit(Stack *s) {
    s->top = -1;
}

// Push an item onto the stack
void stackPush(Stack *s, char val) {
    if (s->top < STACK_SIZE - 1) {
        s->top++;
        s->data[s->top] = val;
    } else {
        printf("Stack Overflow\n");
        exit(1);
    }
}

// Pop an item from the stack
char stackPop(Stack *s) {
    if (s->top >= 0) {
        // Retrieve the value at the top of the stack
        char poppedValue = s->data[s->top];
        
        // Decrement the stack top pointer
        s->top--;
        
        // Return the popped value
        return poppedValue;
    } else {
        // Handle underflow case
        printf("Stack Underflow\n");
        exit(1);
    }
}

// Check if a character is an operator
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Get the precedence of an operator
int precedence(char c) {
    if (c == '+' || c == '-') {
        return 1;
    } else if (c == '*' || c == '/') {
        return 2;
    }
    return 0;
}

// Evaluate the given arithmetic expression
double evaluateExpression(const char *expression) {
    Stack operandStack, operatorStack;
    stackInit(&operandStack);
    stackInit(&operatorStack);

    int len = strlen(expression);
    for (int i = 0; i < len; i++) {
        if (isdigit(expression[i])) {
            // If the character is a digit, convert it to a number and push onto the operand stack
            double num = expression[i] - '0';
            stackPush(&operandStack, num);
        } else if (isOperator(expression[i])) {
            // Process operators while respecting precedence
            while (operatorStack.top >= 0 &&
                   isOperator(operatorStack.data[operatorStack.top]) &&
                   precedence(expression[i]) <= precedence(operatorStack.data[operatorStack.top])) {
                char op = stackPop(&operatorStack);
                double b = stackPop(&operandStack);
                double a = stackPop(&operandStack);
                double result;
                switch (op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/': result = a / b; break;
                }
                stackPush(&operandStack, result);
            }
            // Push the current operator onto the operator stack
            stackPush(&operatorStack, expression[i]);
        } else if (expression[i] == '(') {
            // Push an opening parenthesis onto the operator stack
            stackPush(&operatorStack, expression[i]);
        } else if (expression[i] == ')') {
            // Process operators until the corresponding opening parenthesis
            while (operatorStack.top >= 0 && operatorStack.data[operatorStack.top] != '(') {
                char op = stackPop(&operatorStack);
                double b = stackPop(&operandStack);
                double a = stackPop(&operandStack);
                double result;
                switch (op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/': result = a / b; break;
                }
                stackPush(&operandStack, result);
            }
            stackPop(&operatorStack); // Remove the opening parenthesis
        }
    }

    // Process remaining operators
    while (operatorStack.top >= 0) {
        char op = stackPop(&operatorStack);
        double b = stackPop(&operandStack);
        double a = stackPop(&operandStack);
        double result;
        switch (op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/': result = a / b; break;
        }
        stackPush(&operandStack, result);
    }

    // The final result is at the top of the operand stack
    return stackPop(&operandStack);
}

// Example:
//int main() {
//    const char *expression = "(2+1)*2/3-1";
//    double result = evaluateExpression(expression);
//    printf("Result: %lf\n", result);

//    return 0;
//}
