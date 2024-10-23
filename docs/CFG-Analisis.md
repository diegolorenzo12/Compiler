In C, the top program structure follows a set of global declarations, which can be either function definitions or declarations. All statements (logical, conditional, iterative, etc.) necessarily need to be grouped inside a function.


A declaration informs the compiler about the name and type of variables, functions, structures, etc. A declaration consist of several components. Declarations specify the interpretation given to each identifier; they do not necessarily reserve storage
associated with the identifier. Declarations that reserve storage are called definitions.[(Kernighan & Ritchie, 1988)](../README.md#references) :


- [Storage Class Specifier](courses.physics.ucsd.edu/2014/Winter/physics141/Labs/Lab1/The_C_Programming_Language.pdf#page=230)
- [Type Specifier](https://courses.physics.ucsd.edu/2014/Winter/physics141/Labs/Lab1/The_C_Programming_Language.pdf#page=231)
- [Type Qualifier](https://courses.physics.ucsd.edu/2014/Winter/physics141/Labs/Lab1/The_C_Programming_Language.pdf#page=213)
- [Declarator](https://courses.physics.ucsd.edu/2014/Winter/physics141/Labs/Lab1/The_C_Programming_Language.pdf#page=237)
- [Initializer (optional)](https://courses.physics.ucsd.edu/2014/Winter/physics141/Labs/Lab1/The_C_Programming_Language.pdf#page=241)


Example of a declaration:
![declaration](https://github.com/user-attachments/assets/92c4d3db-7881-4b4f-aa39-2c83bb4284c5)


Specifies the storage duration, linkage, and visibility of an identifier.

A type specifier determines the basic type of a variable or function. It specifies what kind of data the variable can hold or the function can return.

A type qualifier modifies the behavior or properties of a type specifier. It adds additional information about how the type should be treated by the compiler, without changing the basic type itself.