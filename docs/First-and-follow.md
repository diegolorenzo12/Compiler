# First and follow

This document contains the first and follow from the grammer in [CFG-left-factorizing](./CFG-left-factoring.md)

## First

<a id="PROGRAM"></a>
**FIRST(PROGRAM)** = [FIRST(GLOBAL_DECLARATIONS)](#GLOBAL_DECLARATIONS) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `func`}

<a id="PROGRAM_PRIME"></a>
**FIRST(PROGRAM_PRIME)** = [FIRST(GLOBAL_DECLARATIONS)](#GLOBAL_DECLARATIONS) ∪ ε = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `func`, `ε`}

<a id="GLOBAL_DECLARATIONS"></a>
**FIRST(GLOBAL_DECLARATIONS)** = [FIRST(DECLARATION)](#DECLARATION) ∪ [FIRST(FUNCTION_DEFINITION)](#FUNCTION_DEFINITION) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `func`}

<a id="DECLARATION_LIST"></a>
**FIRST(DECLARATION_LIST)** = [FIRST(DECLARATION)](#DECLARATION) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`}

<a id="DECLARATION_LIST_PRIME"></a>
**FIRST(DECLARATION_LIST_PRIME)** = [FIRST(DECLARATION)](#DECLARATION) ∪ ε = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `ε`}

<a id="DECLARATION"></a>
**FIRST(DECLARATION)** = [FIRST(DECLARATION_SPECIFIERS)](#DECLARATION_SPECIFIERS) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`}

<a id="DECLARATION_FAC"></a>
**FIRST(DECLARATION_FAC)** = `;` ∪ [FIRST(INIT_DECLARATOR_LIST)](#INIT_DECLARATOR_LIST) = {`;`, `const`, `restrict`, `volatile`, `*`, `identifier`, `(`}

<a id="DECLARATION_SPECIFIERS"></a>
**FIRST(DECLARATION_SPECIFIERS)** = [FIRST(TYPE_SPECIFIER)](#TYPE_SPECIFIER) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`}

<a id="DECLARATION_SPECIFIERS_PRIME"></a>
**FIRST(DECLARATION_SPECIFIERS_PRIME)** = [FIRST(DECLARATION_SPECIFIERS)](#DECLARATION_SPECIFIERS) ∪ [FIRST(TYPE_QUALIFIER)](#TYPE_QUALIFIER) ∪ [FIRST(FUNCTION_SPECIFIER)](#FUNCTION_SPECIFIER) ∪ [FIRST(STORAGE_SPECIFIER)](#STORAGE_SPECIFIER) ∪ ε = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `const`, `restrict`, `volatile`, `inline`, `static`, `auto`, `register`, `ε`}

<a id="INIT_DECLARATOR_LIST"></a>
**FIRST(INIT_DECLARATOR_LIST)** = [FIRST(INIT_DECLARATOR)](#INIT_DECLARATOR) = {`const`, `restrict`, `volatile`, `*`, `identifier`, `(`}

<a id="INIT_DECLARATOR_LIST_PRIME"></a>
**FIRST(INIT_DECLARATOR_LIST_PRIME)** = {`,`, `ε`}

<a id="INIT_DECLARATOR"></a>
**FIRST(INIT_DECLARATOR)** = [FIRST(DECLARATOR)](#DECLARATOR) = {`const`, `restrict`, `volatile`, `*`, `identifier`, `(`}

<a id="INIT_DECLARATOR_FAC"></a>
**FIRST(INIT_DECLARATOR_FAC)** = {`=`, `ε`}

<a id="STORAGE_SPECIFIER"></a>
**FIRST(STORAGE_SPECIFIER)** = {`static`, `auto`, `register`}

<a id="TYPE_QUALIFIER"></a>
**FIRST(TYPE_QUALIFIER)** = {`const`, `restrict`, `volatile`}

<a id="TYPE_QUALIFIER_LIST"></a>
**FIRST(TYPE_QUALIFIER_LIST)** = [FIRST(TYPE_QUALIFIER)](#TYPE_QUALIFIER) = {`const`, `restrict`, `volatile`}

<a id="TYPE_QUALIFIER_LIST_PRIME"></a>
**FIRST(TYPE_QUALIFIER_LIST_PRIME)** = [FIRST(TYPE_QUALIFIER)](#TYPE_QUALIFIER) ∪ ε = {`const`, `restrict`, `volatile`, `ε`}

<a id="FUNCTION_SPECIFIER"></a>
**FIRST(FUNCTION_SPECIFIER)** = {`inline`}

<a id="TYPE_SPECIFIER"></a>
**FIRST(TYPE_SPECIFIER)** = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`} ∪ [FIRST(STRUCT_SPECIFIER)](#STRUCT_SPECIFIER) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`}

<a id="FUNCTION_DEFINITION"></a>
**FIRST(FUNCTION_DEFINITION)** = {`func`}

<a id="FUNCTION_DEF_FAC"></a>
**FIRST(FUNCTION_DEF_FAC)** = [FIRST(DECLARATION_LIST)](#DECLARATION_LIST) ∪ [FIRST(BLOCK)](#BLOCK) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `{`, `}`}

<a id="STRUCT_SPECIFIER"></a>
**FIRST(STRUCT_SPECIFIER)** = {`struct`}

<a id="STRUCT_SPEC_FAC"></a>
**FIRST(STRUCT_SPEC_FAC)** = {`{`, `identifier`}

<a id="STRUCT_SPEC_FAC2"></a>
**FIRST(STRUCT_SPEC_FAC2)** = {`{`, `ε`}

<a id="STRUCT_DECLARATION_LIST"></a>
**FIRST(STRUCT_DECLARATION_LIST)** = [FIRST(STRUCT_DECLARATION)](#STRUCT_DECLARATION) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`}

<a id="STRUCT_DECLARATION_LIST_PRIME"></a>
**FIRST(STRUCT_DECLARATION_LIST_PRIME)** = [FIRST(STRUCT_DECLARATION)](#STRUCT_DECLARATION) ∪ ε = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `ε`}

<a id="STRUCT_DECLARATION"></a>
**FIRST(STRUCT_DECLARATION)** = [FIRST(SPECIFIER_QUALIFIER_LIST)](#SPECIFIER_QUALIFIER_LIST) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`}

<a id="STRUC_DECL_FAC"></a>
**FIRST(STRUC_DECL_FAC)** = `;` ∪ [FIRST(STRUCT_DECLARATOR_LIST)](#STRUCT_DECLARATOR_LIST) = {`;`, `const`, `restrict`, `volatile`, `*`, `identifier`, `(`}

<a id="SPECIFIER_QUALIFIER_LIST"></a>
**FIRST(SPECIFIER_QUALIFIER_LIST)** = [FIRST(TYPE_SPECIFIER)](#TYPE_SPECIFIER) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`}

<a id="SPECIFIER_QUALIFIER_LIST_PRIME"></a>
**FIRST(SPECIFIER_QUALIFIER_LIST_PRIME)** = [FIRST(SPECIFIER_QUALIFIER_LIST)](#SPECIFIER_QUALIFIER_LIST) ∪ [FIRST(TYPE_QUALIFIER)](#TYPE_QUALIFIER) ∪ ε = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `const`, `restrict`, `volatile`, `ε`}

<a id="STRUCT_DECLARATOR_LIST"></a>
**FIRST(STRUCT_DECLARATOR_LIST)** = [FIRST(DECLARATOR)](#DECLARATOR) = {`const`, `restrict`, `volatile`, `*`, `identifier`, `(`}

<a id="STRUCT_DECLARATOR_LIST_PRIME"></a>
**FIRST(STRUCT_DECLARATOR_LIST_PRIME)** = {`,`, `ε`}

<a id="DECLARATOR"></a>
**FIRST(DECLARATOR)** = [FIRST(POINTER)](#POINTER) ∪ [FIRST(DIRECT_DECLARATOR)](#DIRECT_DECLARATOR) = {`const`, `restrict`, `volatile`, `*`, `identifier`, `(`}

<a id="DIRECT_DECLARATOR"></a>
**FIRST(DIRECT_DECLARATOR)** = {`identifier`, `(`}

<a id="DIRECT_DECLARATOR_PRIME"></a>
**FIRST(DIRECT_DECLARATOR_PRIME)** = {`[`, `(`, `ε`}

<a id="STATIC_OPT_FAC"></a>
**FIRST(STATIC_OPT_FAC)** = {`static`, `ε`}

<a id="TYPE_QUALIFIER_LIST_OPT_FAC"></a>
**FIRST(TYPE_QUALIFIER_LIST_OPT_FAC)** = [FIRST(TYPE_QUALIFIER_LIST)](#TYPE_QUALIFIER_LIST) ∪ ε = {`const`, `restrict`, `volatile`, `ε`}

<a id="ASSIGNMENT_EXPRESSION_OPT_FAC"></a>
**FIRST(ASSIGNMENT_EXPRESSION_OPT_FAC)** = [FIRST(ASSIGNMENT_EXPRESSION)](#ASSIGNMENT_EXPRESSION) ∪ ε = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`, `ε`}

<a id="PARAMETER_LIST_OPT_FAC"></a>
**FIRST(PARAMETER_LIST_OPT_FAC)** = [FIRST(PARAMETER_LIST)](#PARAMETER_LIST) ∪ [FIRST(IDENTIFIER_LIST)](#IDENTIFIER_LIST) ∪ ε = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `identifier`, `ε`}

<a id="POINTER"></a>
**FIRST(POINTER)** = [FIRST(TYPE_QUALIFIER_LIST)](#TYPE_QUALIFIER_LIST) ∪ `*` = {`const`, `restrict`, `volatile`, `*`}

<a id="PARAMETER_LIST"></a>
**FIRST(PARAMETER_LIST)** = [FIRST(PARAMETER_DECLARATION)](#PARAMETER_DECLARATION) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`}

<a id="PARAMETER_LIST_PRIME"></a>
**FIRST(PARAMETER_LIST_PRIME)** = {`,`, `ε`}

<a id="PARAMETER_DECLARATION"></a>
**FIRST(PARAMETER_DECLARATION)** = [FIRST(DECLARATION_SPECIFIERS)](#DECLARATION_SPECIFIERS) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`}

<a id="PARAM_DECL_FAC"></a>
**FIRST(PARAM_DECL_FAC)** = [FIRST(DECLARATOR)](#DECLARATOR) ∪ ε = {`const`, `restrict`, `volatile`, `*`, `identifier`, `(`, `ε`}

<a id="IDENTIFIER_LIST"></a>
**FIRST(IDENTIFIER_LIST)** = {`identifier`}

<a id="IDENTIFIER_LIST_PRIME"></a>
**FIRST(IDENTIFIER_LIST_PRIME)** = {`,`, `ε`}

<a id="INITIALIZER"></a>
**FIRST(INITIALIZER)** = [FIRST(INITIALIZER_BRAZE_FAC)](#INITIALIZER_BRAZE_FAC) ∪ [FIRST(ASSIGNMENT_EXPRESSION)](#ASSIGNMENT_EXPRESSION) = {`{`, `identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="INITIALIZER_BRACE_FAC"></a>
**FIRST(INITIALIZER_BRACE_FAC)** = {`{`, `}`}

<a id="INITIALIZER_BRACE_FAC2"></a>
**FIRST(INITIALIZER_BRACE_FAC2)** = {`ε`, `,`}

<a id="INITIALIZER_LIST"></a>
**FIRST(INITIALIZER_LIST)** = [FIRST(INITIALIZER)](#INITIALIZER) = {`{`, `identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="INITIALIZER_LIST_PRIME"></a>
**FIRST(INITIALIZER_LIST_PRIME)** = {`,`, `ε`}

<a id="STATEMENT"></a>
**FIRST(STATEMENT)** = [FIRST(LABELED_STATEMENT)](#LABELED_STATEMENT) ∪ [FIRST(BLOCK)](#BLOCK) ∪ [FIRST(EXPRESSION_STATEMENT)](#EXPRESSION_STATEMENT) ∪ [FIRST(SELECTION_STATEMENT)](#SELECTION_STATEMENT) ∪ [FIRST(ITERATION_STATEMENT)](#ITERATION_STATEMENT) ∪ [FIRST(JUMP_STATEMENT)](#JUMP_STATEMENT) = {`identifier`, `case`, `default`, `{`, `;`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`, `if`, `switch`, `while`, `do`, `for`, `continue`, `break`, `return`}

<a id="LABELED_STATEMENT"></a>
**FIRST(LABELED_STATEMENT)** = {`case`, `default`}

<a id="BLOCK"></a>
**FIRST(BLOCK)** = {`{`, `}`}

<a id="BLOCK_CONTENT"></a>
**FIRST(BLOCK_CONTENT)** = [FIRST(BLOCK_ITEM_LIST)](#BLOCK_ITEM_LIST) ∪ ε = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `identifier`, `case`, `default`, `{`, `;`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`, `if`, `switch`, `while`, `do`, `for`, `continue`, `break`, `return`, `ε`}

<a id="BLOCK_ITEM"></a>
**FIRST(BLOCK_ITEM)** = [FIRST(DECLARATION)](#DECLARATION) ∪ [FIRST(STATEMENT)](#STATEMENT) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `identifier`, `case`, `default`, `{`, `;`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`, `if`, `switch`, `while`, `do`, `for`, `continue`, `break`, `return`}

<a id="BLOCK_ITEM_LIST"></a>
**FIRST(BLOCK_ITEM_LIST)** = [FIRST(BLOCK_ITEM)](#BLOCK_ITEM) = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `identifier`, `case`, `default`, `{`, `;`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`, `if`, `switch`, `while`, `do`, `for`, `continue`, `break`, `return`}

<a id="BLOCK_ITEM_LIST_PRIME"></a>
**FIRST(BLOCK_ITEM_LIST_PRIME)** = [FIRST(BLOCK_ITEM)](#BLOCK_ITEM) ∪ ε = {`int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`, `identifier`, `case`, `default`, `{`, `;`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`, `if`, `switch`, `while`, `do`, `for`, `continue`, `break`, `return`, `ε`}

<a id="EXPRESSION_STATEMENT"></a>
**FIRST(EXPRESSION_STATEMENT)** = `;` ∪ [FIRST(EXPRESSION)](#EXPRESSION) = {`;`, `identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="EXPRESSION"></a>
**FIRST(EXPRESSION)** = [FIRST(ASSIGNMENT_EXPRESSION)](#ASSIGNMENT_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="EXPRESSION_PRIME"></a>
**FIRST(EXPRESSION_PRIME)** = {`,`, `ε`}

<a id="ASSIGNMENT_EXPRESSION"></a>
**FIRST(ASSIGNMENT_EXPRESSION)** = [FIRST(CONDITIONAL_EXPRESSION)](#CONDITIONAL_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="ASSIGNMENT_EXPRESSION_FAC"></a>
**FIRST(ASSIGNMENT_EXPRESSION_FAC)** = [FIRST(ASSIGNMENT_OPERATOR)](#ASSIGNMENT_OPERATOR) ∪ ε = {`=`, `*=`, `/=`, `%=`, `+=`, `-=`, `<<=`, `>>=`, `&&=`, `^=`, `|=`, `ε`}

<a id="SELECTION_STATEMENT"></a>
**FIRST(SELECTION_STATEMENT)** = {`if`, `switch`}

<a id="ELSE_FAC"></a>
**FIRST(ELSE_FAC)** = {`else`, `ε`}

<a id="ITERATION_STATEMENT"></a>
**FIRST(ITERATION_STATEMENT)** = {`while`, `do`, `for`}

<a id="FOR_INIT_STATEMENT"></a>
**FIRST(FOR_INIT_STATEMENT)** = [FIRST(EXPRESSION_STATEMENT)](#EXPRESSION_STATEMENT) ∪ [FIRST(DECLARATION)](#DECLARATION) = {`;`, `identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`, `int`, `void`, `char`, `short`, `long`, `float`, `double`, `signed`, `unsigned`, `bool`, `struct`}

<a id="FOR_OPTIONAL_EXPRESSION"></a>
**FIRST(FOR_OPTIONAL_EXPRESSION)** = [FIRST(EXPRESSION)](#EXPRESSION) ∪ ε = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`, `ε`}

<a id="JUMP_STATEMENT"></a>
**FIRST(JUMP_STATEMENT)** = {`continue`, `break`, `return`}

<a id="ASSIGNMENT_OPERATOR"></a>
**FIRST(ASSIGNMENT_OPERATOR)** = {`=`, `*=`, `/=`, `%=`, `+=`, `-=`, `<<=`, `>>=`, `&&=`, `^=`, `|=`}

<a id="CONDITIONAL_EXPRESSION"></a>
**FIRST(CONDITIONAL_EXPRESSION)** = [FIRST(LOGICAL_OR_EXPRESSION)](#LOGICAL_OR_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="CONDITIONAL_EXPRESSION_PRIME"></a>
**FIRST(CONDITIONAL_EXPRESSION_PRIME)** = {`?`, `ε`}

<a id="LOGICAL_OR_EXPRESSION"></a>
**FIRST(LOGICAL_OR_EXPRESSION)** = [FIRST(LOGICAL_AND_EXPRESSION)](#LOGICAL_AND_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="LOGICAL_OR_EXPRESSION_PRIME"></a>
**FIRST(LOGICAL_OR_EXPRESSION_PRIME)** = {`||`, `ε`}

<a id="LOGICAL_AND_EXPRESSION"></a>
**FIRST(LOGICAL_AND_EXPRESSION)** = [FIRST(INCLUSIVE_OR_EXPRESSION)](#INCLUSIVE_OR_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="LOGICAL_AND_EXPRESSION_PRIME"></a>
**FIRST(LOGICAL_AND_EXPRESSION_PRIME)** = {`&&`, `ε`}

<a id="INCLUSIVE_OR_EXPRESSION"></a>
**FIRST(INCLUSIVE_OR_EXPRESSION)** = [FIRST(EXCLUSIVE_OR_EXPRESSION)](#EXCLUSIVE_OR_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="INCLUSIVE_OR_EXPRESSION_PRIME"></a>
**FIRST(INCLUSIVE_OR_EXPRESSION_PRIME)** = {`|`, `ε`}

<a id="EXCLUSIVE_OR_EXPRESSION"></a>
**FIRST(EXCLUSIVE_OR_EXPRESSION)** = [FIRST(AND_EXPRESSION)](#AND_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="EXCLUSIVE_OR_EXPRESSION_PRIME"></a>
**FIRST(EXCLUSIVE_OR_EXPRESSION_PRIME)** = {`^`, `ε`}

<a id="AND_EXPRESSION"></a>
**FIRST(AND_EXPRESSION)** = [FIRST(EQUALITY_EXPRESSION)](#EQUALITY_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="AND_EXPRESSION_PRIME"></a>
**FIRST(AND_EXPRESSION_PRIME)** = {`&`, `ε`}

<a id="EQUALITY_EXPRESSION"></a>
**FIRST(EQUALITY_EXPRESSION)** = [FIRST(RELATIONAL_EXPRESSION)](#RELATIONAL_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="EQUALITY_EXPRESSION_PRIME"></a>
**FIRST(EQUALITY_EXPRESSION_PRIME)** = {`==`, `!=`, `ε`}

<a id="RELATIONAL_EXPRESSION"></a>
**FIRST(RELATIONAL_EXPRESSION)** = [FIRST(SHIFT_EXPRESSION)](#SHIFT_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="RELATIONAL_EXPRESSION_PRIME"></a>
**FIRST(RELATIONAL_EXPRESSION_PRIME)** = {`<`, `>`, `<=`, `>=`, `ε`}

<a id="SHIFT_EXPRESSION"></a>
**FIRST(SHIFT_EXPRESSION)** = [FIRST(ADDITIVE_EXPRESSION)](#ADDITIVE_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="SHIFT_EXPRESSION_PRIME"></a>
**FIRST(SHIFT_EXPRESSION_PRIME)** = {`<<`, `>>`, `ε`}

<a id="ADDITIVE_EXPRESSION"></a>
**FIRST(ADDITIVE_EXPRESSION)** = [FIRST(MULTIPLICATIVE_EXPRESSION)](#MULTIPLICATIVE_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="ADDITIVE_EXPRESSION_PRIME"></a>
**FIRST(ADDITIVE_EXPRESSION_PRIME)** = {`+`, `-`, `ε`}

<a id="MULTIPLICATIVE_EXPRESSION"></a>
**FIRST(MULTIPLICATIVE_EXPRESSION)** = [FIRST(UNARY_EXPRESSION)](#UNARY_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="MULTIPLICATIVE_EXPRESSION_PRIME"></a>
**FIRST(MULTIPLICATIVE_EXPRESSION_PRIME)** = {`*`, `/`, `%`, `ε`}

<a id="UNARY_EXPRESSION"></a>
**FIRST(UNARY_EXPRESSION)** = [FIRST(POSTFIX_EXPRESSION)](#POSTFIX_EXPRESSION) ∪ `++` ∪ `--` ∪ `sizeof` = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="POSTFIX_EXPRESSION"></a>
**FIRST(POSTFIX_EXPRESSION)** = [FIRST(PRIMARY_EXPRESSION)](#PRIMARY_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`}

<a id="POSTFIX_EXPRESSION_PRIME"></a>
**FIRST(POSTFIX_EXPRESSION_PRIME)** = {`[`, `(`, `.`, `->`, `++`, `--`, `ε`}

<a id="POSTFIX_ARGUMENTS"></a>
**FIRST(POSTFIX_ARGUMENTS)** = [FIRST(ARGUMENT_EXPRESSION_LIST)](#ARGUMENT_EXPRESSION_LIST) ∪ ε = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`, `ε`}

<a id="ARGUMENT_EXPRESSION_LIST"></a>
**FIRST(ARGUMENT_EXPRESSION_LIST)** = [FIRST(ASSIGNMENT_EXPRESSION)](#ASSIGNMENT_EXPRESSION) = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`, `++`, `--`, `sizeof`}

<a id="ARGUMENT_EXPRESSION_LIST_PRIME"></a>
**FIRST(ARGUMENT_EXPRESSION_LIST_PRIME)** = {`,`, `ε`}

<a id="PRIMARY_EXPRESSION"></a>
**FIRST(PRIMARY_EXPRESSION)** = `identifier` ∪ [FIRST(CONSTANT)](#CONSTANT) ∪ `(` = {`identifier`, `integer_constant`, `float_constant`, `string_literal`, `(`}

<a id="CONSTANT"></a>
**FIRST(CONSTANT)** = {`integer_constant`, `float_constant`, `string_literal`}

## Follow

In this follow analysis `$` denotes end of line

**FOLLOW(ADDITIVE_EXPRESSION)**= {`!=` `%=` `&` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<` `<<` `<<=` `<=` `=` `==` `>` `>=` `>>` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(ADDITIVE_EXPRESSION_PRIME)**= {`!=` `%=` `&` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<` `<<` `<<=` `<=` `=` `==` `>` `>=` `>>` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(AND_EXPRESSION)**= {`%=` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(AND_EXPRESSION_PRIME)**= {`%=` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(ARGUMENT_EXPRESSION_LIST)**= {`)`}


**FOLLOW(ARGUMENT_EXPRESSION_LIST_PRIME)**= {`)`}


**FOLLOW(ASSIGNMENT_EXPRESSION)**= {`)` `,` `:` `;` `]` `}`}


**FOLLOW(ASSIGNMENT_EXPRESSION_FAC)**= {`)` `,` `:` `;` `]` `}`}


**FOLLOW(ASSIGNMENT_EXPRESSION_OPT_FAC)**= {`]`}


**FOLLOW(ASSIGNMENT_OPERATOR)**= {`(` `++` `--` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `sizeof`}


**FOLLOW(BLOCK)**= {`$` `(` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` `else` `float` `for` `func` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(BLOCK_CONTENT)**= {`}`}


**FOLLOW(BLOCK_ITEM)**= {`(` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` `float` `for` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(BLOCK_ITEM_LIST)**= {`}`}


**FOLLOW(BLOCK_ITEM_LIST_PRIME)**= {`}`}


**FOLLOW(CONDITIONAL_EXPRESSION)**= {`%=` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `]` `^=` `|=` `}`}


**FOLLOW(CONDITIONAL_EXPRESSION_PRIME)**= {`%=` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `]` `^=` `|=` `}`}


**FOLLOW(CONSTANT)**= {`!=` `%` `%=` `&` `&&` `&&=` `(` `)` `*` `*=` `+` `++` `+=` `,` `-` `--` `-=` `.` `/` `/=` `:` `;` `<` `<<` `<<=` `<=` `=` `==` `>` `>=` `>>` `>>=` `?` `[` `]` `^` `^=` `|=` `}` `|` `->` `||`}


**FOLLOW(DECLARATION)**= {`$` `(` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` `float` `for` `func` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(DECLARATION_FAC)**= {`$` `(` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` `float` `for` `func` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(DECLARATION_LIST)**= {`{`}


**FOLLOW(DECLARATION_LIST_PRIME)**= {`{`}


**FOLLOW(DECLARATION_SPECIFIERS)**= {`(` `)` `*` `,` `;` `IDENTIFIER` `const` `restrict` `volatile`}


**FOLLOW(DECLARATION_SPECIFIERS_PRIME)**= {`(` `)` `*` `,` `;` `IDENTIFIER` `const` `restrict` `volatile`}


**FOLLOW(DECLARATOR)**= {`)` `,` `;` `=` `bool` `char` `double` `float` `int` `long` `short` `signed` `struct` `unsigned` `void` `{`}


**FOLLOW(DIRECT_DECLARATOR)**= {`)` `,` `;` `=` `bool` `char` `double` `float` `int` `long` `short` `signed` `struct` `unsigned` `void` `{`}


**FOLLOW(DIRECT_DECLARATOR_PRIME)**= {`)` `,` `;` `=` `bool` `char` `double` `float` `int` `long` `short` `signed` `struct` `unsigned` `void` `{`}


**FOLLOW(ELSE_FAC)**= {`(` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` `else` `float` `for` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(EQUALITY_EXPRESSION)**= {`%=` `&` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(EQUALITY_EXPRESSION_PRIME)**= {`%=` `&` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(EXCLUSIVE_OR_EXPRESSION)**= {`%=` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^=` `|=` `}` `|` `||`}


**FOLLOW(EXCLUSIVE_OR_EXPRESSION_PRIME)**= {`%=` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^=` `|=` `}` `|` `||`}


**FOLLOW(EXPRESSION)**= {`)` `:` `;` `]`}


**FOLLOW(EXPRESSION_PRIME)**= {`)` `:` `;` `]`}


**FOLLOW(EXPRESSION_STATEMENT)**= {`(` `)` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` `else` `float` `for` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(FOR_INIT_STATEMENT)**= {`)`}


**FOLLOW(FOR_OPTIONAL_EXPRESSION)**= {`)`}


**FOLLOW(FUNCTION_DEFINITION)**= {`$` `bool` `char` `double` `float` `func` `int` `long` `short` `signed` `struct` `unsigned` `void`}


**FOLLOW(FUNCTION_DEF_FAC)**= {`$` `bool` `char` `double` `float` `func` `int` `long` `short` `signed` `struct` `unsigned` `void`}


**FOLLOW(FUNCTION_SPECIFIER)**= {`(` `)` `*` `,` `;` `IDENTIFIER` `auto` `bool` `char` `const` `double` `float` `inline` `int` `long` `register` `restrict` `short` `signed` `static` `struct` `unsigned` `void` `volatile`}


**FOLLOW(GLOBAL_DECLARATIONS)**= {`$` `bool` `char` `double` `float` `func` `int` `long` `short` `signed` `struct` `unsigned` `void`}


**FOLLOW(IDENTIFIER_LIST)**= {`)`}


**FOLLOW(IDENTIFIER_LIST_PRIME)**= {`)`}


**FOLLOW(INCLUSIVE_OR_EXPRESSION)**= {`%=` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^=` `|=` `}` `||`}


**FOLLOW(INCLUSIVE_OR_EXPRESSION_PRIME)**= {`%=` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^=` `|=` `}` `||`}


**FOLLOW(INITIALIZER)**= {`,` `;` `}`}


**FOLLOW(INITIALIZER_BRACE_FAC)**= {`,` `;` `}`}


**FOLLOW(INITIALIZER_BRACE_FAC2)**= {`}`}


**FOLLOW(INITIALIZER_LIST)**= {`,` `}`}


**FOLLOW(INITIALIZER_LIST_PRIME)**= {`,` `}`}


**FOLLOW(INIT_DECLARATOR)**= {`,` `;`}


**FOLLOW(INIT_DECLARATOR_FAC)**= {`,` `;`}


**FOLLOW(INIT_DECLARATOR_LIST)**= {`;`}


**FOLLOW(INIT_DECLARATOR_LIST_PRIME)**= {`;`}


**FOLLOW(ITERATION_STATEMENT)**= {`(` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` `else` `float` `for` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(JUMP_STATEMENT)**= {`(` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` 
`else` `float` `for` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(LABELED_STATEMENT)**= {`(` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` `else` `float` `for` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(LOGICAL_AND_EXPRESSION)**= {`%=` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^=` `|=` `}` `||`}


**FOLLOW(LOGICAL_AND_EXPRESSION_PRIME)**= {`%=` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^=` `|=` `}` `||`}


**FOLLOW(LOGICAL_OR_EXPRESSION)**= {`%=` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^=` `|=` `}`}


**FOLLOW(LOGICAL_OR_EXPRESSION_PRIME)**= {`%=` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `>>=` `?` `]` `^=` `|=` `}`}


**FOLLOW(MULTIPLICATIVE_EXPRESSION)**= {`!=` `%=` `&` `&&` `&&=` `)` `*=` `+` `+=` `,` `-` `-=` `/=` `:` `;` `<` `<<` `<<=` `<=` `=` `==` `>` `>=` `>>` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(MULTIPLICATIVE_EXPRESSION_PRIME)**= {`!=` `%=` `&` `&&` `&&=` `)` `*=` `+` `+=` `,` `-` `-=` `/=` `:` `;` `<` `<<` `<<=` `<=` `=` `==` `>` `>=` `>>` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(PARAMETER_DECLARATION)**= {`)` `,`}


**FOLLOW(PARAMETER_LIST)**= {`)`}


**FOLLOW(PARAMETER_LIST_OPT_FAC)**= {`)`}


**FOLLOW(PARAMETER_LIST_PRIME)**= {`)`}


**FOLLOW(PARAM_DECL_FAC)**= {`)` `,`}


**FOLLOW(POINTER)**= {`(` `IDENTIFIER`}


**FOLLOW(POSTFIX_ARGUMENTS)**= {`)`}


**FOLLOW(POSTFIX_EXPRESSION)**= {`!=` `%` `%=` `&` `&&` `&&=` `)` `*` `*=` `+` `+=` `,` `-` `-=` `/` `/=` `:` `;` `<` `<<` `<<=` `<=` `=` `==` `>` `>=` `>>` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(POSTFIX_EXPRESSION_PRIME)**= {`!=` `%` `%=` `&` `&&` `&&=` `)` `*` `*=` `+` `+=` `,` `-` `-=` `/` `/=` `:` `;` `<` `<<` `<<=` `<=` `=` `==` `>` `>=` `>>` `>>=` `?` `]` 
`^` `^=` `|=` `}` `|` `||`}


**FOLLOW(PRIMARY_EXPRESSION)**= {`!=` `%` `%=` `&` `&&` `&&=` `(` `)` `*` `*=` `+` `++` `+=` `,` `-` `--` `-=` `.` `/` `/=` `:` `;` `<` `<<` `<<=` `<=` `=` `==` `>` `>=` `>>` `>>=` `?` `[` `]` `^` `^=` `|=` `}` `|` `->` `||`}


**FOLLOW(PROGRAM)**= {`$`}


**FOLLOW(PROGRAM_PRIME)**= {`$`}


**FOLLOW(RELATIONAL_EXPRESSION)**= {`!=` `%=` `&` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `==` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(RELATIONAL_EXPRESSION_PRIME)**= {`!=` `%=` `&` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<<=` `=` `==` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(SELECTION_STATEMENT)**= {`(` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` `else` `float` `for` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(SHIFT_EXPRESSION)**= {`!=` `%=` `&` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<` `<<=` `<=` `=` `==` `>` `>=` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}


**FOLLOW(SHIFT_EXPRESSION_PRIME)**= {`!=` `%=` `&` `&&` `&&=` `)` `*=` `+=` `,` `-=` `/=` `:` `;` `<` `<<=` `<=` `=` `==` `>` `>=` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}     


**FOLLOW(SPECIFIER_QUALIFIER_LIST)**= {`(` `*` `;` `IDENTIFIER` `const` `restrict` `volatile`}


**FOLLOW(SPECIFIER_QUALIFIER_LIST_PRIME)**= {`(` `*` `;` `IDENTIFIER` `const` `restrict` `volatile`}


**FOLLOW(STATEMENT)**= {`(` `++` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `bool` `break` `case` `char` `continue` `default` `do` `double` `else` `float` `for` `if` `int` `long` `return` `short` `signed` `sizeof` `struct` `switch` `unsigned` `void` `while` `{` `}`}


**FOLLOW(STATIC_OPT_FAC)**= {`(` `++` `--` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `]` `const` `restrict` `sizeof` `volatile`}


**FOLLOW(STORAGE_SPECIFIER)**= {`(` `)` `*` `,` `;` `IDENTIFIER` `auto` `bool` `char` `const` `double` `float` `inline` `int` `long` `register` `restrict` `short` `signed` `static` `struct` `unsigned` `void` `volatile`}


**FOLLOW(STRUCT_DECLARATION)**= {`bool` `char` `double` `float` `int` `long` `short` `signed` `struct` `unsigned` `void` `}`}


**FOLLOW(STRUCT_DECLARATION_LIST)**= {`}`}


**FOLLOW(STRUCT_DECLARATION_LIST_PRIME)**= {`}`}


**FOLLOW(STRUCT_DECLARATOR_LIST)**= {`;`}


**FOLLOW(STRUCT_DECLARATOR_LIST_PRIME)**= {`;`}


**FOLLOW(STRUCT_DECL_FAC)**= {`bool` `char` `double` `float` `int` `long` `short` `signed` `struct` `unsigned` `void` `}`}


**FOLLOW(STRUCT_SPECIFIER)**= {`(` `)` `*` `,` `;` `IDENTIFIER` `auto` `bool` `char` `const` `double` `float` `inline` `int` `long` `register` `restrict` `short` `signed` `static` `struct` `unsigned` `void` `volatile`}


**FOLLOW(STRUCT_SPEC_FAC)**= {`(` `)` `*` `,` `;` `IDENTIFIER` `auto` `bool` `char` `const` `double` `float` `inline` `int` `long` `register` `restrict` `short` `signed` `static` `struct` `unsigned` `void` `volatile`}


**FOLLOW(STRUCT_SPEC_FAC2)**= {`(` `)` `*` `,` `;` `IDENTIFIER` `auto` `bool` `char` `const` `double` `float` `inline` `int` `long` `register` `restrict` `short` `signed` `static` `struct` `unsigned` `void` `volatile`}


**FOLLOW(TYPE_QUALIFIER)**= {`(` `)` `*` `++` `,` `--` `;` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `]` `auto` `bool` `char` `const` `double` `float` `inline` `int` `long` `register` `restrict` `short` `signed` `sizeof` `static` `struct` `unsigned` `void` `volatile`}


**FOLLOW(TYPE_QUALIFIER_LIST)**= {`(` `*` `++` `--` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `]` `const` `restrict` `sizeof` `volatile`}


**FOLLOW(TYPE_QUALIFIER_LIST_OPT_FAC)**= {`(` `++` `--` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `]` `sizeof`}


**FOLLOW(TYPE_QUALIFIER_LIST_PRIME)**= {`(` `*` `++` `--` `FLOAT_CONSTANT` `IDENTIFIER` `INTEGER_CONSTANT` `STRING_LITERAL` `]` `const` `restrict` `sizeof` `volatile`}


**FOLLOW(TYPE_SPECIFIER)**= {`(` `)` `*` `,` `;` `IDENTIFIER` `auto` `bool` `char` `const` `double` `float` `inline` `int` `long` `register` `restrict` `short` `signed` `static` `struct` `unsigned` `void` `volatile`}


**FOLLOW(UNARY_EXPRESSION)**= {`!=` `%` `%=` `&` `&&` `&&=` `)` `*` `*=` `+` `+=` `,` `-` `-=` `/` `/=` `:` `;` `<` `<<` `<<=` `<=` `=` `==` `>` `>=` `>>` `>>=` `?` `]` `^` `^=` `|=` `}` `|` `||`}