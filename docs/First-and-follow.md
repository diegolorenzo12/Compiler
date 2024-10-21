FIRST(PROGRAM) = FIRST(GLOBAL_DECLARATIONS) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register}

FIRST(GLOBAL_DECLARATIONS) = FIRST(DECLARATION) U FIRST(FUNCTION_DEFINITION) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register}

FIRST(DECLARATION_LIST) = FIRST(DECLARATION) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register}

FIRST(DECLARATION_LIST_PRIME) = FIRST(DECLARATION) U ε = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register  ε} 

FIRST(DECLARATION) = FIRST(DECLARATION_SPECIFIERS) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register}

FIRST(DECLARATION_SPECIFIERS) = FIRST(TYPE_SPECIFIER) U FIRST(TYPE_QUALIFIER) U FIRST(FUNCTION_SPECIFIER) U FIRST STORAGE_SPECIFIER) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register}

FIRST(DECLARATION_SPECIFIERS_PRIME) = FIRST(DECLARATION_SPECIFIERS) U ε = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register  ε}

FIRST(INIT_DECLARATOR_LIST) = FIRST(INIT_DECLARATOR) = {const  restrict  volatile  identifier  (  [}

FIRST(INIT_DECLARATOR_LIST_PRIME) = {,  ε}

FIRST(INIT_DECLARATOR) = FIRST(DECLARATOR) = {const  restrict  volatile  identifier  (  [}

FIRST(STORAGE_SPECIFIER) = {typedef  static  auto  register}

FIRST(TYPE_QUALIFIER) = {const  restrict  volatile}

FIRST(TYPE_QUALIFIER_LIST) = FIRST(TYPE_QUALIFIER) = {const  restrict  volatile}

FIRST(TYPE_QUALIFIER_LIST_PRIME) = FIRST(TYPE_QUALIFIER) U ε = {const  restrict  volatile  ε}

FIRST(FUNCTION_SPECIFIER) = {inline}

FIRST(TYPE_SPECIFIER) = int void char short long float double signed unsigned bool U FIRST(STRUCT_SPECIFIER) U FIRST(ENUM_SPECIFIER) U FIRST(TYPEDEF_NAME) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum}

FIRST(FUNCTION_DEFINITION) = FIRST(DECLARATION_SPECIFIERS) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register}

FIRST(STRUCT_SPECIFIER) = {struct}

FIRST(STRUCT_DECLARATION_LIST) = FIRST(STRUCT_DECLARATION) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile}

FIRST(STRUCT_DECLARATION_LIST_PRIME) = FIRST(STRUCT_DECLARATION) U ε = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  ε}

FIRST(STRUCT_DECLARATION) = FIRST(SPECIFIER_QUALIFIER_LIST) =  {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile}

FIRST(SPECIFIER_QUALIFIER_LIST) = FIRST(TYPE_SPECIFIER) U FIRST(TYPE_QUALIFIER) =  {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile}

FIRST(SPECIFIER_QUALIFIER_LIST_PRIME) = FIRST(SPECIFIER_QUALIFIER_LIST) U ε = 

FIRST(STRUCT_DECLARATOR_LIST) = FIRST(DECLARATOR) = {const  restrict  volatile  identifier  (  [}

FIRST(STRUCT_DECLARATOR_LIST_PRIME) = {,  ε}

FIRST(ENUM_SPECIFIER) = {enum}

FIRST(ENUMERATOR_LIST) = {identifier}

FIRST(ENUMERATOR_LIST_PRIME) = {,  ε}

FIRST(ENUMERATOR) = {identifier}

FIRST(DECLARATOR) = FIRST(POINTER) U FIRST(DIRECT_DECLARATOR)= {const  restrict  volatile  identifier  (  [}

FIRST(DIRECT_DECLARATOR) = {identifier  (  [}

FIRST(DIRECT_DECLARATOR_PRIME) = {  [  (  ε}

FIRST(POINTER) = FIRST(TYPE_QUALIFIER_LIST) = {const  restrict  volatile}

FIRST(POINTER_PRIME) = {*  ε}

FIRST(PARAMETER_LIST) = FIRST(PARAMETER_DECLARATION) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register}

FIRST(PARAMETER_LIST_PRIME) = {,  ε}  

FIRST(PARAMETER_DECLARATION) = FIRST(DECLARATION_SPECIFIERS) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register}

FIRST(IDENTIFIER_LIST) = {identifier}

FIRST(IDENTIFIER_LIST_PRIME) = {,  ε}

FIRST(INITIALIZER) = { U FIRST(ASSIGNMENT_EXPRESSION) =  {  {  identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(INITIALIZER_LIST) = FIRST(INITIALIZER) = {  {  identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(INITIALIZER_LIST_PRIME) = {,  ε}

FIRST(STATEMENT) = FIRST( LABELED_STATEMENT) U FIRST(BLOCK) U FIRST(EXPRESSION_STATEMENT) U FIRST(SELECTION_STATEMENT) U FIRST(ITERATION_STATEMENT) U FIRST(JUMP_STATEMENT) = {identifier  case  default  {  ;  integer_constant  float_constant  string_literal  (  ++  –  sizeof  if  switch  while  do  for  continue  break  return} 

FIRST(LABELED_STATEMENT) = {IDENTIFIER  case  default}

FIRST(BLOCK) = {  {  }

FIRST(BLOCK_ITEM) = FIRST(DECLARATION) U FIRST(STATEMENT) = {int  void  char  short  long  float  double  signed  unsigned  bool  struct  enum  const  restrict  volatile  inline  typedef  static  auto  register  identifier  case  default  {  ;  integer_constant  float_constant  string_literal  (  ++  –  sizeof  if  switch  while  do  for  continue  break  return}

FIRST(BLOCK_ITEM_LIST) = FIRST(BLOCK_ITEM)

FIRST(BLOCK_ITEM_LIST_PRIME) = FIRST(BLOCK_ITEM) U ε

FIRST(EXPRESSION_STATEMENT) = ;  U FIRST(EXPRESSION) = {;  identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(EXPRESSION) = FIRST(ASSIGNMENT_EXPRESSION)= {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(EXPRESSION_PRIME) = {,  ε}

FIRST(ASSIGNMENT_EXPRESSION) = FIRST(CONDITIONAL_EXPRESSION) U 
FIRST(UNARY_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(SELECTION_STATEMENT) = {if  switch}

FIRST(ITERATION_STATEMENT) = {while  do  for}

FIRST(JUMP_STATEMENT) = {continue  break  return}

FIRST(ASSIGNMENT_OPERATOR) = {=  *=  /=  %=  +=  -=  <<=  >>=  &&=  ^=  |=

FIRST(CONDITIONAL_EXPRESSION) = FIRST(LOGICAL_OR_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(LOGICAL_OR_EXPRESSION) = FIRST(LOGICAL_AND_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(LOGICAL_OR_EXPRESSION_PRIME) = {||  ε}

FIRST(LOGICAL_AND_EXPRESSION) = FIRST(INCLUSIVE_OR_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(LOGICAL_AND_EXPRESSION_PRIME) = {&&  ε}

FIRST(INCLUSIVE_OR_EXPRESSION) = FIRST(EXCLUSIVE_OR_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(INCLUSIVE_OR_EXPRESSION_PRIME) = {{identifier  
integer_constant  float_constant  string_literal  (  ++  –  sizeof  ε}

FIRST(EXCLUSIVE_OR_EXPRESSION) = FIRST(AND_EXPRESSION ) = {identifier  
integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(EXCLUSIVE_OR_EXPRESSION_PRIME) = {^   ε}

FIRST(AND_EXPRESSION) = FIRST(EQUALITY_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(AND_EXPRESSION_PRIME) = {&   ε}

FIRST(EQUALITY_EXPRESSION) = FIRST(RELATIONAL_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(EQUALITY_EXPRESSION_PRIME) = {==  !=   ε}

FIRST(RELATIONAL_EXPRESSION) = FIRST(SHIFT_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(RELATIONAL_EXPRESSION_PRIME) = {<  >  <=  >=   ε}

FIRST(SHIFT_EXPRESSION) = FIRST(ADDITIVE_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(SHIFT_EXPRESSION_PRIME) = {<<  >>  ε}

FIRST(ADDITIVE_EXPRESSION) = FIRST ( MULTIPLICATIVE_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(ADDITIVE_EXPRESSION_PRIME) = {+  -  ε}

FIRST(MULTIPLICATIVE_EXPRESSION) = FIRST(UNARY_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(MULTIPLICATIVE_EXPRESSION_PRIME) = {*  /  %  ε}

FIRST(UNARY_EXPRESSION) = FIRST (POSTFIX_EXPRESSION) U  ++ U -- U sizeof = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(POSTFIX_EXPRESSION) = FIRST( PRIMARY_EXPRESSION ) = {identifier  integer_constant  float_constant  string_literal  (}

FIRST(POSTFIX_EXPRESSION_PRIME) = { [  (  .  ->  ++  --  ε}

FIRST(ARGUMENT_EXPRESSION_LIST) = FIRST(ASSIGNMENT_EXPRESSION) = FIRST(CONDITIONAL_EXPRESSION) U FIRST(UNARY_EXPRESSION) = {identifier  integer_constant  float_constant  string_literal  (  ++  –  sizeof}

FIRST(ARGUMENT_EXPRESSION_LIST_PRIME) = {, | ε} 

FIRST(PRIMARY_EXPRESSION)  = {identifier  integer_constant  float_constant  string_literal  (}

FIRST(CONSTANT) = {integer_constant  float_constant  string_literal  identifier}