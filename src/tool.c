#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include <wasmedge/wasmedge.h>

static inline void print_help_message(){
    printf("Usage: tool [version] [run] wasm_path [arguments]\n");
}

static inline void print_version(){
    printf("WasmEdge version: %s\n", WasmEdge_VersionGet());
}

static inline void print_need_module_error(){
    printf("Please specify a wasm module. \n");
}

static inline void print_too_many_args_error(){
    printf("Too many arguments.\n");
}

static inline void print_too_few_args_error(){
    printf("Too few arguments, please specify the operand. \n");
}

static inline void print_parsing_error(){
    printf("Parsing error: both argument needs to be valid 32 bits integers. \n");
}

static int parse_add_args(long long* arg1, long long* arg2, const char* string1, const char* string2){
    /* parse argument string into integer value */
    char* endptr = NULL;
    errno = 0;

    *arg1 = strtol(string1, &endptr, 10);
    /* use errno and endptr to check if the parse is valid and detect overflow*/
    if (errno == ERANGE){
        return -1;
    }else if((*endptr) != 0 || *arg1 > INT32_MAX || *arg1 < INT32_MIN){
        return -1;
    }

    errno = 0;
    *arg2 = strtol(string2, &endptr, 10);
    if (errno == ERANGE){
        return -1;
    }else if((*endptr) != 0 || *arg2 > INT32_MAX || *arg2 < INT32_MIN){
        return -1;
    }
    return 0;
}

void run_wasm_module(const char* obj_path, int add1, int add2){ 
    /* Launch a stand wasm VM, reference from https://wasmedge.org/book/en/sdk/c.html */

    /* Create the configure context and add the WASI support. */
    /* This step is not necessary unless you need WASI support. */
    WasmEdge_ConfigureContext *ConfCxt = WasmEdge_ConfigureCreate();
    WasmEdge_ConfigureAddHostRegistration(ConfCxt, WasmEdge_HostRegistration_Wasi);
    /* The configure and store context to the VM creation can be NULL. */
    WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(ConfCxt, NULL);

    /* The parameters and returns arrays. */
    WasmEdge_Value Params[2] = { WasmEdge_ValueGenI32(add1) , WasmEdge_ValueGenI32(add2)};
    WasmEdge_Value Returns[1];
    /* Function name. */
    WasmEdge_String FuncName = WasmEdge_StringCreateByCString("add");
    /* Run the WASM function from file. */
    WasmEdge_Result Res = WasmEdge_VMRunWasmFromFile(VMCxt, obj_path, FuncName, Params, 2, Returns, 1);

    if (WasmEdge_ResultOK(Res)) {
        printf("Get result: %d\n", WasmEdge_ValueGetI32(Returns[0]));
    } else {
        printf("Error message: %s\n", WasmEdge_ResultGetMessage(Res));
    }

    /* Resources deallocations. */
    WasmEdge_VMDelete(VMCxt);
    WasmEdge_ConfigureDelete(ConfCxt);
    WasmEdge_StringDelete(FuncName);
}

int main(int argc, const char* argv[]) {

    if(argc < 2){
        print_help_message();
        return -1;
    }

    if(argc > 5){
        print_too_many_args_error();
        print_help_message();
    }

    long long add1, add2;

    if(strcmp(argv[1], "version") == 0){
        print_version();
        return 0;
    }else if(strcmp(argv[1], "run") == 0){
        if(argc == 2){
            print_need_module_error();
            return -1;
        }else{
            if(argc < 5){
                print_too_few_args_error();
                return -1;
            }
            if(parse_add_args(&add1, &add2, argv[3], argv[4]) == 0){
                run_wasm_module(argv[2], add1, add2);
            }else{
                print_parsing_error();
            }   
        }
    }else if(argc < 4){
        print_too_few_args_error();
        return -1;
    }else{
        if(parse_add_args(&add1, &add2, argv[2], argv[3]) == 0){
            run_wasm_module(argv[1], add1, add2); 
        }else{
            print_parsing_error();
        }
    }

    return 0;
}