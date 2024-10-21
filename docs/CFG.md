# Compiler CFG Documentation

This document provides the Context-Free Grammar (CFG) for the language used in the compiler. This only shows the CFG with left recursion. To see the CFG with left recursion removed, refer to the [CFG-LR](CFG-LR.md) file.
## Terminals

1. **`IDENTIFIER`**
2. **`INTEGER_CONSTANT`**
3. **`FLOAT_CONSTANT`**
4. **`STRING_LITERAL`**
5. **`;`**
6. **`,`**
7. **`:`**
8. **`.`**
9. **`->`**
10. **`(`**
11. **`)`**
12. **`{`**
13. **`}`**
14. **`[`**
15. **`]`**
16. **`+`**
17. **`-`**
18. **`*`**
19. **`/`**
20. **`%`**
21. **`!`**
22. **`~`**
23. **`&`**
24. **`|`**
25. **`^`**
26. **`<<`**
27. **`>>`**
28. **`==`**
29. **`!=`**
30. **`<`**
31. **`>`**
32. **`<=`**
33. **`>=`**
34. **`&&`**
35. **`||`**
36. **`=`**
37. **`*=`**
38. **`/=`**
39. **`%=`**
40. **`+=`**
41. **`-=`**
42. **`<<=`**
43. **`>>=`**
44. **`&=`**
45. **`^=`**
46. **`|=`**
47. **`++`**
48. **`--`**
49. **`typedef`**
50. **`static`**
51. **`auto`**
52. **`register`**
53. **`const`**
54. **`restrict`**
55. **`volatile`**
56. **`inline`**
57. **`int`**
58. **`void`**
59. **`char`**
60. **`short`**
61. **`long`**
62. **`float`**
63. **`double`**
64. **`signed`**
65. **`unsigned`**
66. **`bool`**
67. **`struct`**
68. **`if`**
69. **`else`**
70. **`switch`**
71. **`case`**
72. **`default`**
73. **`while`**
74. **`do`**
75. **`for`**
76. **`continue`**
77. **`break`**
78. **`return`**
79. **`sizeof`**

## Non-Terminals

- [PROGRAM](#PROGRAM)
- [GLOBAL_DECLARATIONS](#GLOBAL_DECLARATIONS)
- [DECLARATION_LIST](#declaration_list)
- [DECLARATION](#declaration)
- [DECLARATION_SPECIFIERS](#declaration_specifiers)
- [INIT_DECLARATOR_LIST](#init_declarator_list)
- [INIT_DECLARATOR](#init_declarator)
- [STORAGE_SPECIFIER](#storage_class_specifier)
- [TYPE_QUALIFIER](#type_qualifier)
- [TYPE_QUALIFIER_LIST](#type_qualifier_list)
- [FUNCTION_SPECIFIER](#function_specifier)
- [TYPE_SPECIFIER](#type_specifier)
- [FUNCTION_DEFINITION](#function_definition)
- [STRUCT_SPECIFIER](#struct_or_union_specifier)
- [STRUCT_DECLARATION_LIST](#struct_declaration_list)
- [STRUCT_DECLARATION](#struct_declaration)
- [SPECIFIER_QUALIFIER_LIST](#specifier_qualifier_list)
- [STRUCT_DECLARATOR_LIST](#struct_declarator_list)
- [DECLARATOR](#declarator)
- [DIRECT_DECLARATOR](#direct_declarator)
- [POINTER](#pointer)
- [PARAMETER_LIST](#parameter_list)
- [PARAMETER_DECLARATION](#parameter_declaration)
- [IDENTIFIER_LIST](#identifier_list)
- [INITIALIZER](#initializer)
- [INITIALIZER_LIST](#initializer_list)
- [STATEMENT](#statement)
- [LABELED_STATEMENT](#labeled_statement)
- [BLOCK](#BLOCK)
- [BLOCK_ITEM](#block_item)
- [BLOCK_ITEM_LIST](#block_item_list)
- [EXPRESSION_STATEMENT](#expression_statement)
- [EXPRESSION](#expression)
- [ASSIGNMENT_EXPRESSION](#assignment_expression)
- [SELECTION_STATEMENT](#selection_statement)
- [ITERATION_STATEMENT](#iteration_statement)
- [JUMP_STATEMENT](#jump_statement)
- [ASSIGNMENT_OPERATOR](#assignment_operator)
- [CONDITIONAL_EXPRESSION](#conditional_expression)
- [LOGICAL_OR_EXPRESSION](#logical_or_expression)
- [LOGICAL_AND_EXPRESSION](#logical_and_expression)
- [INCLUSIVE_OR_EXPRESSION](#inclusive_or_expression)
- [EXCLUSIVE_OR_EXPRESSION](#exclusive_or_expression)
- [AND_EXPRESSION](#and_expression)
- [EQUALITY_EXPRESSION](#equality_expression)
- [RELATIONAL_EXPRESSION](#relational_expression)
- [SHIFT_EXPRESSION](#shift_expression)
- [ADDITIVE_EXPRESSION](#additive_expression)
- [MULTIPLICATIVE_EXPRESSION](#multiplicative_expression)
- [UNARY_EXPRESSION](#unary_expression)
- [POSTFIX_EXPRESSION](#postfix_expression)
- [ARGUMENT_EXPRESSION_LIST](#argument_expression_list)
- [PRIMARY_EXPRESSION](#primary_expression)
- [CONSTANT](#constant)
---

## Grammar Rules

<a id="PROGRAM"></a>
**PROGRAM → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[GLOBAL_DECLARATIONS](#GLOBAL_DECLARATIONS) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PROGRAM [GLOBAL_DECLARATIONS](#GLOBAL_DECLARATIONS)**

_Description:_  
The Program rule represents the starting point of the grammar.

<a id="GLOBAL_DECLARATIONS"></a>

**GLOBAL_DECLARATIONS → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION](#declaration) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[FUNCTION_DEFINITION](#function_definition)**

_Description:_
The language supports either declarations or function declarations externally or globally. 

<a id="declaration_list"></a>
**DECLARATION_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION](#declaration) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DECLARATION_LIST [DECLARATION](#declaration)**

<a id="declaration"></a>
**DECLARATION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#declaration_specifiers) ; | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#declaration_specifiers) [INIT_DECLARATOR_LIST](#init_declarator_list) ;**

_Description:_  
This represents the entire process of defining a variable, function, or type.
A declaration introduces new identifiers and specifies their types.

```c
int x;                      // Simple declaration
float y = 3.14f, z = 0.0f;  // Declaration with initializers
```

<a id="declaration_specifiers"></a>
**DECLARATION_SPECIFIERS → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_SPECIFIER](#type_specifier) [DECLARATION_SPECIFIERS](#declaration_specifiers) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_SPECIFIER](#type_specifier) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_QUALIFIER](#type_qualifier) [DECLARATION_SPECIFIERS](#declaration_specifiers) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_QUALIFIER](#type_qualifier) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[FUNCTION_SPECIFIER](#function_specifier) [DECLARATION_SPECIFIERS](#declaration_specifiers) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[FUNCTION_SPECIFIER](#function_specifier) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[STORAGE_SPECIFIER](#storage_class_specifier) [DECLARATION_SPECIFIERS](#declaration_specifiers) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[STORAGE_SPECIFIER](#storage_class_specifier)**

This is for all declaration specifiers

<a id="init_declarator_list"></a>
**INIT_DECLARATOR_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[INIT_DECLARATOR](#init_declarator) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;INIT_DECLARATOR_LIST  ,  [INIT_DECLARATOR](#init_declarator)**

Used to declare one or more variables, optionally specifying the initial value

```c
x = 10
x, y
x = 1, y = 2 // Used for declaration with initializers
```

<a id="init_declarator"></a>
**INIT_DECLARATOR → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATOR](#declarator)  =  [INITIALIZER](#initializer) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATOR](#declarator)**

For initializing variables

```c
x = 10
a
```

<a id="storage_class_specifier"></a>
**STORAGE_SPECIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;typedef | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;static | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;auto | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;register**

<a id="type_qualifier"></a>
**TYPE_QUALIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;const | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;restrict | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;volatile**

<a id="type_qualifier_list"></a>
**TYPE_QUALIFIER_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_QUALIFIER](#type_qualifier) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;TYPE_QUALIFIER_LIST [TYPE_QUALIFIER](#type_qualifier)**

For lists of type qualifiers

<a id="function_specifier"></a>
**FUNCTION_SPECIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;inline**

<a id="type_specifier"></a>
**TYPE_SPECIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;short | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;long | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;float | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;double | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;signed | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unsigned | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;bool | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[STRUCT_SPECIFIER](#struct_or_union_specifier)**

<a id="function_definition"></a>
**FUNCTION_DEFINITION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#declaration_specifiers) [DECLARATOR](#declarator) [DECLARATION_LIST](#declaration_list) [BLOCK](#BLOCK) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#declaration_specifiers) [DECLARATOR](#declarator) [BLOCK](#BLOCK)**

A declaration of a function follows a declaration specifier (the function type), the declarator

<a id="struct_or_union_specifier"></a>
**STRUCT_SPECIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;struct  {  [STRUCT_DECLARATION_LIST](#struct_declaration_list)  }  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;struct IDENTIFIER  {  [STRUCT_DECLARATION_LIST](#struct_declaration_list)  }  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;struct IDENTIFIER**

For parsing structs specifiers
```c
struct Point
{    
     int x;    
     int y;
};
```

<a id="struct_declaration_list"></a>
**STRUCT_DECLARATION_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[STRUCT_DECLARATION](#struct_declaration) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;STRUCT_DECLARATION_LIST [STRUCT_DECLARATION](#struct_declaration)**

Declarations work different inside a struct, hence a `STRUCT_DECLARATION` is needed. 
```c
struct str {
    int i = 10; // This is not allowed in C
};
struct str {
    int i; // Allowed in C
};
```

<a id="struct_declaration"></a>
**STRUCT_DECLARATION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[SPECIFIER_QUALIFIER_LIST](#specifier_qualifier_list)  ;  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[SPECIFIER_QUALIFIER_LIST](#specifier_qualifier_list) [STRUCT_DECLARATOR_LIST](#struct_declarator_list)  ; **

<a id="specifier_qualifier_list"></a>
**SPECIFIER_QUALIFIER_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_SPECIFIER](#type_specifier) SPECIFIER_QUALIFIER_LIST | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_SPECIFIER](#type_specifier) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_QUALIFIER](#type_qualifier) SPECIFIER_QUALIFIER_LIST | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_QUALIFIER](#type_qualifier)**

This is for types lists
```c
int
const
unsigned long
volatile const int
```

<a id="struct_declarator_list"></a>
**STRUCT_DECLARATOR_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATOR](#declarator) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;STRUCT_DECLARATOR_LIST  ,  [DECLARATOR](#declarator)**

List of declarations in a struct


<a id="declarator"></a>
**DECLARATOR → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[POINTER](#pointer) [DIRECT_DECLARATOR](#direct_declarator) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DIRECT_DECLARATOR](#direct_declarator)**

Specifies the name of a variable, function, or pointer along with any additional information about its structure.

```c
x
*ptr
arr[10]
func_name(int a, float b)
```

<a id="direct_declarator"></a>
**DIRECT_DECLARATOR → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; (  [DECLARATOR](#declarator)  )  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  [   ]  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  [   *   ]  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  [  static [TYPE_QUALIFIER_LIST](#type_qualifier_list) [ASSIGNMENT_EXPRESSION](#assignment_expression)  ]  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  [  static [ASSIGNMENT_EXPRESSION](#assignment_expression)  ]  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  [  [TYPE_QUALIFIER_LIST](#type_qualifier_list)  *   ]  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  [  [TYPE_QUALIFIER_LIST](#type_qualifier_list) static [ASSIGNMENT_EXPRESSION](#assignment_expression)  ]  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  [  [TYPE_QUALIFIER_LIST](#type_qualifier_list) [ASSIGNMENT_EXPRESSION](#assignment_expression)  ]  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  [  [TYPE_QUALIFIER_LIST](#type_qualifier_list)  ]  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  [  [ASSIGNMENT_EXPRESSION](#assignment_expression)  ]  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  (  [PARAMETER_LIST](#parameter_list)  )  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  (   )  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DIRECT_DECLARATOR  (  [IDENTIFIER_LIST](#identifier_list)  )**

This is for all extra information that a declarator may have on a structure (parameter lists, assignment lists, etc)
Note: some invalid structures will be caught at semantic level, example: int func[]{}

<a id="pointer"></a>
**POINTER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *  [TYPE_QUALIFIER_LIST](#type_qualifier_list) POINTER | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *  [TYPE_QUALIFIER_LIST](#type_qualifier_list) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *  POINTER | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;***

For identifying pointers. 

<a id="parameter_list"></a>
**PARAMETER_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[PARAMETER_DECLARATION](#parameter_declaration) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PARAMETER_LIST  ,  [PARAMETER_DECLARATION](#parameter_declaration)**

```c
int x, float c, char b
```

<a id="parameter_declaration"></a>
**PARAMETER_DECLARATION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#declaration_specifiers) [DECLARATOR](#declarator) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#declaration_specifiers)**

```c
int x
```

<a id="identifier_list"></a>
**IDENTIFIER_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER_LIST  ,  IDENTIFIER**

```c
one, two, apple, orange
```

<a id="initializer"></a>
**INITIALIZER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; {  [INITIALIZER_LIST](#initializer_list)  }  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; {  [INITIALIZER_LIST](#initializer_list)  ,   }  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ASSIGNMENT_EXPRESSION](#assignment_expression)**

For initializing variables

```c
int x = 5; // 5 is an initializer
{1, 2, 3} // This is an initializer for an array
{1, 2, 3 ,} // This is also valid in C
```

<a id="initializer_list"></a>
**INITIALIZER_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[INITIALIZER](#initializer) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;INITIALIZER_LIST  ,  [INITIALIZER](#initializer)**

<a id="statement"></a>
**STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[LABELED_STATEMENT](#labeled_statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[BLOCK](#BLOCK) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[EXPRESSION_STATEMENT](#expression_statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[SELECTION_STATEMENT](#selection_statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ITERATION_STATEMENT](#iteration_statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[JUMP_STATEMENT](#jump_statement)**

This are the supported statements in this language

<a id="labeled_statement"></a>
**LABELED_STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER  :  [STATEMENT](#statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;case [CONDITIONAL_EXPRESSION](#conditional_expression)  :  [STATEMENT](#statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;default  :  [STATEMENT](#statement)**

This will be used in switch cases, example: 
```c
switch (expression) {
    case constant1: // case label
        // statements
        break;
    default: //default label
        // statements
}
```
In c a constant expresion can be a conditional expression if this expresion can be evaluated at compile time, hence conditional expressions are in case labels.

<a id="BLOCK"></a>
**BLOCK → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; {   }  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; {  [BLOCK_ITEM_LIST](#block_item_list)  }**

A block can have a series of declarations and statements inside it.

<a id="block_item"></a>
**BLOCK_ITEM → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION](#declaration) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[STATEMENT](#statement)**

Can be either a declaration or a statement

<a id="block_item_list"></a>
**BLOCK_ITEM_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[BLOCK_ITEM](#block_item) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;BLOCK_ITEM_LIST [BLOCK_ITEM](#block_item)**

A list of block items

<a id="expression_statement"></a>
**EXPRESSION_STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ;  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[EXPRESSION](#expression)  ;**

This is used to execute an expression, example:
```c
int x = 5;
x = 10; // This is an expression statement, assignment 
; // This is an empty statement, valid in C
```

<a id="expression"></a>
**EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ASSIGNMENT_EXPRESSION](#assignment_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EXPRESSION  ,  [ASSIGNMENT_EXPRESSION](#assignment_expression)**

Example:
```c
int a = 1, b = 2, c;
c = (a = 5, b = a + 3); // this is useful in for loops
```

<a id="assignment_expression"></a>
**ASSIGNMENT_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[CONDITIONAL_EXPRESSION](#conditional_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[UNARY_EXPRESSION](#unary_expression) [ASSIGNMENT_OPERATOR](#assignment_operator) [ASSIGNMENT_EXPRESSION](#assignment_expression)**

Example:
``` c
int a = 5;
x = y = 10;  // multiple assignments
x = func(1); 
x += a; // Compound assignment
```

<a id="selection_statement"></a>
**SELECTION_STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;if  (  [EXPRESSION](#expression)  )  [STATEMENT](#statement) else [STATEMENT](#statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;if  (  [EXPRESSION](#expression)  )  [STATEMENT](#statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;switch  (  [EXPRESSION](#expression)  )  [STATEMENT](#statement)**

`else if` is supported because following an `else` we have a statement that can be another `if` statement
```c
if (expression) {
    // statements
} else if (expression) {
    // statements
} else {
    // statements
}
```

<a id="iteration_statement"></a>
**ITERATION_STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;while  (  [EXPRESSION](#expression)  )  [STATEMENT](#statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;do [STATEMENT](#statement) while  (  [EXPRESSION](#expression)  )   ;  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;for  (  [EXPRESSION_STATEMENT](#expression_statement) [EXPRESSION_STATEMENT](#expression_statement)  )  [STATEMENT](#statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;for  (  [EXPRESSION_STATEMENT](#expression_statement) [EXPRESSION_STATEMENT](#expression_statement) [EXPRESSION](#expression)  )  [STATEMENT](#statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;for  (  [DECLARATION](#declaration) [EXPRESSION_STATEMENT](#expression_statement)  )  [STATEMENT](#statement) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;for  (  [DECLARATION](#declaration) [EXPRESSION_STATEMENT](#expression_statement) [EXPRESSION](#expression)  )  [STATEMENT](#statement)**

<a id="jump_statement"></a>
**JUMP_STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;continue  ;  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;break  ;  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return  ;  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return [EXPRESSION](#expression)  ;**

<a id="assignment_operator"></a>
**ASSIGNMENT_OPERATOR → <br>**
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; =  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *=  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; /=  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; %=  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; +=  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -=  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <<=  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; >>=  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &=  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ^=  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |=

These are identified by our lexer as single tokens, meaning `<<=` is a single token.

<a id="conditional_expression"></a>
**CONDITIONAL_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[LOGICAL_OR_EXPRESSION](#logical_or_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[LOGICAL_OR_EXPRESSION](#logical_or_expression)  ?  [EXPRESSION](#expression)  :  [CONDITIONAL_EXPRESSION](#conditional_expression)**

The ternary operator is supported

<a id="logical_or_expression"></a>
**LOGICAL_OR_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[LOGICAL_AND_EXPRESSION](#logical_and_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;LOGICAL_OR_EXPRESSION  ||  [LOGICAL_AND_EXPRESSION](#logical_and_expression)**

Involves logical operators like `&&`, `||`. This rule is for nested logical OR/AND expressions. The AND operation has higher priority than the OR operation. Example:
```c
a && b || c && d 
```
is equivalent to
```c
(a && b) || (c && d)
```

<a id="logical_and_expression"></a>
**LOGICAL_AND_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[INCLUSIVE_OR_EXPRESSION](#inclusive_or_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;LOGICAL_AND_EXPRESSION  &&  [INCLUSIVE_OR_EXPRESSION](#inclusive_or_expression)**

<a id="inclusive_or_expression"></a>
**INCLUSIVE_OR_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[EXCLUSIVE_OR_EXPRESSION](#exclusive_or_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;INCLUSIVE_OR_EXPRESSION  |  [EXCLUSIVE_OR_EXPRESSION](#exclusive_or_expression)**

<a id="exclusive_or_expression"></a>
**EXCLUSIVE_OR_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[AND_EXPRESSION](#and_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EXCLUSIVE_OR_EXPRESSION  ^  [AND_EXPRESSION](#and_expression)**

Exclusive and inclusive OR are bitwise operators.

<a id="and_expression"></a>
**AND_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[EQUALITY_EXPRESSION](#equality_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;AND_EXPRESSION  &  [EQUALITY_EXPRESSION](#equality_expression)**

<a id="equality_expression"></a>
**EQUALITY_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[RELATIONAL_EXPRESSION](#relational_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EQUALITY_EXPRESSION  ==  [RELATIONAL_EXPRESSION](#relational_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;EQUALITY_EXPRESSION  !=  [RELATIONAL_EXPRESSION](#relational_expression)**

Assesses equality of two expressions.

<a id="relational_expression"></a>
**RELATIONAL_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[SHIFT_EXPRESSION](#shift_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;RELATIONAL_EXPRESSION  <  [SHIFT_EXPRESSION](#shift_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;RELATIONAL_EXPRESSION  >  [SHIFT_EXPRESSION](#shift_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;RELATIONAL_EXPRESSION  <=  [SHIFT_EXPRESSION](#shift_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;RELATIONAL_EXPRESSION  >=  [SHIFT_EXPRESSION](#shift_expression)**

Relational operators compare two expressions based on their value.

<a id="shift_expression"></a>
**SHIFT_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ADDITIVE_EXPRESSION](#additive_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SHIFT_EXPRESSION  <<  [ADDITIVE_EXPRESSION](#additive_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;SHIFT_EXPRESSION  >>  [ADDITIVE_EXPRESSION](#additive_expression)**

Shift operators are used to shift the bits of an expression to the left or right.

```c
int a = 10; // 10 in binary is 1010
int b = a << 1; // This will shift the bits of a to the left by 1, so b will be 20 (10100 in binary)
```

<a id="additive_expression"></a>
**ADDITIVE_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[MULTIPLICATIVE_EXPRESSION](#multiplicative_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ADDITIVE_EXPRESSION  +  [MULTIPLICATIVE_EXPRESSION](#multiplicative_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ADDITIVE_EXPRESSION  -  [MULTIPLICATIVE_EXPRESSION](#multiplicative_expression)**

Additive operators are used to add or subtract two expressions.

<a id="multiplicative_expression"></a>
**MULTIPLICATIVE_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[UNARY_EXPRESSION](#unary_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MULTIPLICATIVE_EXPRESSION  *  [UNARY_EXPRESSION](#unary_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MULTIPLICATIVE_EXPRESSION  /  [UNARY_EXPRESSION](#unary_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;MULTIPLICATIVE_EXPRESSION  %  [UNARY_EXPRESSION](#unary_expression)**

Multiplicative operators are used to multiply or divide two expressions.

To multiplicative expression follows a unary expression because something like the following is valid:

```c
int a = 5, b = 3;
int result = a * -b;
```

<a id="unary_expression"></a>
**UNARY_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[POSTFIX_EXPRESSION](#postfix_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ++  UNARY_EXPRESSION | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --  UNARY_EXPRESSION | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; sizeof  UNARY_EXPRESSION**

A unary expression is an expression that has only one operand.

<a id="postfix_expression"></a>
**POSTFIX_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[PRIMARY_EXPRESSION](#primary_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;POSTFIX_EXPRESSION  [  [EXPRESSION](#expression)  ]  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;POSTFIX_EXPRESSION  (   )  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;POSTFIX_EXPRESSION  (  [ARGUMENT_EXPRESSION_LIST](#argument_expression_list)  )  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;POSTFIX_EXPRESSION  .  IDENTIFIER | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;POSTFIX_EXPRESSION  ->  IDENTIFIER | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;POSTFIX_EXPRESSION  ++  | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;POSTFIX_EXPRESSION  --**

`a[5]` is a postfix expression because it has a primary expression and an expression between brackets. The `. IDENTIFIER` and `-> IDENTIFIER` are used for structs.

``` c
int a = 5;
int b = a++; 
int c = a--; 
```

<a id="argument_expression_list"></a>
**ARGUMENT_EXPRESSION_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ASSIGNMENT_EXPRESSION](#assignment_expression) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ARGUMENT_EXPRESSION_LIST  ,  [ASSIGNMENT_EXPRESSION](#assignment_expression)**

This is used for function calls with a list of arguments.

```c
int a = 5, b = 3;
int result = func(a, b, 1+2);
```

<a id="primary_expression"></a>
**PRIMARY_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[CONSTANT](#constant) | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; (  [EXPRESSION](#expression)  )**

<a id="constant"></a>
**CONSTANT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;INTEGER_CONSTANT | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;FLOAT_CONSTANT | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;STRING_LITERAL**

Integer, float, and string literal constants are passed by the lexer.

