# C minus minus

A compiler for a C-like programming language, with a reduced set of instructions.

## Table of Contents

- [Summary](#summary)
- [Motivation and Problem to Solve](#motivation-and-problem-to-solve)
- [Project Objectives](#project-objectives)
- [State of the Art](#state-of-the-art)
- [Compiler Architecture and Design](#compiler-architecture-and-design)
  - [Block Diagram](#block-diagram)
  - [Data Flow Explanation](#data-flow-explanation)
  - [Design Decisions](#design-decisions)
- [Lexical Analysis](#lexical-analysis)
  - [Automata](#automata)
- [Syntax Analysis](#syntax-analysis)
  - [Parser](#parser-implementation)
- [Semantic Analysis](#semantic-analysis)
- [Installation and Running](#installation-and-running)
- [Testing and Validation](#testing-and-validation)
- [Tools and Development Environment](#tools-and-development-environment)
- [Demonstration](#demonstration)
- [Challenges and Solutions](#challenges-and-solutions)
- [Conclusions and Future Work](#conclusions-and-future-work)
- [References](#references)

## Summary

This project aims to develop a complete compiler, covering everything from lexical analysis to final code generation. The compiler will allow for the translation of source code written in a C-- into machine code.

## Motivation and Problem to Solve

- **Problem Description:** We aim to expand our knowledge by creating a compiler, as building one is a powerful way to gain a deep understanding of how compilers work. And what better way to do this than by creating a compiler for a C-like language?
- **Importance:** An efficient compiler is essential for any development environment, as it directly impacts developer productivity and the quality of the final product. Moreover, having a custom compiler allows for greater customization and optimization for specific use cases.
- **Use Cases:** This compiler will be useful in both academic and industrial settings, enabling the compilation of C++ code and its adaptation to various architectures and platforms. It will also serve as a teaching and experimentation tool in compiler courses.

## Project Objectives

- Develop a lexical analyzer that correctly identifies and classifies the lexical units of the input language.
  -Implement a syntax analyzer that constructs abstract syntax trees (AST) from the tokens generated by the lexical analyzer.
- Create a semantic analyzer that checks the coherence and correctness of the source code in terms of data types, variable scope, and language rules.
- Design a code generator that translates the AST into machine code or an intermediate code optimized for efficient execution.
- Include additional optimizations in the code generation to improve the efficiency of the compiled code.
- Develop an error handler that identifies, reports, and suggests solutions to lexical, syntactic, and semantic errors.

## State of the art

- **Similar Compilers:** GCC, Clang, and other compilers that support C/C++ and provide advanced analysis, code optimization, and machine code generation.
- **Limitations of Current Solutions:** Many current compilers can be difficult to adapt or customize for specific needs. They can also be complex and hard to learn for those just starting in compiler development.
- **Justification for the New Compiler:** This project aims to create a modular and extensible compiler that is not only efficient but also easy to understand and modify. It focuses on a structure that allows for the incorporation of new features and optimizations without needing to restructure the entire compiler.

## Compiler Architecture and Design

- **Block Diagram:**
  ![image](https://github.com/user-attachments/assets/f55bb4f8-9be9-4076-a452-e2bbc98ba996)
  (Figure 1.1, Cooper & Torczon, 2011, p. 9)

- **Data Flow Explanation:**

  1. **Front End:**
     The front end of the compiler is responsible for analyzing the source code and converting it into an intermediate form.

     - **Scanner:**  
       The scanner (also known as the lexical analyzer) scans the source code and divides it into tokens (smallest units like keywords, identifiers, and operators). It also removes any unnecessary whitespaces or comments.

     - **Parser:**  
       The parser checks the syntax of the token stream produced by the scanner. It builds a **parse tree** or **abstract syntax tree (AST)** based on the grammar of the source language. This ensures the code follows the proper syntax rules.

     - **Elaboration:**  
       This phase performs semantic checks (such as type checking) to ensure that the parsed structure makes sense in terms of meaning. It verifies correct variable declarations, function calls, and ensures that expressions are logically valid. The elaboration phase is critical for ensuring that the code is semantically correct before it proceeds to optimization.

  2. **Optimizer:**

     - The optimizer takes the intermediate representation from the front end and attempts to improve the code for efficiency without changing its functionality.

  3. **Back End:**

     - The back end is responsible for generating the target machine code from the optimized intermediate representation.

     - **Instruction Selection:**  
       In this phase, the compiler selects appropriate machine instructions to implement the operations in the intermediate code. This step is specific to the target architecture.

     - **Instruction Scheduling:**  
       This phase reorders the machine instructions to improve performance by avoiding pipeline stalls and ensuring better use of the processor's execution units.

     - **Register Allocation:**  
       In this phase, variables and temporary values are assigned to the limited number of CPU registers. The register allocator may also insert instructions to load and store values from memory when the registers are full.

  4. **Infrastructure:**
     - **Infrastructure:**  
       This layer supports the entire compilation process by providing necessary data structures, algorithms, and management of symbol tables, error handling, and other utilities. The infrastructure ensures that each phase has access to shared resources like intermediate representations and data structures.

- **Design Decisions:**

  1. **Modular Architecture:**

     - The architecture is divided into three key phases—**Front End**, **Optimizer**, and **Back End**—making the design modular and allowing for the independent development and optimization and testing of each phase. This also allows flexibility for porting the compiler to different architectures by adjusting only the back end.

  2. **Separation of Concerns:**

     - The **Front End** focuses on source code analysis and validation, the **Optimizer** improves the performance, and the **Back End** generates machine-specific code. This clear division of responsibilities simplifies maintenance and allows for enhancements without affecting other stages.

  3. **Target-specific Code Generation:**

     - The **Back End** is highly architecture-specific, especially during **Instruction Selection**, **Instruction Scheduling**, and **Register Allocation**, allowing the compiler to produce efficient machine code tailored to the target hardware.

  4. **Infrastructure Support:**
     - A robust **Infrastructure** layer supports all phases of the compiler. It provides necessary services like memory management, symbol table handling, and error reporting, enabling smooth communication and resource sharing among different phases.

## Lexical Analysis

### Lexical Analysis

- Tokenization, identification of keywords, operators, etc.
- Keywords, tokens, identifiers, etc., are the same as those in the C language.

### Automata

Both of these DFAs are the same but were broken down into two images for better viewing clarity.

![Automata Compilador identifier, keyword](https://github.com/user-attachments/assets/695c0dce-311f-41f3-afb7-6a2bc6482d45)
![Automata Compilador operator, constant, punctiation](https://github.com/user-attachments/assets/335a3b7c-0970-485b-887f-e8e05a3d3649)

We implemented a **table-driven lexer** based on the automata diagram. This approach allows for a flexible and generalized solution. The table-driven lexer was designed manually (not generated by any tool), ensuring that it meets the specific requirements and complexities of the language being compiled.

A table-driven lexer provides several advantages over a directly coded lexer:

- **Modularity and Flexibility:** Since the lexer’s logic is abstracted into tables (often a state-transition table), the DFA can be easily modified or extended by changing the table entries without altering the core logic of the lexer.
- **Maintainability:** The table-driven approach simplifies maintenance. If new keywords, tokens, or operators need to be added, it can be done by updating the relevant table entries.
- **Efficiency:** This method can be computationally efficient as it avoids large, nested conditional structures (such as if-else or switch cases) by making use of direct table lookups.

To further enhance our development process, we later migrated the lexer to **Flex**, a powerful lexical analyzer generator that simplifies the process of defining and managing the lexer. Flex allows for easier modifications and faster updates since it provides a more abstracted way of writing lexer rules. With Flex, the need for manual table management is reduced, and changes can be implemented by simply updating the lexer definition file. This streamlines the addition of new keywords, operators, or tokens, thus improving development speed and reducing the likelihood of errors.

For instructions on how to install Flex and set up your environment, refer to the section [Installation and Running](#installation-and-running).
You can copy and paste this code into your Markdown editor or file. If you need further assistance,


## Syntax Analysis

### Overview
The syntax analysis phase ensures that the source code conforms to the grammatical structure of the language. This phase utilizes a context-free grammar (CFG) to parse the code and build a parse tree or an abstract syntax tree (AST).

### CFG Analysis
The structure of the language was analyzed, and the findings are documented in the [CFG Analysis](docs/CFG-Analysis.md). This document outlines the original grammar and explains the reasoning behind each rule.

### Grammar Development
- **Original CFG**: The initial set of grammar rules, representing the core language structure, is available in the [CFG](docs/CFG.md) file. Each rule is accompanied by a brief explanation to provide insight into its design.
- **Elimination of Left Recursion**: Left recursion was removed to align the grammar with the requirements for an LL(1) parser. The modified version is documented in the [CFG-LR](docs/CFG-LR.md) file.
- **Left Factoring**: To handle ambiguity and create a deterministic parsing structure, left factoring was applied to the grammar rules. The final version of the grammar can be found in the [CFG-Left-Factoring](docs/CFG-left-factoring.md) file.
- **First and Follow Analysis**: We also performed first and follow analysis to ensure compatibility with the LL(1) parsing strategy. The details of this analysis are documented in the [First-and-Follow](docs/First-and-Follow.md) file.

### Visual Representation
To aid comprehension, a syntactic diagram was created to illustrate the final version of the grammar. You can view this diagram [here](https://htmlpreview.github.io/?https://github.com/diegolorenzo12/Compiler/blob/0f19faf2cd49f8ebea099f548f1f6bea33e9ee84/docs/Syntactic-diagram.html).

### Parser Implementation
The parser was implemented as a **recursive descent parser**, designed to conform to the LL(1) parsing strategy. Some modifications were made to the original C syntax to enable compatibility with LL(1):
- **Function Declarations**: Prefixed with `func` to simplify parsing.
- **Type Qualifiers and Struct Pointers**: Certain features, such as multiple type qualifiers and struct pointers, were excluded to reduce complexity and ambiguity.

These adjustments helped streamline the grammar, making it more suitable for top-down parsing while preserving essential language functionality. 

## Semantic Analysis

### Overview
The semantic analyzer handles the classification of composite data types (structs, pointers, primitives, etc.). A symbol table was designed where each symbol is associated with its type and value. This symbol table uses a hash table with chained scopes, facilitating the creation and deletion of scopes.  

The Visitor pattern decouples the AST from the semantic analyzer, allowing efficient traversal of the tree and performing semantic analysis.

### Features and Examples

#### 1. Variables Out of Scope
The analyzer ensures variables are accessed only within their valid scope.

```c
func int calculate_sum(int a, int b) {
    return a + b;
}

func int main() {
    int x = 5;
    {
        int y = 10;
        int result = calculate_sum(x, y); // Valid
    }
    int result = calculate_sum(x, y); // Error: 'y' is out of scope
    return 0;
}
```

#### 2. Type Checking in Assignments
Assignments are checked to ensure type compatibility and that the Lvalue is not constant.

```c
func int double_value(int a) {
    return 2 * a;
}

func int main() {
    1 = 10; // Error: Cannot assign to a constant Lvalue

    char str[10] = "hello";
    int doubled = double_value(str); // Error: Implicit type conversion from 'char[]' to 'int'
    return 0;
}
```

#### 3. Data Type Compatibility in Expressions
The analyzer ensures expressions involve compatible types.

```c
func int main() {
    int a = 5;
    a = (1>3) + 4; // bool + int
    return 0;
}
```

#### 4. Return Type Consistency
The return type of a function must match the declared type.

```c
func int square(int n) {
    return n * n;
}

func void print_value(int n) {
    return n; // Error: Return type 'void' cannot return a value
}

func int main() {
    int result = square(4); // Valid
    return 0;
}
```

#### 5. Function Signature Matching
Function calls must match the declared function signature.

```c
func int multiply(int a, int b) {
    return a * b;
}

func int main() {
    int product = multiply(3, 4); // Valid
    product = multiply(3);       // Error: Incorrect number of arguments
    product = multiply(2, "hi"); // Error: Incorrect Type
    return 0;
}
```

#### 6. Struct Member Access
The analyzer ensures struct members exist and match their types.

```c
struct Point {
    int x;
    int y;
};

func int calculate_distance(struct Point p) {
    return p.x * p.x + p.y * p.y;
}

func int main() {
    struct Point p;
    p.x=3;
    p.y=4;
    int distance = calculate_distance(p); // Valid
    int error = p.z; // Error: 'z' is not a member of 'Point'
    return 0;
}
```

#### 7. Pointer Arrow (->) Operator Usage
Pointers must use the arrow operator correctly when accessing members of a struct.

```c
struct Node {
    int data;
    struct Node *next;
};

func int get_data(struct Node *node) {
    return node->data;
}

func int main() {
    struct Node *ptr;
    int value = get_data(ptr); // Valid
    return 0;
}
```

#### 8. Break and Continue Statement Validation
The analyzer checks that break and continue statements are used only inside loops or switch statements.

```c
func void loop_example() {
    while (1) {
        break; // Valid
    }
}

func int main() {
    if (1) {
        break; // Error: 'break' outside of a loop or switch
    }
    loop_example();
    return 0;
}
```

#### 9. Array Initialization with Brace {} Matching Dimensions
The initializer must match the declared array dimensions.

```c

func int main() {
    int arr[3] = {1, 2, 3}; // Valid
    return 0;
}
```

#### 10. Constant Types in Case Statements
case labels must be constant expressions.

```c
func int get_status(int value) {
    switch (value) {
        case 1: // Valid
            return 0;
        case value + 1: // Error: 'value + 1' is not a constant expression
            return -1;
    }
    return 1;
}

func int main() {
    int result = get_status(1);
    return 0;
}
```

## Installation and Running

### Prerequisites

If you don't have Conan installed, run the following:

```bash
pip install conan
conan profile detect
```

### Install Conan packages

From the root of the project directory, run the following command to install Conan packages:

```bash
conan install . --build=missing --output-folder=build -s build_type=Debug
```

### Build the Project

To build the project, run the following:

```bash
cd build/
cmake ..
cmake --build .
```

> [!NOTE]  
> If you see the error "FLEX_EXECUTABLE not found." when running cmake, try setting the LEX enviroment variable by running the generated .bat, .sh or manually setting it from the path conan created. 

### Build and Run the Project

To build and run the project, execute:

```bash
cd build/
cmake --build . --target run
```

## Testing and Validation

### Testing Methodology
To ensure the reliability and correctness of the compiler, we adopted a rigorous testing methodology using **Google Test (GTest)**. Testing is organized into three main categories, corresponding to key stages of the compiler's frontend:

1. **Lexer Tests**  
   - Verifies tokenization of valid code.  
   - Ensures that invalid input correctly raises lexer errors.

2. **Parser Tests**  
   - Confirms syntactically valid code generates the correct parse tree.  
   - Validates that syntactic errors are detected and reported accurately.

3. **Semantic Tests**  
   - Checks that semantically valid code passes without errors.  
   - Detects and reports semantic issues like type mismatches, undeclared variables, and invalid operations.

Each test case evaluates whether errors are correctly identified and ensures the compiler progresses smoothly to the next phase when no errors are present.

### Results Obtained
The testing framework successfully validated the following:

- **Error Handling**  
  - Lexer errors are detected and reported with specific error messages.  
  - Parser errors (e.g., unmatched brackets or invalid syntax) are identified and halt further compilation.  
  - Semantic errors (e.g., undeclared variables or type mismatch) are properly flagged.

- **Compilation Success**  
  - Valid code compiles successfully through each stage, up to the tested phase.  
  - No crashes or unexpected behavior occur when processing valid input.

The tests confirmed the expected behavior for all scenarios and ensured stable and predictable compiler operation.

![image](https://github.com/user-attachments/assets/b25eb52c-78cd-4493-8cf6-35048028f654)


### Specific Test Cases
Specific test cases can be found in:
- [tests/lexer_test.cpp](tests/lexer_test.cpp)
- [tests/parser_test.cpp](tests/parser_test.cpp)
- [tests/semantic_test.cpp](tests/semantic_test.cpp)

### Running tests
Use cmake to compile the project and specifically target all the components, including the tests. Then use the ctest command to run the tests. 
```bash
cd build/
cmake --build . --target all
ctest
```

## Tools and Development Environment

- **Programming languages used:** C++
- **Development tools:** CMake, Conan for package management

For installation instructions, see the [Installation and Running](#installation-and-running) section.

## Demonstration
### Source code example:
```cpp
func int add(int a, int b) {
    int result = b;
    result += a;
    return 0;
}

struct myStruct {
    int x[10];
    int y;
};


func float prueba() {   
    struct myStruct * point;
    point->x[1] = 1;
    point->y = 5;

    struct myStruct point2;
    point2.x[1] = 1;
    point2.y = 5;
    {
        int point; //on another scope 
        point = 1;
        return 0.0;
    }

    struct Anonimo {
        int point[10];
        int test;
    };
    Anonimo.test = 2;
}

func int main() {
    int x = 10, y = 20; // one line with multiple declaration
    int arr[2][2] = {
        {1, 2},
        {1, 2}
    };  // brace initializer
    
    char string[10] = "string";

    switch (x) {
        int newScope = 10;
        case 1: {
            x++;
            break;
        }
        case 2:
            break;
        default: {
            y++;
            newScope--;
            break;
        }
    }

    int result = 1 + 2;

    {

    }

    // Add two numbers
    result = add(x, y);
    
    // Conditional statement
    if (result = 1) {
    }

    int n = 10;

    for (int i = 0; n; i++) {
        i = 10;
    }

    do {
    } while (1);

    while (1) {
        continue;
        break;
    }
    return 0;
}
```


### Compilation process:
The process begins when the source code is passed to the lexer, which breaks the raw code into tokens—meaningful units like keywords, identifiers, and operators. The lexer uses regular expressions or finite automata (like DFA) to match patterns and generate a token stream, which is then passed to the syntactic analyzer (parser).

The syntactic analyzer processes the token stream based on the grammar rules of the language, checking if the sequence of tokens forms valid structures (expressions, statements, etc.). It constructs a syntax tree or Abstract Syntax Tree (AST) representing the structure of the program. If there are any syntax errors, such as missing parentheses or incorrect function calls, the parser catches them and reports the issue.

Next, the semantic analyzer examines the AST for logical consistency. This includes checking type correctness (e.g., adding an integer to a string), variable scope, and the validity of operations based on the data types. If there are semantic errors (such as undeclared variables or type mismatches), the semantic analyzer identifies and reports them.

If the code passes through all these stages without errors, the compilation is considered successful. However, if any errors are detected during lexing, parsing, or semantic analysis, the respective analyzer reports the error and halts further compilation, allowing the developer to correct the issues before retrying the process.
### Execution of the compiled code:
![CompilationSuccess](https://github.com/user-attachments/assets/35313feb-a88d-42b3-b6d7-c0eb67129165)

## Challenges and Solutions

### Technical or design problems:
- Implementing a manual lexical analyzer using DFA transition tables was challenging due to the number of errors caused by the inefficient handling of ambiguous cases, such as keywords resembling identifiers.
- Eliminating left recursion and factoring common prefixes in the grammar was complex and led to inconsistencies in the LL(1) top-down parsing process.

### Strategies adopted to overcome challenges:
- The project transitioned to Flex to simplify the process and enable greater abstraction in defining lexical rules.
- Changes to the grammar were carefully documented, supported by syntax diagrams and First and Follow analysis.
- An LL(1) parser was developed using a recursive descent approach to construct the Abstract Syntax Tree (AST).
- A semantic analyzer was implemented to perform consistency checks between variable declarations and usage, ensuring operations are valid according to data types.
- Error handling was improved across all phases (lexical, syntactic, and semantic).


### Lessons learned:
- Modularity: Designing a compiler with a modular architecture allows each phase to be developed and tested independently.
- Continuous Documentation: Recording every design decision and maintaining a change log facilitates understanding and collaboration in complex projects.
- Iterative Testing: Conducting continuous testing at each phase helped identify errors early, saving time in later stages of development.

## Conclusions and Future Work
This project successfully developed a functional compiler for C--, covering the initial phases of the compilation process, from lexical analysis to semantic analysis. Although intermediate code generation was not implemented, the modular approach laid a solid foundation for future extensions and potential adaptation to various architectures and platforms.

Throughout development, several technical challenges related to grammar and error handling were overcome by implementing practical solutions such as utilizing specialized tools (Flex) and simplifying grammar design. Semantic analysis validated the consistency of the source code in areas such as type handling and variable scope.

The result is an extensible and educationally suitable compiler that provides a robust foundation for expanding its capabilities in the future. Future stages could include implementing intermediate code generation, code optimization, and support for advanced language features.

## References

- Cooper, K., & Torczon, L. (2011). Engineering a compiler (2nd ed.). Morgan Kaufmann.

- Kernighan, B. W., & Ritchie, D. M. (1988). The C programming language (2nd ed.). Prentice Hall.
