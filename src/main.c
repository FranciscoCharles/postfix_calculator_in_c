#include "calc.h"

int main(int argc, char** argv) {
    
	Stack stack = NULL;
    Stack copy = NULL;
    Stack stack_exp = NULL;
    
    char str_expression[80]= "";
    bool _continue = true;
    
    while(_continue) {
        printf("enter the expression :\n");
        scanf(" %79[^\n]s",str_expression);
        if(strcmp(str_expression,"q")==0 || strcmp(str_expression,"quit")==0) {
            _continue = false;
        } else {
            stack = tokenize(stack_create(), str_expression);
            if(stack != NULL) {

                printf("infix notation :\n\t");
                expression_print(stack);
                
                copy = stack_copy(stack);
                stack_exp = postfix_notation(copy);
                
                printf("postfix notation :\n\t");
                expression_print(stack_exp);
                
                printf("result = %d\n", evaluate_expression(stack_exp));
                stack = stack_delete(stack);
                
                stack_exp = stack_delete(stack_exp);
                copy = stack_delete(copy);

            } else {
                printf("Error: expression invalid!\n");
            }
        }
    }
    return 0;
}
