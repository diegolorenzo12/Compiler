# CFG Without Left Recursion

This document extends the [CFG](CFG.md) file by removing the left recursion from the grammar rules.

## Grammar Rules without left recursion

<a id="PROGRAM"></a>
**PROGRAM → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[GLOBAL_DECLARATIONS](#GLOBAL_DECLARATIONS)**

<a id="GLOBAL_DECLARATIONS"></a>
**GLOBAL_DECLARATIONS → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION](#DECLARATION) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[FUNCTION_DEFINITION](#FUNCTION_DEFINITION)**

<a id="DECLARATION_LIST"></a>
**DECLARATION_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION](#DECLARATION) [DECLARATION_LIST_PRIME](#DECLARATION_LIST_PRIME)**

<a id="DECLARATION_LIST_PRIME"></a>
**DECLARATION_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION](#DECLARATION) [DECLARATION_LIST_PRIME](#DECLARATION_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="DECLARATION"></a>
**DECLARATION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#DECLARATION_SPECIFIERS)  ;  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#DECLARATION_SPECIFIERS) [INIT_DECLARATOR_LIST](#INIT_DECLARATOR_LIST)  ; **

<a id="DECLARATION_SPECIFIERS"></a>
**DECLARATION_SPECIFIERS → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_SPECIFIER](#TYPE_SPECIFIER) [DECLARATION_SPECIFIERS_PRIME](#DECLARATION_SPECIFIERS_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_QUALIFIER](#TYPE_QUALIFIER) [DECLARATION_SPECIFIERS_PRIME](#DECLARATION_SPECIFIERS_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[FUNCTION_SPECIFIER](#FUNCTION_SPECIFIER) [DECLARATION_SPECIFIERS_PRIME](#DECLARATION_SPECIFIERS_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[STORAGE_SPECIFIER](#STORAGE_SPECIFIER) [DECLARATION_SPECIFIERS_PRIME](#DECLARATION_SPECIFIERS_PRIME)**

<a id="DECLARATION_SPECIFIERS_PRIME"></a>
**DECLARATION_SPECIFIERS_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#DECLARATION_SPECIFIERS) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="INIT_DECLARATOR_LIST"></a>
**INIT_DECLARATOR_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[INIT_DECLARATOR](#INIT_DECLARATOR) [INIT_DECLARATOR_LIST_PRIME](#INIT_DECLARATOR_LIST_PRIME)**

<a id="INIT_DECLARATOR_LIST_PRIME"></a>
**INIT_DECLARATOR_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ,  [INIT_DECLARATOR](#INIT_DECLARATOR) [INIT_DECLARATOR_LIST_PRIME](#INIT_DECLARATOR_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="INIT_DECLARATOR"></a>
**INIT_DECLARATOR → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATOR](#DECLARATOR)  =  [INITIALIZER](#INITIALIZER) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATOR](#DECLARATOR)**

<a id="STORAGE_SPECIFIER"></a>
**STORAGE_SPECIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;typedef |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;static |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;auto |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;register**

<a id="TYPE_QUALIFIER"></a>
**TYPE_QUALIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;const |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;restrict |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;volatile**

<a id="TYPE_QUALIFIER_LIST"></a>
**TYPE_QUALIFIER_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_QUALIFIER](#TYPE_QUALIFIER) [TYPE_QUALIFIER_LIST_PRIME](#TYPE_QUALIFIER_LIST_PRIME)**

<a id="TYPE_QUALIFIER_LIST_PRIME"></a>
**TYPE_QUALIFIER_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_QUALIFIER](#TYPE_QUALIFIER) [TYPE_QUALIFIER_LIST_PRIME](#TYPE_QUALIFIER_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="FUNCTION_SPECIFIER"></a>
**FUNCTION_SPECIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;inline**

<a id="TYPE_SPECIFIER"></a>
**TYPE_SPECIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;short |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;long |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;float |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;double |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;signed |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unsigned |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;bool |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[STRUCT_SPECIFIER](#STRUCT_SPECIFIER) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ENUM_SPECIFIER](#ENUM_SPECIFIER) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPEDEF_NAME](#TYPEDEF_NAME)**

<a id="FUNCTION_DEFINITION"></a>
**FUNCTION_DEFINITION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#DECLARATION_SPECIFIERS) [DECLARATOR](#DECLARATOR) [DECLARATION_LIST](#DECLARATION_LIST) [BLOCK](#BLOCK) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#DECLARATION_SPECIFIERS) [DECLARATOR](#DECLARATOR) [BLOCK](#BLOCK)**

<a id="STRUCT_SPECIFIER"></a>
**STRUCT_SPECIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;struct  {  [STRUCT_DECLARATION_LIST](#STRUCT_DECLARATION_LIST)  }  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;struct IDENTIFIER  {  [STRUCT_DECLARATION_LIST](#STRUCT_DECLARATION_LIST)  }  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;struct IDENTIFIER**

<a id="STRUCT_DECLARATION_LIST"></a>
**STRUCT_DECLARATION_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[STRUCT_DECLARATION](#STRUCT_DECLARATION) [STRUCT_DECLARATION_LIST_PRIME](#STRUCT_DECLARATION_LIST_PRIME)**

<a id="STRUCT_DECLARATION_LIST_PRIME"></a>
**STRUCT_DECLARATION_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[STRUCT_DECLARATION](#STRUCT_DECLARATION) [STRUCT_DECLARATION_LIST_PRIME](#STRUCT_DECLARATION_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="STRUCT_DECLARATION"></a>
**STRUCT_DECLARATION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[SPECIFIER_QUALIFIER_LIST](#SPECIFIER_QUALIFIER_LIST)  ;  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[SPECIFIER_QUALIFIER_LIST](#SPECIFIER_QUALIFIER_LIST) [STRUCT_DECLARATOR_LIST](#STRUCT_DECLARATOR_LIST)  ;**

<a id="SPECIFIER_QUALIFIER_LIST"></a>
**SPECIFIER_QUALIFIER_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_SPECIFIER](#TYPE_SPECIFIER) [SPECIFIER_QUALIFIER_LIST_PRIME](#SPECIFIER_QUALIFIER_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_QUALIFIER](#TYPE_QUALIFIER) [SPECIFIER_QUALIFIER_LIST_PRIME](#SPECIFIER_QUALIFIER_LIST_PRIME)**

<a id="SPECIFIER_QUALIFIER_LIST_PRIME"></a>
**SPECIFIER_QUALIFIER_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[SPECIFIER_QUALIFIER_LIST](#SPECIFIER_QUALIFIER_LIST) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="STRUCT_DECLARATOR_LIST"></a>
**STRUCT_DECLARATOR_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATOR](#DECLARATOR) [STRUCT_DECLARATOR_LIST_PRIME](#STRUCT_DECLARATOR_LIST_PRIME)**

<a id="STRUCT_DECLARATOR_LIST_PRIME"></a>
**STRUCT_DECLARATOR_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ,  [DECLARATOR](#DECLARATOR) [STRUCT_DECLARATOR_LIST_PRIME](#STRUCT_DECLARATOR_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="ENUM_SPECIFIER"></a>
**ENUM_SPECIFIER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;enum  {  [ENUMERATOR_LIST](#ENUMERATOR_LIST)  }  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;enum  {  [ENUMERATOR_LIST](#ENUMERATOR_LIST)  ,   }  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;enum IDENTIFIER  {  [ENUMERATOR_LIST](#ENUMERATOR_LIST)  }  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;enum IDENTIFIER  {  [ENUMERATOR_LIST](#ENUMERATOR_LIST)  ,   }  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;enum IDENTIFIER**

<a id="ENUMERATOR_LIST"></a>
**ENUMERATOR_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ENUMERATOR](#ENUMERATOR) [ENUMERATOR_LIST_PRIME](#ENUMERATOR_LIST_PRIME)**

<a id="ENUMERATOR_LIST_PRIME"></a>
**ENUMERATOR_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ,  [ENUMERATOR](#ENUMERATOR) [ENUMERATOR_LIST_PRIME](#ENUMERATOR_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="ENUMERATOR"></a>
**ENUMERATOR → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER  =  IDENTIFIER |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER**

<a id="DECLARATOR"></a>
**DECLARATOR → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[POINTER](#POINTER) [DIRECT_DECLARATOR](#DIRECT_DECLARATOR) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DIRECT_DECLARATOR](#DIRECT_DECLARATOR)**

<a id="DIRECT_DECLARATOR"></a>
**DIRECT_DECLARATOR → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; (  [DECLARATOR](#DECLARATOR)  )  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [   ]  [DIRECT_DECLARATOR_PRIME](#DIRECT_DECLARATOR_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; (  [PARAMETER_LIST](#PARAMETER_LIST)  )  [DIRECT_DECLARATOR_PRIME](#DIRECT_DECLARATOR_PRIME)**

<a id="DIRECT_DECLARATOR_PRIME"></a>
**DIRECT_DECLARATOR_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [   ]  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [  [TYPE_QUALIFIER_LIST](#TYPE_QUALIFIER_LIST) static [ASSIGNMENT_EXPRESSION](#ASSIGNMENT_EXPRESSION)  ]  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [  static [ASSIGNMENT_EXPRESSION](#ASSIGNMENT_EXPRESSION)  ]  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [  [TYPE_QUALIFIER_LIST](#TYPE_QUALIFIER_LIST)  ]  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [  [ASSIGNMENT_EXPRESSION](#ASSIGNMENT_EXPRESSION)  ]  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; (  [IDENTIFIER_LIST](#IDENTIFIER_LIST)  )  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="POINTER"></a>
**POINTER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[TYPE_QUALIFIER_LIST](#TYPE_QUALIFIER_LIST) [POINTER_PRIME](#POINTER_PRIME)**

<a id="POINTER_PRIME"></a>
**POINTER_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *  [POINTER_PRIME](#POINTER_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="PARAMETER_LIST"></a>
**PARAMETER_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[PARAMETER_DECLARATION](#PARAMETER_DECLARATION) [PARAMETER_LIST_PRIME](#PARAMETER_LIST_PRIME)**

<a id="PARAMETER_LIST_PRIME"></a>
**PARAMETER_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ,  [PARAMETER_DECLARATION](#PARAMETER_DECLARATION) [PARAMETER_LIST_PRIME](#PARAMETER_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="PARAMETER_DECLARATION"></a>
**PARAMETER_DECLARATION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#DECLARATION_SPECIFIERS) [DECLARATOR](#DECLARATOR) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#DECLARATION_SPECIFIERS) [ABSTRACT_DECLARATOR](#ABSTRACT_DECLARATOR) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION_SPECIFIERS](#DECLARATION_SPECIFIERS)**

<a id="IDENTIFIER_LIST"></a>
**IDENTIFIER_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER [IDENTIFIER_LIST_PRIME](#IDENTIFIER_LIST_PRIME)**

<a id="IDENTIFIER_LIST_PRIME"></a>
**IDENTIFIER_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ,  IDENTIFIER [IDENTIFIER_LIST_PRIME](#IDENTIFIER_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="INITIALIZER"></a>
**INITIALIZER → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; {  [INITIALIZER_LIST](#INITIALIZER_LIST)  }  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; {  [INITIALIZER_LIST](#INITIALIZER_LIST)  ,   }  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ASSIGNMENT_EXPRESSION](#ASSIGNMENT_EXPRESSION)**

<a id="INITIALIZER_LIST"></a>
**INITIALIZER_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[INITIALIZER](#INITIALIZER) [INITIALIZER_LIST_PRIME](#INITIALIZER_LIST_PRIME)**

<a id="INITIALIZER_LIST_PRIME"></a>
**INITIALIZER_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ,  [INITIALIZER](#INITIALIZER) [INITIALIZER_LIST_PRIME](#INITIALIZER_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="STATEMENT"></a>
**STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[LABELED_STATEMENT](#LABELED_STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[BLOCK](#BLOCK) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[EXPRESSION_STATEMENT](#EXPRESSION_STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[SELECTION_STATEMENT](#SELECTION_STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ITERATION_STATEMENT](#ITERATION_STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[JUMP_STATEMENT](#JUMP_STATEMENT)**

<a id="LABELED_STATEMENT"></a>
**LABELED_STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER  :  [STATEMENT](#STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;case [CONDITIONAL_EXPRESSION](#CONDITIONAL_EXPRESSION)  :  [STATEMENT](#STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;default  :  [STATEMENT](#STATEMENT)**

<a id="BLOCK"></a>
**BLOCK → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; {   }  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; {  [BLOCK_ITEM_LIST](#BLOCK_ITEM_LIST)  } **

<a id="BLOCK_ITEM"></a>
**BLOCK_ITEM → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[DECLARATION](#DECLARATION) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[STATEMENT](#STATEMENT)**

<a id="BLOCK_ITEM_LIST"></a>
**BLOCK_ITEM_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[BLOCK_ITEM](#BLOCK_ITEM) [BLOCK_ITEM_LIST_PRIME](#BLOCK_ITEM_LIST_PRIME)**

<a id="BLOCK_ITEM_LIST_PRIME"></a>
**BLOCK_ITEM_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[BLOCK_ITEM](#BLOCK_ITEM) [BLOCK_ITEM_LIST_PRIME](#BLOCK_ITEM_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="EXPRESSION_STATEMENT"></a>
**EXPRESSION_STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ;  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[EXPRESSION](#EXPRESSION)  ;**

<a id="EXPRESSION"></a>
**EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ASSIGNMENT_EXPRESSION](#ASSIGNMENT_EXPRESSION) [EXPRESSION_PRIME](#EXPRESSION_PRIME)**

<a id="EXPRESSION_PRIME"></a>
**EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ,  [ASSIGNMENT_EXPRESSION](#ASSIGNMENT_EXPRESSION) [EXPRESSION_PRIME](#EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="ASSIGNMENT_EXPRESSION"></a>
**ASSIGNMENT_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[CONDITIONAL_EXPRESSION](#CONDITIONAL_EXPRESSION) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[UNARY_EXPRESSION](#UNARY_EXPRESSION) [ASSIGNMENT_OPERATOR](#ASSIGNMENT_OPERATOR) [ASSIGNMENT_EXPRESSION](#ASSIGNMENT_EXPRESSION)**

<a id="SELECTION_STATEMENT"></a>
**SELECTION_STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;if  (  [EXPRESSION](#EXPRESSION)  )  [STATEMENT](#STATEMENT) else [STATEMENT](#STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;if  (  [EXPRESSION](#EXPRESSION)  )  [STATEMENT](#STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;switch  (  [EXPRESSION](#EXPRESSION)  )  [STATEMENT](#STATEMENT)**

<a id="ITERATION_STATEMENT"></a>
**ITERATION_STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;while  (  [EXPRESSION](#EXPRESSION)  )  [STATEMENT](#STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;do [STATEMENT](#STATEMENT) while  (  [EXPRESSION](#EXPRESSION)  )   ;  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;for  (  [EXPRESSION_STATEMENT](#EXPRESSION_STATEMENT) [EXPRESSION_STATEMENT](#EXPRESSION_STATEMENT)  )  [STATEMENT](#STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;for  (  [EXPRESSION_STATEMENT](#EXPRESSION_STATEMENT) [EXPRESSION_STATEMENT](#EXPRESSION_STATEMENT) [EXPRESSION](#EXPRESSION)  )  [STATEMENT](#STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;for  (  [DECLARATION](#DECLARATION) [EXPRESSION_STATEMENT](#EXPRESSION_STATEMENT)  )  [STATEMENT](#STATEMENT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;for  (  [DECLARATION](#DECLARATION) [EXPRESSION_STATEMENT](#EXPRESSION_STATEMENT) [EXPRESSION](#EXPRESSION)  )  [STATEMENT](#STATEMENT)**

<a id="JUMP_STATEMENT"></a>
**JUMP_STATEMENT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;continue  ;  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;break  ;  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return  ;  |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return [EXPRESSION](#EXPRESSION)  ;**

<a id="ASSIGNMENT_OPERATOR"></a>
**ASSIGNMENT_OPERATOR → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;= |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;*= |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;/= |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;%= |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;+= |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-= |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&lt;&lt;= |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&gt;&gt;= |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&&= |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;^= |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|=**

<a id="CONDITIONAL_EXPRESSION"></a>
**CONDITIONAL_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[LOGICAL_OR_EXPRESSION](#LOGICAL_OR_EXPRESSION) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[LOGICAL_OR_EXPRESSION](#LOGICAL_OR_EXPRESSION)  ?  [EXPRESSION](#EXPRESSION)  :  [CONDITIONAL_EXPRESSION](#CONDITIONAL_EXPRESSION)**

<a id="LOGICAL_OR_EXPRESSION"></a>
**LOGICAL_OR_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[LOGICAL_AND_EXPRESSION](#LOGICAL_AND_EXPRESSION) [LOGICAL_OR_EXPRESSION_PRIME](#LOGICAL_OR_EXPRESSION_PRIME)**

<a id="LOGICAL_OR_EXPRESSION_PRIME"></a>
**LOGICAL_OR_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ||  [LOGICAL_AND_EXPRESSION](#LOGICAL_AND_EXPRESSION) [LOGICAL_OR_EXPRESSION_PRIME](#LOGICAL_OR_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="LOGICAL_AND_EXPRESSION"></a>
**LOGICAL_AND_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[INCLUSIVE_OR_EXPRESSION](#INCLUSIVE_OR_EXPRESSION) [LOGICAL_AND_EXPRESSION_PRIME](#LOGICAL_AND_EXPRESSION_PRIME)**

<a id="LOGICAL_AND_EXPRESSION_PRIME"></a>
**LOGICAL_AND_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &&  [INCLUSIVE_OR_EXPRESSION](#INCLUSIVE_OR_EXPRESSION) [LOGICAL_AND_EXPRESSION_PRIME](#LOGICAL_AND_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="INCLUSIVE_OR_EXPRESSION"></a>
**INCLUSIVE_OR_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[EXCLUSIVE_OR_EXPRESSION](#EXCLUSIVE_OR_EXPRESSION) [INCLUSIVE_OR_EXPRESSION_PRIME](#INCLUSIVE_OR_EXPRESSION_PRIME)**

<a id="INCLUSIVE_OR_EXPRESSION_PRIME"></a>
**INCLUSIVE_OR_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; |  [EXCLUSIVE_OR_EXPRESSION](#EXCLUSIVE_OR_EXPRESSION) [INCLUSIVE_OR_EXPRESSION_PRIME](#INCLUSIVE_OR_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="EXCLUSIVE_OR_EXPRESSION"></a>
**EXCLUSIVE_OR_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[AND_EXPRESSION](#AND_EXPRESSION) [EXCLUSIVE_OR_EXPRESSION_PRIME](#EXCLUSIVE_OR_EXPRESSION_PRIME)**

<a id="EXCLUSIVE_OR_EXPRESSION_PRIME"></a>
**EXCLUSIVE_OR_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ^  [AND_EXPRESSION](#AND_EXPRESSION) [EXCLUSIVE_OR_EXPRESSION_PRIME](#EXCLUSIVE_OR_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="AND_EXPRESSION"></a>
**AND_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[EQUALITY_EXPRESSION](#EQUALITY_EXPRESSION) [AND_EXPRESSION_PRIME](#AND_EXPRESSION_PRIME)**

<a id="AND_EXPRESSION_PRIME"></a>
**AND_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; &  [EQUALITY_EXPRESSION](#EQUALITY_EXPRESSION) [AND_EXPRESSION_PRIME](#AND_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="EQUALITY_EXPRESSION"></a>
**EQUALITY_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[RELATIONAL_EXPRESSION](#RELATIONAL_EXPRESSION) [EQUALITY_EXPRESSION_PRIME](#EQUALITY_EXPRESSION_PRIME)**

<a id="EQUALITY_EXPRESSION_PRIME"></a>
**EQUALITY_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ==  [RELATIONAL_EXPRESSION](#RELATIONAL_EXPRESSION) [EQUALITY_EXPRESSION_PRIME](#EQUALITY_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; !=  [RELATIONAL_EXPRESSION](#RELATIONAL_EXPRESSION) [EQUALITY_EXPRESSION_PRIME](#EQUALITY_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="RELATIONAL_EXPRESSION"></a>
**RELATIONAL_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[SHIFT_EXPRESSION](#SHIFT_EXPRESSION) [RELATIONAL_EXPRESSION_PRIME](#RELATIONAL_EXPRESSION_PRIME)**

<a id="RELATIONAL_EXPRESSION_PRIME"></a>
**RELATIONAL_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <  [SHIFT_EXPRESSION](#SHIFT_EXPRESSION) [RELATIONAL_EXPRESSION_PRIME](#RELATIONAL_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; >  [SHIFT_EXPRESSION](#SHIFT_EXPRESSION) [RELATIONAL_EXPRESSION_PRIME](#RELATIONAL_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <=  [SHIFT_EXPRESSION](#SHIFT_EXPRESSION) [RELATIONAL_EXPRESSION_PRIME](#RELATIONAL_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; > = [SHIFT_EXPRESSION](#SHIFT_EXPRESSION) [RELATIONAL_EXPRESSION_PRIME](#RELATIONAL_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="SHIFT_EXPRESSION"></a>
**SHIFT_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ADDITIVE_EXPRESSION](#ADDITIVE_EXPRESSION) [SHIFT_EXPRESSION_PRIME](#SHIFT_EXPRESSION_PRIME)**

<a id="SHIFT_EXPRESSION_PRIME"></a>
**SHIFT_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <<  [ADDITIVE_EXPRESSION](#ADDITIVE_EXPRESSION) [SHIFT_EXPRESSION_PRIME](#SHIFT_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; >>  [ADDITIVE_EXPRESSION](#ADDITIVE_EXPRESSION) [SHIFT_EXPRESSION_PRIME](#SHIFT_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="ADDITIVE_EXPRESSION"></a>
**ADDITIVE_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[MULTIPLICATIVE_EXPRESSION](#MULTIPLICATIVE_EXPRESSION) [ADDITIVE_EXPRESSION_PRIME](#ADDITIVE_EXPRESSION_PRIME)**

<a id="ADDITIVE_EXPRESSION_PRIME"></a>
**ADDITIVE_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; +  [MULTIPLICATIVE_EXPRESSION](#MULTIPLICATIVE_EXPRESSION) [ADDITIVE_EXPRESSION_PRIME](#ADDITIVE_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -  [MULTIPLICATIVE_EXPRESSION](#MULTIPLICATIVE_EXPRESSION) [ADDITIVE_EXPRESSION_PRIME](#ADDITIVE_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="MULTIPLICATIVE_EXPRESSION"></a>
**MULTIPLICATIVE_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[UNARY_EXPRESSION](#UNARY_EXPRESSION) [MULTIPLICATIVE_EXPRESSION_PRIME](#MULTIPLICATIVE_EXPRESSION_PRIME)**

<a id="MULTIPLICATIVE_EXPRESSION_PRIME"></a>
**MULTIPLICATIVE_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; *  [UNARY_EXPRESSION](#UNARY_EXPRESSION) [MULTIPLICATIVE_EXPRESSION_PRIME](#MULTIPLICATIVE_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; /  [UNARY_EXPRESSION](#UNARY_EXPRESSION) [MULTIPLICATIVE_EXPRESSION_PRIME](#MULTIPLICATIVE_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; %  [UNARY_EXPRESSION](#UNARY_EXPRESSION) [MULTIPLICATIVE_EXPRESSION_PRIME](#MULTIPLICATIVE_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="UNARY_EXPRESSION"></a>
**UNARY_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[POSTFIX_EXPRESSION](#POSTFIX_EXPRESSION) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ++  [UNARY_EXPRESSION](#UNARY_EXPRESSION) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --  [UNARY_EXPRESSION](#UNARY_EXPRESSION) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; sizeof  [UNARY_EXPRESSION](#UNARY_EXPRESSION)**

<a id="POSTFIX_EXPRESSION"></a>
**POSTFIX_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[PRIMARY_EXPRESSION](#PRIMARY_EXPRESSION) [POSTFIX_EXPRESSION_PRIME](#POSTFIX_EXPRESSION_PRIME)**

<a id="POSTFIX_EXPRESSION_PRIME"></a>
**POSTFIX_EXPRESSION_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; [  [EXPRESSION](#EXPRESSION)  ]  [POSTFIX_EXPRESSION_PRIME](#POSTFIX_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; (   )  [POSTFIX_EXPRESSION_PRIME](#POSTFIX_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; (  [ARGUMENT_EXPRESSION_LIST](#ARGUMENT_EXPRESSION_LIST)  )  [POSTFIX_EXPRESSION_PRIME](#POSTFIX_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; .  IDENTIFIER [POSTFIX_EXPRESSION_PRIME](#POSTFIX_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ->  IDENTIFIER [POSTFIX_EXPRESSION_PRIME](#POSTFIX_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ++  [POSTFIX_EXPRESSION_PRIME](#POSTFIX_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --  [POSTFIX_EXPRESSION_PRIME](#POSTFIX_EXPRESSION_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="ARGUMENT_EXPRESSION_LIST"></a>
**ARGUMENT_EXPRESSION_LIST → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[ASSIGNMENT_EXPRESSION](#ASSIGNMENT_EXPRESSION) [ARGUMENT_EXPRESSION_LIST_PRIME](#ARGUMENT_EXPRESSION_LIST_PRIME)**

<a id="ARGUMENT_EXPRESSION_LIST_PRIME"></a>
**ARGUMENT_EXPRESSION_LIST_PRIME → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ,  [ASSIGNMENT_EXPRESSION](#ASSIGNMENT_EXPRESSION) [ARGUMENT_EXPRESSION_LIST_PRIME](#ARGUMENT_EXPRESSION_LIST_PRIME) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;ε**

<a id="PRIMARY_EXPRESSION"></a>
**PRIMARY_EXPRESSION → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;[CONSTANT](#CONSTANT) |<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; (  [EXPRESSION](#EXPRESSION)  )**

<a id="CONSTANT"></a>
**CONSTANT → <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;INTEGER_CONSTANT | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;FLOAT_CONSTANT | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;STRING_LITERAL | <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;IDENTIFIER**

