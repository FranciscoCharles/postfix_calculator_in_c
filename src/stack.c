#include "stack.h"

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

Stack stack_create() {
    Stack stack = malloc(sizeof(struct Stack));
    stack->top = NULL;
    stack->size = 0;
    return stack;
}
Stack stack_delete(Stack stack) {
    free(stack);
    return NULL;
}
bool stack_is_empty(Stack stack) {
    return stack->top == NULL;
}
void stack_push(Stack stack, Token novo_token) {
    novo_token->next = stack->top;
    stack->top = novo_token;
    stack->size++;
}
Token stack_pop(Stack stack) {
    Token token = NULL;
    if(!stack_is_empty(stack)) {
        token = stack->top;
        stack->top = token->next;
        token->next = NULL;
        stack->size--;
    }
    return token;
}
Stack stack_reverse(Stack stack) {
    Token token = stack->top;
    Stack inverted_stack = stack_create();
    
    while(!stack_is_empty(stack)) {
        token = stack_pop(stack);
        stack_push(inverted_stack, token);
    }
    stack = stack_delete(stack);
    return inverted_stack;
}

Stack stack_copy(Stack stack) {

    Token token = stack->top;
    Stack copy = stack_create();

    while(token != NULL) {
        stack_push(copy, token_copy(token));
        token = token->next;
    }
    
    return stack_reverse(copy);
}
