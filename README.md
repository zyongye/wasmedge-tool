# wasmedge-tool: Pretest for LFX-mentorship with WasmEdge

## Introduction
This repository is a implementation for pretest for LFX mentorship with WasmEdge on issue [#2241](https://github.com/WasmEdge/WasmEdge/issues/2241). For more information, please visit [the discussion page](https://github.com/WasmEdge/WasmEdge/discussions/2227). 

## Usage
To try this tool, please make sure you have WasmEdge SDK installed as well as GCC > 9.4. After clone the repo, run cmake commands. 
```
git clone https://github.com/zyongye/wasmedge-tool.git && cd wasmedge-tool
cmake .
cmake --build .
```
Then use version command to check installation 
```
./tool version
```

## Implementation details
The very important of this tool is to 1) check validity of input and 2) launch WasmEdge VM through C APIs. We will disucss each of them separately. 

1. Check input: Based on the number of input arguments, depends there is either too many or too few variables. And check the second argument is `run` or `version`. If `version`, print the version number, else go ahead and get object file name depends on if `run` is present. After that, check if there are exact two argumets after the object file arguments. Convert them into 32 bit integer and give error if unable to convert or overflow occurs.  
2. Launch WasmEdge VM: First create the VM context and corresponding parameters. Here we use the function name `add` to add two numbers. After that we create the VM and get the responses. After we deallocate the resources we created. 

## Sample test
### Version test
![version-test](/content/version-test.png)
### Functionality test
![function-test](/content/function-test.png)

Test without `run`

![function2-test](/content/function2-test.png)

