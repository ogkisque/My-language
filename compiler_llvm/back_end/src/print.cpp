#include "print.h"
#include "operators.h"

Error print_funcs (const Functions* funcs, FILE* file)
{
    if (!file)
        RETURN_ERROR(NULL_POINTER, "Null pointer of file");
    if (!funcs)
        RETURN_ERROR(NULL_POINTER, "Null pointer of funcs");

    Error error = {};
    IfWhileId if_while_id = {};

    error = print_header (file);
    PARSE_ERROR_WITHOUT_TREE(error);

    for (size_t i = 0; i < funcs->num_funcs; i++)
    {
        error = print_func_decl (funcs->funcs[i], &if_while_id, file);
        PARSE_ERROR_WITHOUT_TREE(error);
    }
    
    RETURN_ERROR(CORRECT, "");
}

Error print_header (FILE* file)
{
    if (!file)
        RETURN_ERROR(NULL_POINTER, "Null pointer of file");

    fprintf (file, LLVM_HEADER);

    RETURN_ERROR(CORRECT, "");
}

Error print_func_decl (Function* func, IfWhileId* if_while_id, FILE* file)
{
    if (!file)
        RETURN_ERROR(NULL_POINTER, "Null pointer of file");
    if (!func)
        RETURN_ERROR(NULL_POINTER, "Null pointer of func");
    
    func->curr_var_id = 1;
    Error error = {};

    if (strcmp (func->root->name, "main") == 0)
        fprintf (file, "%s%s(", LLVM_MAIN_HEADER, func->root->name);
    else
        fprintf (file, "%s%s(", LLVM_FUNC_HEADER, func->root->name);

    if (func->root->left)
    {
        error = print_args (func->root->left, func, file);
        PARSE_ERROR_WITHOUT_TREE(error);
    }

    fprintf (file, ")\n{\n");

    int alloc_vars[MAX_NUM_VARS] = {};

    if (func->root->left)
    {
        error = print_args_store (func->root->left, alloc_vars, func, file);
        PARSE_ERROR_WITHOUT_TREE(error);
    }

    error = print_commands (func->root->right, if_while_id, alloc_vars, func, file);
    PARSE_ERROR_WITHOUT_TREE(error);

    if (strcmp (func->root->name, "main") == 0)
        fprintf (file, "ret i32 0\n");
    fprintf (file, "}\n\n");

    RETURN_ERROR(CORRECT, "");
}

Error print_args (const Node* node, Function* func, FILE* file)
{
    if (!file)
        RETURN_ERROR(NULL_POINTER, "Null pointer of file");
    if (!func)
        RETURN_ERROR(NULL_POINTER, "Null pointer of func");
    if (!node)
        RETURN_ERROR(NULL_POINTER, "Null pointer of node");

    fprintf (file, "double %%%s", node->left->name);

    if (node->right)
    {
        fprintf (file, ", ");
        Error error = print_args (node->right, func, file);
        PARSE_ERROR_WITHOUT_TREE(error);
    }

    RETURN_ERROR(CORRECT, "");
}

Error print_args_store (const Node* node, int* alloc_vars, Function* func, FILE* file)
{
    if (!file)
        RETURN_ERROR(NULL_POINTER, "Null pointer of file");
    if (!func)
        RETURN_ERROR(NULL_POINTER, "Null pointer of func");
    if (!node)
        RETURN_ERROR(NULL_POINTER, "Null pointer of node");

    print_alloca_var (file, node->left->name);
    print_store_var_var (file, node->left->name, node->left->name);
    alloc_vars[get_num_var(node->left->name)] = 1;

    if (node->right)
    {
        Error error = print_args_store (node->right, alloc_vars, func, file);
        PARSE_ERROR_WITHOUT_TREE(error);
    }

    RETURN_ERROR(CORRECT, "");
}

Error print_commands (const Node* node, IfWhileId* if_while_id, int* alloc_vars, Function* func, FILE* file)
{
    if (!node)
        RETURN_ERROR(CORRECT, "");

    Error error = {};
    bool is_print = false;
    if (node->type == OPER)
    {
        if ((int) node->value == END_STR)
        {
            error = print_commands (node->left, if_while_id, alloc_vars, func, file);
            PARSE_ERROR_WITHOUT_TREE(error);

            error = print_commands (node->right, if_while_id, alloc_vars, func, file);
            PARSE_ERROR_WITHOUT_TREE(error);

            RETURN_ERROR(CORRECT, "");
        }

        error = print_input_output_return (node, alloc_vars, &is_print, func, file);
        PARSE_ERROR_WITHOUT_TREE(error);
        if (is_print)
            RETURN_ERROR(CORRECT, "");

        error = print_assign (node, &is_print, alloc_vars, func, file);
        PARSE_ERROR_WITHOUT_TREE(error);
        if (is_print)
            RETURN_ERROR(CORRECT, "");

        error = print_if (node, if_while_id, &is_print, alloc_vars, func, file);
        PARSE_ERROR_WITHOUT_TREE(error);
        if (is_print)
            RETURN_ERROR(CORRECT, "");

        error = print_while (node, if_while_id, &is_print, alloc_vars, func, file);
        PARSE_ERROR_WITHOUT_TREE(error);
        if (is_print)
            RETURN_ERROR(CORRECT, "");
    }

    RETURN_ERROR(SYNTAX_ERR, "Unknown operator");
}

Error print_input_output_return (const Node* node, int* alloc_vars, bool* is_print, Function* func, FILE* file)
{
    *is_print = false;

    if ((int) node->value == INPUT_VAR)
    {
        if (alloc_vars[get_num_var (node->right->name)] == 0)
        {
            print_alloca_var (file, node->right->name);
            alloc_vars[get_num_var(node->right->name)] = 1;
        }

        fprintf (file, "%%%d = ", func->curr_var_id);
        func->curr_var_id++;
        fprintf (file, LLVM_CALL_SCANF, node->right->name);

        *is_print = true;
    }

    if ((int) node->value == PRINT)
    {
        print_load_reg_var (file, func->curr_var_id, node->right->name);
        func->curr_var_id++;
        fprintf (file, "%%%d = ", func->curr_var_id);
        fprintf (file, LLVM_CALL_PRINTF, func->curr_var_id - 1);
        func->curr_var_id++;

        *is_print = true;
    }

    if ((int) node->value == RET)
    {
        print_load_reg_var (file, func->curr_var_id, node->right->name);
        fprintf (file, "ret double %%%d\n", func->curr_var_id);
        func->curr_var_id++;
        *is_print = true;
    }

    RETURN_ERROR(CORRECT, "");
}

Error print_assign (const Node* node, bool* is_print, int* alloc_vars, Function* func, FILE* file)
{
    if ((int) node->value != ASSIGN)
    {
        *is_print = false;
        RETURN_ERROR(CORRECT, "");
    }

    *is_print = true;
    Error error = {};

    int var = 0;
    error = print_expression (node->left, func, &var, file);
    PARSE_ERROR_WITHOUT_TREE(error);

    if (alloc_vars[get_num_var (node->right->name)] == 0)
    {
        print_alloca_var (file, node->right->name);
        alloc_vars[get_num_var(node->right->name)] = 1;
    }

    print_load_reg_reg (file, func->curr_var_id, var);
    print_store_reg_var (file, func->curr_var_id, node->right->name);
    func->curr_var_id++;

    RETURN_ERROR(CORRECT, "");
}

Error print_expression (const Node* node, Function* func, int* var, FILE* file)
{
    if (!node)
        RETURN_ERROR(CORRECT, "");

    Error error = {};

    if (node->type == OPER)
    {
        int var1 = 0;
        error = print_expression (node->left, func, &var1, file);
        PARSE_ERROR_WITHOUT_TREE(error);

        int var2 = 0;
        error = print_expression (node->right, func, &var2, file);
        PARSE_ERROR_WITHOUT_TREE(error);

        print_load_reg_reg (file, func->curr_var_id, var1);
        var1 = func->curr_var_id;
        func->curr_var_id++;
        print_load_reg_reg (file, func->curr_var_id, var2);
        var2 = func->curr_var_id;
        func->curr_var_id++;

        fprintf (file, "%%%d = ", func->curr_var_id);
        func->curr_var_id++;

        for (size_t i = 0; i < NUM_OPERS; i++)
            if ((int) node->value == OPERATORS[i].id)
            {
                if (OPERATORS[i].name_to_print_asm)
                    fprintf (file, "%s ", OPERATORS[i].name_to_print_asm);
                else
                    RETURN_ERROR(SYNTAX_ERR, "Assembler don't know this operator");
            }

        fprintf (file, "double %%%d, %%%d\n", var1, var2);

        *var = func->curr_var_id;
        print_alloca_reg (file, func->curr_var_id);
        print_store_reg_reg (file, func->curr_var_id - 1, func->curr_var_id);
        func->curr_var_id++;

        RETURN_ERROR(CORRECT, "");
    }

    if (node->type == VAR)
    {
        print_alloca_reg (file, func->curr_var_id);
        func->curr_var_id++;
        print_load_reg_var (file, func->curr_var_id, node->name);
        print_store_reg_reg (file, func->curr_var_id, func->curr_var_id - 1);
        *var = func->curr_var_id - 1;
        func->curr_var_id++;
        RETURN_ERROR(CORRECT, "");
    }

    if (node->type == NUM)
    {
        print_alloca_reg (file, func->curr_var_id);
        print_store_num_reg (file, node->value, func->curr_var_id);
        *var = func->curr_var_id;
        func->curr_var_id++;
        RETURN_ERROR(CORRECT, "");
    }

    if (node->type == FUNC)
    {
        error = print_func_call (node, var, func, file);
        PARSE_ERROR_WITHOUT_TREE(error);
        RETURN_ERROR(CORRECT, "");
    }

    RETURN_ERROR(SYNTAX_ERR, "Unknown type");
}

Error print_func_call (const Node* node, int* var, Function* func, FILE* file)
{
    Error error = {};

    int num_var0 = func->curr_var_id;
    error = print_args_call_load (node->left, func, file);
    PARSE_ERROR_WITHOUT_TREE(error);

    fprintf (file, "%%%d = call double @%s(", func->curr_var_id, node->name);
    func->curr_var_id++;

    error = print_args_call (node->left, num_var0, func, file);
    PARSE_ERROR_WITHOUT_TREE(error);

    fprintf (file, ")\n");

    print_alloca_reg (file, func->curr_var_id);
    *var = func->curr_var_id;
    print_store_reg_reg (file, func->curr_var_id - 1, func->curr_var_id);
    func->curr_var_id++;

    RETURN_ERROR(CORRECT, "");
}

Error print_args_call_load (const Node* node, Function* func, FILE* file)
{
    if (!node)
        RETURN_ERROR(CORRECT, "");

    Error error = {};

    print_load_reg_var (file, func->curr_var_id, node->left->name);
    func->curr_var_id++;
    
    error = print_args_call_load (node->right, func, file);
    PARSE_ERROR_WITHOUT_TREE(error);
    RETURN_ERROR(CORRECT, "");
}

Error print_args_call (const Node* node, int num_var, Function* func, FILE* file)
{
    if (!node)
        RETURN_ERROR(CORRECT, "");

    Error error = {};

    fprintf (file, "double noundef %%%d", num_var);

    if (node->right)
    {
        fprintf (file, ", ");
        error = print_args_call (node->right, num_var + 1, func, file);
        PARSE_ERROR_WITHOUT_TREE(error);
    }

    RETURN_ERROR(CORRECT, "");
}

Error print_if (const Node* node, IfWhileId* if_while_id, bool* is_print, int* alloc_vars, Function* func, FILE* file)
{
    if ((int) node->value != IF)
    {
        *is_print = false;
        RETURN_ERROR(CORRECT, "");
    }

    *is_print = true;
    Error error = {};

    int var1 = 0;
    error = print_expression (node->left->left, func, &var1, file);
    PARSE_ERROR_WITHOUT_TREE(error);

    int var2 = 0;
    error = print_expression (node->left->right, func, &var2, file);
    PARSE_ERROR_WITHOUT_TREE(error);

    print_load_reg_reg (file, func->curr_var_id, var1);
    var1 = func->curr_var_id;
    func->curr_var_id++;
    print_load_reg_reg (file, func->curr_var_id, var2);
    var2 = func->curr_var_id;
    func->curr_var_id++;

    fprintf (file, "%%%d = fcmp ", func->curr_var_id);
    func->curr_var_id++;

    for (size_t i = 0; i < NUM_OPERS; i++)
        if ((int) node->left->value == OPERATORS[i].id)
            fprintf (file, "%s double %%%d, %%%d\n", OPERATORS[i].name_to_print_asm, var1, var2);

    int num_if = if_while_id->num_if;
    if_while_id->num_if++;
    fprintf (file, "br i1 %%%d, label %%if%d, label %%end_if%d\n", func->curr_var_id - 1, num_if, num_if);

    fprintf (file, "if%d:\n", num_if);

    error = print_commands (node->right, if_while_id, alloc_vars, func, file);
    PARSE_ERROR_WITHOUT_TREE(error);

    fprintf (file, "br label %%end_if%d\n", num_if);
    fprintf (file, "end_if%d:\n", num_if);
    RETURN_ERROR(CORRECT, "");
}

Error print_while (const Node* node, IfWhileId* if_while_id, bool* is_print, int* alloc_vars, Function* func, FILE* file)
{
    if ((int) node->value != WHILE)
    {
        *is_print = false;
        RETURN_ERROR(CORRECT, "");
    }

    *is_print = true;
    Error error = {};
    int num_while = if_while_id->num_while;
    if_while_id->num_while++;

    fprintf (file, "br label %%while%d\n", num_while);
    fprintf (file, "while%d:\n", num_while);

    int var1 = 0;
    error = print_expression (node->left->left, func, &var1, file);
    PARSE_ERROR_WITHOUT_TREE(error);

    int var2 = 0;
    error = print_expression (node->left->right, func, &var2, file);
    PARSE_ERROR_WITHOUT_TREE(error);

    print_load_reg_reg (file, func->curr_var_id, var1);
    var1 = func->curr_var_id;
    func->curr_var_id++;
    print_load_reg_reg (file, func->curr_var_id, var2);
    var2 = func->curr_var_id;
    func->curr_var_id++;

    fprintf (file, "%%%d = fcmp ", func->curr_var_id);
    func->curr_var_id++;

    for (size_t i = 0; i < NUM_OPERS; i++)
        if ((int) node->left->value == OPERATORS[i].id)
            fprintf (file, "%s double %%%d, %%%d\n", OPERATORS[i].name_to_print_asm, var1, var2);

    fprintf (file, "br i1 %%%d, label %%body_while%d, label %%end_while%d\n", func->curr_var_id - 1, num_while, num_while);
    fprintf (file, "body_while%d:\n", num_while);

    error = print_commands (node->right, if_while_id, alloc_vars, func, file);
    PARSE_ERROR_WITHOUT_TREE(error);

    fprintf (file, "br label %%while%d\n", num_while);
    fprintf (file, "end_while%d:\n", num_while);
    RETURN_ERROR(CORRECT, "");
}

void print_store_num_reg (FILE* file, double num, int reg)
{
    fprintf (file, "store double 0x%zX, double* %%%d, align 8\n", (*(uint64_t*) &num), reg);
}

void print_load_reg_reg (FILE* file, int reg1, int reg2)
{
    fprintf (file, "%%%d = load double, double* %%%d, align 8\n", reg1, reg2);
}

void print_load_reg_var (FILE* file, int reg, const char* var)
{
    fprintf (file, "%%%d = load double, double* %%%sp, align 8\n", reg, var);
}

void print_store_reg_reg (FILE* file, int reg1, int reg2)
{
    fprintf (file, "store double %%%d, double* %%%d, align 8\n", reg1, reg2);
}

void print_store_reg_var (FILE* file, int reg, const char* var)
{
    fprintf (file, "store double %%%d, double* %%%sp, align 8\n", reg, var);
}

void print_store_var_var (FILE* file, const char* var1, const char* var2)
{
    fprintf (file, "store double %%%s, double* %%%sp, align 8\n", var1, var2);
}

void print_alloca_reg (FILE* file, int reg)
{
    fprintf (file, "%%%d = alloca double, align 8\n", reg);
}

void print_alloca_var (FILE* file, const char* var)
{
    fprintf (file, "%%%sp = alloca double, align 8\n", var);
}

int get_num_var (const char* name)
{
    int num = 0;
    sscanf (name + 3, "%d", &num);
    return num;
}