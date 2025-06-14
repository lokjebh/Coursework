/******************************************************************************
 *                                                                            *
 * minijvm.c                                                                  *
 *                                                                            *
 * Computer Science 2211b - winter 2024                                       *
 * Assignment 5                                                               *
 * Student: Jebsen Hui                                                        *
 *                                                                            *
 * This program implements part of a Java Virtual Machine (JVM) in C to read  *
 * .class files into memory and execute them                                  *
 *                                                                            *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "minijvm.h"
#include "stack.h"
#include <string.h>
#include <sys/stat.h>

/*Help functions*/
void test_stack(stack s);
// psuh 0 onto the operand stack
// void iconst0(stack s);
// Pop the top item off the stack (discord it)
void pop(minijvm* jvm);
// Duplicate the top item on the stack
void dup(minijvm* jvm);
// Pop and add the top two elements on the operand stack, push the result
void iadd(minijvm* jvm);
// Pop and substract the top element from second top element of the stack, push the result
void isub(minijvm* jvm);
// Pop and multiply the top two elements on the operand stack, push the result
void imul(minijvm* jvm);
// Pop the top two items and perform division (second from top) / (top), push the result
void idiv(minijvm* jvm);
// Pop the top two items and perform division (second from top) % (top), push the remainder back on stack
void irem(minijvm* jvm);
// Arithmetic shift right
void ishr(minijvm* jvm);
// void operation(minijvm* jvm, unsigned char operation);
// Print the top item of the stack
void iprint(stack s);
// Push local variable at index n onto the stack
void iload(minijvm* jvm, unsigned int n);
// Pop the top of the stack and store it in local variable at index n
void istore(minijvm*, unsigned int n);
// Increment local variable at index n by d
void iinc(minijvm* jvm);
// Pop the top item, branch to offset if it is equal to zero
char* ifeq(minijvm* jvm);
// Branch to offset
char* branch_to(minijvm* jvm);

char* jvm_read(const char* filename)
{
    FILE* fp = NULL; // Initailizing a file pointer
    char* bytecode = NULL; // Initializing a pointer points to bytecode array

    // Compute the target filename
    size_t len_filename = strlen(filename) + strlen(CLASS_EXT) + 1;
    char file_path[len_filename];

    strcpy(file_path, filename);
    strcat(file_path, CLASS_EXT);

    // read the file containing bytecodes, if the file doesn't exit, print an error and exit with EXIT_FAILURE
    struct stat st; // trying to declare the stat structure

    if (stat(file_path, &st) != 0)
    {
        fprintf(stderr, "File ''%s' not found\n", file_path);
        exit(EXIT_FAILURE);
    }

    // dynamically allocate an array of bytes to store the bytecode
    bytecode = malloc(MAX_CLASS_SIZE);

    // use stat function to determine the file size
    long file_size = st.st_size;
    // open the specified file
    fp = fopen(file_path, "rb");
    if (fp == NULL)
    {
        free(bytecode);
        exit(EXIT_FAILURE);
    }

    // read byte-by-byte into the array until EOF is reach
    size_t nmemb = fread(bytecode, sizeof(bytecode[0]), file_size, fp);

    // if number of element read is not file_size, return null
    if (nmemb != file_size)
    {
        free(bytecode);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    return bytecode;

}

void jvm_run(minijvm* jvm)
{
    while (true)
    {
        if (jvm->return_value == 1)
        {
            exit(1);
        }
        // Get next bytecode b pointed to by the program counter
        // (interpret it as an unsigned char since bytecode instructions are
        // unsigned -- that is cast it)
        unsigned char b = (unsigned char)*(jvm->pc);

        switch (b)
        {
            case INST_ICONST0:
                // psuh 0 onto the operand stack
                stack_push(jvm->operands, 0);
                break;

            case INST_POP:
                // Pop the top item off the stack (discard it)
                pop(jvm);
                break;

            case INST_DUP: dup(jvm);
                           break;

            case INST_IADD:
                // Pop and add the top two items on the stack, push the result
                iadd(jvm);
                break;

            case INST_ISUB:
                // Pop and subtract the top and second top items on the stack, push the result
                isub(jvm);
                break;

            case INST_IMUL:
                // Pop and multiply the top two items on the stack, push the result
                imul(jvm);
                break;

            case INST_IDIV:
                // idiv (108)
                idiv(jvm);
                break;

            case INST_IREM:
                // Pop the top two items and compute the remainder of integer divsion
                irem(jvm);
                break;

            case INST_ISHR:
                ishr(jvm);
                break;

            case INST_BIPUSH:
                // take a single operand in the bytecode
                signed int byte = (signed int)*(jvm->pc + 1);
                // Push signed byte onto the stack
                stack_push(jvm->operands, byte);
                jvm->pc++;
                break;

            case INST_RETURN:
                // Stop execution and return to operating system
                return;

            case INST_PRINT:
                // Print the top item on the stack
                iprint(jvm->operands);
                break;

            case INST_ILOAD:
                iload(jvm, (unsigned int)*(jvm->pc + 1));
                jvm->pc++;
                break;

            case INST_ISTORE:
                istore(jvm, (unsigned int)*(jvm->pc + 1));
                jvm->pc++;
                break;

            case INST_IINC:
                iinc(jvm);
                jvm->pc = jvm->pc + 2;
                break;

            case INST_IFEQ:
                jvm->pc = ifeq(jvm);
                break;

            case INST_GOTO:
                jvm->pc = branch_to(jvm);
                break;

        }
        
        // Update the program counter to point at the next bytecode
        jvm->pc++;
    }

}

/* Initializes a new minijvm struct 
 * Parameters:
 *     const char* filename
 * Return:
 *     A pointer to the initialized minijvm
 */
minijvm* jvm_init(const char* filename)
{
    // dynamically allocate a new minijvm struct
    minijvm* mjvm = malloc( sizeof(minijvm) );

    // Read the file and store the bytecode array in the struct
    mjvm->bytecode = jvm_read(filename);

    // Set the program counter to point at the first bytecode in the array
    mjvm->pc = mjvm->bytecode;

    // Create the operand stack
    mjvm->operands = stack_create();

    // Initializing mjvm.locals
    for (int i = 0; i < 10; i++)
    {
        mjvm->locals[i] = 0;
    }

    // Initializing mjvm.return_value
    mjvm->return_value = 0;

    return mjvm;

}

void jvm_free(minijvm* jvm)
{
    if (jvm != NULL)
    {
        free(jvm->bytecode);
        free(jvm);
    }
}

void jvm_usage()
{
    fprintf(stderr, "Usage: mjvm FILENAME\n");
    exit(1);
}

int main(int argc, char** argv)
{
    // if an incorrect number of arguments is passeed, call usage
    if (argc < 2 || argc > 2)
    {
        jvm_usage();
    }

    // Call functions to initialize MiniJVM
    minijvm* MiniJVM = jvm_init(argv[1]);

    // run the program
    jvm_run(MiniJVM);


    // free dynamically allocated memory
    jvm_free(MiniJVM);

    return 0;

}

void pop(minijvm* jvm)
{
    item_type value;
    if (stack_pop(jvm->operands, &value))
    {
        return;
    }
    else
    {
        jvm->return_value = 1;
        return;
    }
}

void dup(minijvm* jvm)
{
    item_type value;
    if (stack_peek(jvm->operands, &value))
    {
        stack_push(jvm->operands, value);
    }
    else
    {
        jvm->return_value = 1;
        return;
    }
}


void iadd(minijvm* jvm)
{
    signed int top, second_top;
    if (stack_pop(jvm->operands, &top) && stack_pop(jvm->operands, &second_top))
    {
        stack_push(jvm->operands, second_top + top);
    }
    else
    {
        jvm->return_value = 1;
        return;
    }
}

void isub(minijvm* jvm)
{
    signed int top, second_top;
    if (stack_pop(jvm->operands, &top) && stack_pop(jvm->operands, &second_top))
    {
        stack_push(jvm->operands, second_top - top);
    }
    else
    {
        jvm->return_value = 1;
        return;
    }
}

void imul(minijvm* jvm)
{
    signed int top, second_top;
    if (stack_pop(jvm->operands, &top) && stack_pop(jvm->operands, &second_top))
    {
        stack_push(jvm->operands, second_top * top);
    }
    else
    {
        jvm->return_value = 1;
        return;
    }
}

void idiv(minijvm* jvm)
{
    signed int top, second_top;
    if (stack_pop(jvm->operands, &top) && stack_pop(jvm->operands, &second_top))
    {
        if (top == 0)
        {
            fprintf(stderr, "Divsion by zero\n");
            jvm->return_value = 1;
            return;
        }
        stack_push(jvm->operands, second_top / top);
    }
    else
    {
        jvm->return_value = 1;
        return;
    }
}

void irem(minijvm* jvm)
{
    signed int top, second_top;
    if (stack_pop(jvm->operands, &top) && stack_pop(jvm->operands, &second_top))
    {
        if (top == 0)
        {
            fprintf(stderr, "Division by zero\n");
            jvm->return_value = 1;
            return;
        }
        else
        {
            stack_push(jvm->operands, second_top % top);
        }
    }
    else
    {
        jvm->return_value = 1;
        return;
    }
}

void ishr(minijvm* jvm)
{
    signed int top, second_top;
    if (stack_pop(jvm->operands, &top) && stack_pop(jvm->operands, &second_top))
    {
        stack_push(jvm->operands, second_top >> top);
    }
    else
    {
        jvm->return_value = 1;
        return;
    }
}



void iprint(stack s)
{
    signed int top;
    if (stack_peek(s, &top))
    {
        printf("%d\n", top);
    }
}

void iload(minijvm* jvm, unsigned int n)
{
    if (n >= 0 && n < 10)
    {
        signed int local_variable = jvm->locals[n];
        stack_push(jvm->operands, local_variable);
    }
    else
    {
        jvm->return_value = 1;
        return;
    }
}


void istore(minijvm* jvm, unsigned int n)
{
    signed int top;
    if (stack_pop(jvm->operands, &top))
    {
        if (n >= 0 && n < 10)
        {
            jvm->locals[n] = top;
        }
        else
        {
            jvm->return_value = 1;
            return;
        }
    }
    else
    {
        jvm->return_value = 1;
        return;
    }
}

void iinc(minijvm* jvm)
{
    unsigned int n = (unsigned int)*(jvm->pc + 1);
    signed int d = (signed int)*(jvm->pc + 2);
    jvm->locals[n] = jvm->locals[n] + d;
}

char* ifeq(minijvm* jvm)
{
    unsigned int byte;
    signed int top, offset;
    char* pc;

    if (stack_pop(jvm->operands, &top))
    {
        if (top == 0)
        {
            byte = ((unsigned int)*(jvm->pc + 1) << 8) | (unsigned int)*(jvm->pc + 2);
            offset = (signed int)byte;
            pc = jvm->pc + offset - 1;
        }
        else
        {
            pc = jvm->pc + 2;
        }
    }
    else
    {
        jvm->return_value = 1;
        pc = jvm->pc + 2;
    }
    return pc;
}

char* branch_to(minijvm* jvm)
{
    unsigned int byte;
    signed int offset;
    char* pc;
    
    byte = ((unsigned int)*(jvm->pc + 1) << 8) | (unsigned int)*(jvm->pc + 2);
    offset = (signed int)byte;
    pc = jvm->pc + offset - 1;

    return pc;
}

