# C Programming Structure

In C, the top program structure follows a set of global declarations, which can be either function definitions or declarations. All statements (logical, conditional, iterative, etc.) necessarily need to be grouped inside a function.

A declaration informs the compiler about the name and type of variables, functions, structures, etc. A declaration consists of several components. Declarations specify the interpretation given to each identifier; they do not necessarily reserve storage associated with the identifier. Declarations that reserve storage are called definitions. [(Kernighan & Ritchie, 1988)](../README.md#references):

- [Storage Class Specifier](https://courses.physics.ucsd.edu/2014/Winter/physics141/Labs/Lab1/The_C_Programming_Language.pdf#page=230)
- [Type Specifier](https://courses.physics.ucsd.edu/2014/Winter/physics141/Labs/Lab1/The_C_Programming_Language.pdf#page=231)
- [Type Qualifier](https://courses.physics.ucsd.edu/2014/Winter/physics141/Labs/Lab1/The_C_Programming_Language.pdf#page=213)
- [Declarator](https://courses.physics.ucsd.edu/2014/Winter/physics141/Labs/Lab1/The_C_Programming_Language.pdf#page=237)
- [Initializer (optional)](https://courses.physics.ucsd.edu/2014/Winter/physics141/Labs/Lab1/The_C_Programming_Language.pdf#page=241)

Example of a declaration:  
![declaration](https://github.com/user-attachments/assets/92c4d3db-7881-4b4f-aa39-2c83bb4284c5)

Specifies the storage duration, linkage, and visibility of an identifier.

A type specifier determines the basic type of a variable or function. It specifies what kind of data the variable can hold or the function can return.

A type qualifier modifies the behavior or properties of a type specifier. It adds additional information about how the type should be treated by the compiler, without changing the basic type itself.

## Expressions in C

Expressions in C are more complex than those seen in class, as their hierarchy of operations is more extensive. Below is a detailed description of this hierarchy:

- **Assignment Operators**: `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `^=`, `|=`, `<<=`, `>>=`
- **Ternary Operator**: `?:`
- **Logical Operators**: `&&`, `||`
- **Bitwise Operators**: `&`, `^`, `|`
- **Equality Operators**: `==`, `!=`
- **Comparison Operators**: `<`, `>`, `<=`, `>=`
- **Bit Shift Operators**: `<<`, `>>`
- **Addition and Subtraction Operators**: `+`, `-`
- **Multiplication and Division Operators**: `*`, `/`, `%`
- **Unary Operators**: `++expr`, `--expr`, `+`, `-`, `!`, `~`
- **Postfix Operators**: `expr++`, `expr--`, `()`, `[]`, `.`, `->`

## Statements in C

Statements in C are instructions that tell the compiler what action to perform. These can be classified into several categories based on their function and structure:

- **Declaration Statements**: These statements are used to declare variables and functions, specifying the data type that a variable can store and optionally its initial value.
- **Assignment Statements**: Used to assign a value to a variable using the assignment operator.
- **Control Flow Statements**: These statements alter the normal execution flow of the program and include:
  - **Conditional Statements**: Allow execution of code blocks based on a condition.
  - **Loop Statements**: Allow repeating a block of code while a condition is met.
  - **Exit Statements**: Used to terminate the execution of a program or exit a loop, controlling the flow of execution.
- **Function Call Statements**: Invoke functions defined in the program or in libraries, allowing the execution of specific actions.
- **Composition Statements**: Group multiple statements within a block delimited by braces, organizing the code within control structures.

These statements should be combinable; for example, if I have an `if`, it can be followed by another statement, which can either be a composition `{}` or another statement without needing to put `{}`.
