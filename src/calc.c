#include "calc.h"

/*
MIT License

Copyright (c) 2021 FranciscoCharles

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

bool str_is_operator(char *str) {
    int is_operator = false;

    if(strcmp("+",str)==0) is_operator = true;
    else if(strcmp("-",str)==0) is_operator = true;
    else if(strcmp("*",str)==0) is_operator = true;
    else if(strcmp("/",str)==0) is_operator = true;
    else if(strcmp("^",str)==0) is_operator = true;
    return is_operator;

}
bool str_is_number(char *str) {
    int i = 0;
    while(str[i]!='\0' && isdigit(str[i])) {
        i++;
    }
    return i>0 && str[i]=='\0';
}
void skip_spaces(int *index, char* expression) {
    while(expression[*index]==' ' && expression[*index]) {
        (*index)++;
    }
}
void read_operator(char *read, char* expression, int *index) {

    int i = 0;
    strcpy(read,"");
    while(expression[*index] && expression[*index]!=' ') {
        read[i] = expression[*index];
        (*index)++;
        i++;
    }
    read[i] = '\0';
}
void expression_print(Stack expression) {
    if(stack_is_empty(expression)) {
        printf("\n");
    } else {
        Token token = expression->top;
        while(token != NULL) {
            token_print(token);
            token = token->next;
        }
        printf("\n");
    }

}
Stack tokenize(Stack stack, char* expression) {


    char read[10]="";
    Token token = NULL;
    bool error = false;
    bool must_be_a_number = true;
    bool must_be_a_operator = false;
    int index = 0;
    int open_parenthesis = 0;
    int closed_parenthesis = 0;
    size_t size = strlen(expression);

    while(!error && index<size && expression[index]) {

        skip_spaces(&index, expression);
        read_operator(read, expression, &index);

        if(strcmp("(",read)==0) {
            open_parenthesis += 1;
            token = token_create();
            token->operator = '(';
            stack_push(stack, token);

        } else if(strcmp(")",read)==0) {
            closed_parenthesis += 1;
            token = token_create();
            token->operator = ')';
            stack_push(stack, token);

        } else if(must_be_a_operator && str_is_operator(read)) {
            token = token_create();
            token->operator = read[0];
            stack_push(stack,token);

            must_be_a_operator = false;
            must_be_a_number = true;

        } else if(must_be_a_number && str_is_number(read)) {
            token = token_create();
            token->is_number = true;
            token->number = atoi(read);
            stack_push(stack,token);

            must_be_a_operator = true;
            must_be_a_number = false;

        } else {
            error = true;
        }
        if(closed_parenthesis > open_parenthesis) {
            error = true;
        }
    }
    token = NULL;
    if(error || stack->size < 3) {
        stack = stack_delete(stack);
    } else {
        stack = stack_reverse(stack);
    }
    return stack;
}
uint_least8_t operator_priority(Token token) {
    uint_least8_t priority = 0;
    switch(token->operator) {
        case '^':
            priority = 4;
        case '/':
        case '*':
            priority = 3;
            break;
        case '-':
        case '+':
            priority = 2;
            break;
        default:
            priority = 1;
            break;
    }
    return priority;
}
Stack postfix_notation(Stack stack) {

    Token token = NULL;
    Token token_tmp = NULL;

    Stack expression = stack_create();
    Stack operators = stack_create();

    while(!stack_is_empty(stack)) {

        token = stack_pop(stack);
        if(token->is_number) {
            stack_push(expression, token);

        } else if(token->operator=='(') {
            stack_push(operators, token);

        } else if(token->operator==')') {
            token = token_delete(token);
            token = stack_pop(operators);

            while(token!=NULL && token->operator!='(') {
                stack_push(expression, token);
                token = stack_pop(operators);
            }
        } else {

            while(!stack_is_empty(operators) && operator_priority(operators->top)>=operator_priority(token)) {
                token_tmp = stack_pop(operators);
                stack_push(expression, token_tmp);
            }
            stack_push(operators, token);
        }
    }

    while(!stack_is_empty(operators)) {
        token = stack_pop(operators);
        stack_push(expression, token);
    }
    operators = stack_delete(operators);
    return stack_reverse(expression);
}
int calc_expression(char operator, Token left, Token right, bool *error) {
    int result = 0;
    switch( operator) {
        case '^':
            result = (int)powf(left->number, right->number);
            break;
        case '+':
            result = left->number + right->number;
            break;
        case '-':
            result = left->number - right->number;
            break;
        case '*':
            result = left->number * right->number;
            break;
        case '/':
            if(right->number == 0) {
                *error = true;
            } else {
                result = left->number / right->number;
            }
            break;
    }
    return result;
}
int evaluate_expression(Stack stack) {

    int result = 0;
    bool error = false;
    Token left = NULL;
    Token right = NULL;
    Token token = NULL;
    Stack stack_result = stack_create();

    while(!stack_is_empty(stack) && !error) {

        token = stack_pop(stack);
        if(token->is_number) {
            stack_push(stack_result, token);
        } else {

            right = stack_pop(stack_result);
            left = stack_pop(stack_result);
            result = calc_expression(token->operator, left, right, &error);

            if(!error) {
                left = token_delete(left);
                right = token_delete(right);

                token->is_number = true;
                token->number = result;
                stack_push(stack_result, token);
            }

        }
    }
    if(!error) {
        result = stack_result->top->number;
    }
    stack_result = stack_delete(stack_result);
    return result;
}
