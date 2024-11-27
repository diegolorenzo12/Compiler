#pragma once
#include "ASTVisitor.h"
#include <iostream>
#include "SymbolTable.h"
#include <memory>
#include <stdexcept>
#include "ErrorManager.h"

/*
Semantic Analyzer ABSTRACT SYNTAX TREE visitor
*/

class SemanticAnalyzer : public ASTVisitor
{
private:
    std::shared_ptr<SymbolTable> symbolTable;
    std::shared_ptr<SemanticType> inferredType;

    // helper variables
    bool insideSwitch = false;
    bool insideLoop = false;

public:
    explicit SemanticAnalyzer(std::shared_ptr<SymbolTable> symbolTable) : symbolTable(std::move(symbolTable)) {}

    void acceptIfNotNull(ASTNode *node)
    {
        if (node != nullptr)
        {
            node->accept(*this);
        }
    }

    void acceptIfNotNull(const std::unique_ptr<ASTNode> &node)
    {
        acceptIfNotNull(node.get());
    }

    template <typename NodeType>
    void acceptIfNotNull(const std::unique_ptr<NodeType> &node)
    {
        acceptIfNotNull(node.get());
    }

    void visit(Program &node) override
    {
        for (const auto &decl : node.getTopLevelDeclarations())
        {
            acceptIfNotNull(decl);
        }
    }

    void visit(BinaryExpr &node) override
    {
        acceptIfNotNull(node.getLHS());
        auto lhsType = inferredType;

        acceptIfNotNull(node.getRHS());
        auto rhsType = inferredType;

        if (!lhsType || !rhsType)
        {
            throw SemanticException(node.getLine(), "Both operands of binary expression must have valid types.");
        }

        // Operator-dependent type inference
        if (node.getOp() == "+" || node.getOp() == "-" || node.getOp() == "*" || node.getOp() == "/")
        {
            if ((lhsType->equals(PrimitiveSemanticType(DataType::INT)) || lhsType->equals(PrimitiveSemanticType(DataType::FLOAT)) ||
                 lhsType->equals(PrimitiveSemanticType(DataType::DOUBLE))) &&
                (rhsType->equals(PrimitiveSemanticType(DataType::INT)) || rhsType->equals(PrimitiveSemanticType(DataType::FLOAT)) ||
                 rhsType->equals(PrimitiveSemanticType(DataType::DOUBLE))))
            {
                // The result will be the type that can hold the largest value
                if (lhsType->equals(PrimitiveSemanticType(DataType::DOUBLE)) || rhsType->equals(PrimitiveSemanticType(DataType::DOUBLE)))
                {
                    inferredType = std::make_shared<PrimitiveSemanticType>(DataType::DOUBLE);
                }
                else if (lhsType->equals(PrimitiveSemanticType(DataType::FLOAT)) || rhsType->equals(PrimitiveSemanticType(DataType::FLOAT)))
                {
                    inferredType = std::make_shared<PrimitiveSemanticType>(DataType::FLOAT);
                }
                else
                {
                    inferredType = std::make_shared<PrimitiveSemanticType>(DataType::INT);
                }
            }
            else
            {
                throw SemanticException(node.getLine(), "Operands must be numeric for binary arithmetic operation.");
            }
        }
        else if (node.getOp() == "==" || node.getOp() == "!=" || node.getOp() == "<" || node.getOp() == ">" ||
                 node.getOp() == "<=" || node.getOp() == ">=")
        {
            if ((lhsType->equals(PrimitiveSemanticType(DataType::INT)) || lhsType->equals(PrimitiveSemanticType(DataType::FLOAT)) ||
                 lhsType->equals(PrimitiveSemanticType(DataType::DOUBLE)) || lhsType->equals(PrimitiveSemanticType(DataType::CHAR))) &&
                (rhsType->equals(PrimitiveSemanticType(DataType::INT)) || rhsType->equals(PrimitiveSemanticType(DataType::FLOAT)) ||
                 rhsType->equals(PrimitiveSemanticType(DataType::DOUBLE)) || rhsType->equals(PrimitiveSemanticType(DataType::CHAR))))
            {

                // The result will be boolean
                inferredType = std::make_shared<PrimitiveSemanticType>(DataType::BOOL);
            }
            else
            {
                throw SemanticException(node.getLine(), "Operands must be comparable for binary comparison operation.");
            }
        }
        else if (node.getOp() == "&&" || node.getOp() == "||")
        {
            // Logical operators: both operands must be boolean
            if (lhsType->equals(PrimitiveSemanticType(DataType::BOOL)) && rhsType->equals(PrimitiveSemanticType(DataType::BOOL)))
            {
                inferredType = std::make_shared<PrimitiveSemanticType>(DataType::BOOL);
            }
            else
            {
                throw SemanticException(node.getLine(), "Operands must be boolean for binary logical operation.");
            }
        }
        else
        {
            throw SemanticException(node.getLine(), "Unsupported operator for binary expression.");
        }
    }

    void visit(FunctionDecl &node) override
    {
        acceptIfNotNull(node.getDeclarationSpecifiers());
        // wrap infered in function type

        auto functionType = std::make_shared<FunctionType>(inferredType);
        inferredType = functionType;

        // create a new scope for the function
        auto parentScope = symbolTable;
        auto childScope = symbolTable->createChildScope();
        symbolTable = childScope;
        // in declaration i save in semantic table
        acceptIfNotNull(node.getDeclarator());
        acceptIfNotNull(node.getBody());
        symbolTable = parentScope;
    }

    void visit(Declaration &node) override
    {
        acceptIfNotNull(node.getDeclarationSpecifiers());
        acceptIfNotNull(node.getDeclaratorList());
    }

    void visit(PrimitiveType &node) override
    {
        inferredType = std::make_shared<PrimitiveSemanticType>(stringToDataType(node.getBaseType()), false);
    }

    void visit(Specifier &node) override
    {
    }

    void visit(DeclarationSpecifiers &node) override
    {
        acceptIfNotNull(node.getType());
        acceptIfNotNull(node.getSpecifiers());
    }

    void visit(Declarator &node) override
    {
        std::string identifier = node.getIdentifier();

        acceptIfNotNull(node.getDeclarator());
        for (const auto &decl : node.getDirectDeclarators())
        {
            acceptIfNotNull(decl);
        }
        // create a symbol with the inferred type and identifier
        //  then insert it into the symbol table if you can

        // check if inferreed type is a primitive type to save in a variable symbol
        if (auto functionType = std::dynamic_pointer_cast<FunctionType>(inferredType))
        {
            // if symbol table has parent, insert the function there
            if (symbolTable->getParent())
            {
                if (!symbolTable->getParent()->insertSymbol(std::make_shared<FunctionSymbol>(identifier, functionType)))
                {
                    throw SemanticException(node.getLine(), "Error: Variable " + identifier + " already declared.");
                }
                if (!symbolTable->insertSymbol(std::make_shared<FunctionSymbol>(identifier, functionType)))
                {
                    throw SemanticException(node.getLine(), "Error: Variable " + identifier + " already declared.");
                }
            }
            else
            {
                throw SemanticException(node.getLine(), "Error: Function definition outside of function scope.");
            }
        }
        else if (auto structType = std::dynamic_pointer_cast<StructType>(inferredType))
        {
            std::shared_ptr<Symbol> currentSymbol = symbolTable->lookupSymbol(structType->getName());
            if (currentSymbol && dynamic_cast<StructSymbol *>(currentSymbol.get()))
            {

                auto currentType = currentSymbol->getType();
                if (node.isPointer())
                {
                    currentType = std::make_shared<PointerType>(currentType, false);
                }
                if (!symbolTable->insertSymbol(std::make_shared<VariableSymbol>(identifier, currentType)))
                {
                    throw SemanticException(node.getLine(), "Error: Variable " + identifier + " already declared.");
                }
            }
            else
            {
                throw SemanticException(node.getLine(), "Error: Struct " + structType->getName() + " not declared.");
            }

        } // array, pointer, primitive type
        else
        {
            if (node.isPointer())
            {
                /*wrap in pointer*/
                inferredType = std::make_shared<PointerType>(inferredType, false);
            }
            if (!symbolTable->insertSymbol(std::make_shared<VariableSymbol>(identifier, inferredType)))
            {
                throw SemanticException(node.getLine(), "Error: Variable " + identifier + " already declared.");
            }
        }

        auto tempType = inferredType;
        acceptIfNotNull(node.getInitializer()); // in symbol initializer
        if (node.getInitializer())
        {
            // compare inferred type with initializer type
            if (!tempType->equals(*inferredType))
            {
                throw SemanticException(node.getLine(), "Initializer type does not match declared type.");
            }
        }
        inferredType = tempType;
    }

    void visit(DeclaratorList &node) override
    {
        auto currentType = inferredType;
        for (const auto &decl : node.getDeclarators())
        {
            acceptIfNotNull(decl);
        }
    }

    void visit(ExpressionInitializer &node) override
    {
        acceptIfNotNull(node.getExpr());
    }

    void visit(BraceInitializer &node) override
    {
        auto tempType = inferredType;
        if (auto arrayType = std::dynamic_pointer_cast<ArrayType>(tempType))
        {
            inferredType = arrayType->getElementType();
        }
        else
        {
            throw SemanticException(node.getLine(), "Invalid brace initializer not in array type.");
        }
        acceptIfNotNull(node.getInitializerList());
        inferredType = tempType;
    }

    void visit(InitializerList &node) override
    {
        auto tempType = inferredType;
        for (const auto &initializer : node.getInitializerList())
        {
            if (auto *expressionInitializer = dynamic_cast<ExpressionInitializer *>(initializer.get()))
            {
                acceptIfNotNull(expressionInitializer);

                if (!tempType->equals(*inferredType))
                {
                    throw SemanticException(node.getLine(), "Initializer type does not match declared type.");
                }
            }
            else if (auto *braceInitializer = dynamic_cast<BraceInitializer *>(initializer.get()))
            {
                acceptIfNotNull(braceInitializer);
            }
            else
            {
                throw SemanticException(node.getLine(), "Invalid initializer type.");
            }
        }
        inferredType = tempType;
    }

    void visit(BlockStmt &node) override
    {
        // create new scope
        auto parentScope = symbolTable;
        auto childScope = symbolTable->createChildScope();
        symbolTable = childScope;

        for (const auto &item : node.getItems())
        {
            if (!item)
            {
                continue;
            }
            // try to cast to case or default statement and throw error if they are
            if (auto *stmtWrapper = dynamic_cast<StatementWrapper *>(item.get()))
            {

                if (dynamic_cast<LabaledStatement *>(stmtWrapper->getStatement()))
                {
                    throw SemanticException(node.getLine(), "Error: Case or default statement found outside of switch body.");
                }
            }
            acceptIfNotNull(item);
        }
        symbolTable = parentScope;
    }

    void visit(DeclarationWrapper &node) override
    {
        acceptIfNotNull(node.getDeclaration());
    }

    void visit(StatementWrapper &node) override
    {
        acceptIfNotNull(node.getStatement());
    }

    void visit(CaseStatement &node) override
    {
        acceptIfNotNull(node.getExpr());
        // check that this is constant
        if (!inferredType->isConst())
        {
            throw SemanticException(node.getLine(), "Case expression must be a constant.");
        }
        acceptIfNotNull(node.getStatement());
    }

    void visit(DefaultStatement &node) override
    {
        acceptIfNotNull(node.getStatement());
    }

    void visit(ExpressionStatement &node) override
    {
        acceptIfNotNull(node.getExpr());
    }

    void visit(SwitchStatement &node) override
    {
        // create a new scope
        auto parentScope = symbolTable;
        auto childScope = symbolTable->createChildScope();
        symbolTable = childScope;
        insideSwitch = true;

        acceptIfNotNull(node.getCondition());
        auto conditionType = inferredType;

        if (auto *block = dynamic_cast<BlockStmt *>(node.getBody()))
        {
            bool hasDefault = false;
            for (const std::unique_ptr<BlockItemBase> &item : block->getItems())
            {
                if (auto *stmtWrapper = dynamic_cast<StatementWrapper *>(item.get()))
                {
                    auto *wrappedStmt = stmtWrapper->getStatement();
                    // cast to case or default statement
                    if (auto *caseStmt = dynamic_cast<CaseStatement *>(wrappedStmt))
                    {
                        acceptIfNotNull(caseStmt);
                    }
                    else if (auto *defaultStmt = dynamic_cast<DefaultStatement *>(wrappedStmt))
                    {
                        if (hasDefault)
                        {
                            throw SemanticException(node.getLine(), "Error: Multiple default statements are not allowed in a switch body.");
                        }
                        hasDefault = true;
                        acceptIfNotNull(defaultStmt);
                    }
                    else
                    {
                        throw SemanticException(node.getLine(), "Error: Only LabeledStatement (Case or Default) is allowed in a Switch body.");
                    }
                }
                else
                {
                    acceptIfNotNull(item);
                }
            }
        }
        else
        {
            throw SemanticException(node.getLine(), "Error: Switch body must be a BlockStmt.");
        }
        insideSwitch = false;
        symbolTable = parentScope;
    }

    void visit(IfStatement &node) override
    {
        acceptIfNotNull(node.getCondition());
        acceptIfNotNull(node.getThen());
    }

    void visit(WhileStatement &node) override
    {
        acceptIfNotNull(node.getCondition());
        insideLoop = true;
        acceptIfNotNull(node.getBody());
        insideLoop = false;
    }

    void visit(DoWhileStatement &node) override
    {
        acceptIfNotNull(node.getCondition());
        insideLoop = true;
        acceptIfNotNull(node.getBody());
        insideLoop = false;
    }

    void visit(ForStatement &node) override
    {
        // create new scope
        auto parentScope = symbolTable;
        auto childScope = symbolTable->createChildScope();
        symbolTable = childScope;
        acceptIfNotNull(node.getInit());

        insideLoop = true;
        acceptIfNotNull(node.getStatement());
        insideLoop = false;
        symbolTable = parentScope;
    }

    void visit(ForWitDeclaration &node) override
    {
        acceptIfNotNull(node.getDeclaration());
        acceptIfNotNull(node.getExpr());
        acceptIfNotNull(node.getOptionalExpr());
    }

    void visit(ForWithExpression &node) override
    {
        acceptIfNotNull(node.getExpr());
        acceptIfNotNull(node.getExpr2());
        acceptIfNotNull(node.getOptionalExpr());
    }

    void visit(ReturnStatement &node) override
    {

        acceptIfNotNull(node.getExpr());
        // compare inferred type with function type.
        if (std::shared_ptr<FunctionSymbol> functionSymbol = symbolTable->getFunctionSymbol())
        {
            // std cast to function type
            if (auto functionType = std::dynamic_pointer_cast<FunctionType>(functionSymbol->getType()))
            {
                if (!(functionType && functionType->getReturnType() && functionType->getReturnType()->equals(*inferredType)))
                {
                    if (functionType && functionType->getReturnType() && functionType->getReturnType()->equals(PrimitiveSemanticType(DataType::VOID)) && node.getExpr() == nullptr)
                    {
                        return;
                    }

                    throw SemanticException(node.getLine(), "Return type does not match function type.");
                }
            }
            else
            {
                throw SemanticException(node.getLine(), "Return statement found outside of function scope.");
            }
        }
    }

    void visit(ContinueStatement &node) override
    {
        if (insideLoop == false)
        {
            throw SemanticException(node.getLine(), "Continue statement found outside of loop statement.");
        }
    }

    void visit(BreakStatement &node) override
    {
        if (insideLoop == false && insideSwitch == false)
        {
            throw SemanticException(node.getLine(), "Break statement found outside of loop or switch statement.");
        }
    }

    void visit(ConditionalExpression &node) override
    {
        acceptIfNotNull(node.getLogicalOrExpression());
        auto savedType = inferredType;

        if (node.getExpression() != nullptr || node.getConditionalExpression() != nullptr)
        {
            if (!inferredType || !inferredType->equals(PrimitiveSemanticType(DataType::BOOL)))
            {
                throw SemanticException(node.getLine(), "Condition of conditional expression must be of type 'bool'.");
            }
        }

        // visit true
        acceptIfNotNull(node.getExpression());
        auto trueType = inferredType;

        // visit false
        acceptIfNotNull(node.getConditionalExpression());
        auto falseType = inferredType;

        if (!trueType->equals(*falseType))
        {
            throw SemanticException(node.getLine(), "Types of true and false branches must match in conditional expression.");
        }

        inferredType = trueType;
    }

    void visit(ParenthesizedExpression &node) override
    {
        acceptIfNotNull(node.getExpr());
    }

    void visit(ConstantExpression &node) override
    {
        switch (node.getConstantType())
        {
        case INTEGER_CONSTANT:
            inferredType = std::make_shared<PrimitiveSemanticType>(DataType::INT, true);
            break;
        case FLOAT_CONSTANT:
            inferredType = std::make_shared<PrimitiveSemanticType>(DataType::FLOAT, true);
            break;
        case STRING_LITERAL:
        {
            auto charType = std::make_shared<PrimitiveSemanticType>(DataType::CHAR);
            auto charArrayType = std::make_shared<ArrayType>(charType, node.getConstant().length());
            inferredType = charArrayType;
            break;
        }
        default:
            throw SemanticException(node.getLine(), "Unknown constant type.");
        }
    }

    void visit(IdentifierExpression &node) override
    {
        auto symbol = symbolTable->lookupSymbol(node.getIdentifier());
        if (!symbol)
        {
            throw SemanticException(node.getLine(), "Undeclared variable in expression: " + node.getIdentifier());
        }
        inferredType = symbol->getType();
    }

    void visit(ArrayPostFixExpression &node) override
    {
        if (inferredType->isConst())
        {
            throw SemanticException(node.getLine(), "Cannot access array at constant.");
        }
        if (auto arrayType = dynamic_cast<ArrayType *>(inferredType.get()))
        {
            // step into the array type and extract a level from the array
            inferredType = arrayType->getElementType();
            auto originalInferredType = inferredType;
            // now i have to step into the array size [size here] but since this can be an expression like [expression here] i need to check that this is indeed a valid expression
            // how can i do that without deleteing the inferred type?
            acceptIfNotNull(node.getArraySize());
            inferredType = originalInferredType;
        }
        else
        {
            throw SemanticException(node.getLine(), "Cannot access array at non-array type.");
        }
    }

    void visit(AssignmentExpressionList &node) override
    {
        // cast to function type, if it cans then it is calling a function
        if (auto functionType = std::dynamic_pointer_cast<FunctionType>(inferredType))
        {
            for (size_t i = 0; i < node.getAssignmentExpression().size(); ++i)
            {
                acceptIfNotNull(node.getAssignmentExpression()[i]);
                if (!inferredType->equals(*functionType->getParamTypes()[i]))
                {
                    throw SemanticException(node.getLine(), "Assignment types do not match.");
                }
                inferredType = functionType;
            }
        }
        else
        {
            for (auto &assignment : node.getAssignmentExpression())
            {
                acceptIfNotNull(assignment);
            }
        }
    }

    void visit(AssignmentOperator &node) override
    {
    }

    void visit(AssignmentExpression &node) override
    {
        // have to check conditional expression is only an ident
        if (node.getConditionalExpression() != nullptr && (node.getConditionalExpression()->getExpression() != nullptr || node.getConditionalExpression()->getConditionalExpression() != nullptr))
        {
            throw SemanticException(node.getLine(), "lvalue required as left operand of assignment");
        }
        if (node.getConditionalExpression() != nullptr && node.getConditionalExpression()->getLogicalOrExpression() != nullptr)
        {
            acceptIfNotNull(node.getConditionalExpression());
            if (node.getAssignmentOperator() != nullptr && node.getAssignmentExpression() != nullptr)
            {

                if (auto logicalOrExpression = dynamic_cast<BinaryExpr *>(node.getConditionalExpression()->getLogicalOrExpression()))
                {
                    throw SemanticException(node.getLine(), "lvalue required as left operand of assignment");
                }

                if (auto postfix = dynamic_cast<PostfixExpressions *>(node.getConditionalExpression()->getLogicalOrExpression()))
                {
                    // check that postfix is type identifier expression
                    if (postfix->getPrimaryExpression() == nullptr)
                    {
                        throw SemanticException(node.getLine(), "Primary expression in lvalue not found during assignment.");
                    }
                    if (dynamic_cast<ConstantExpression *>(postfix->getPrimaryExpression()))
                    {
                        throw SemanticException(node.getLine(), "Assigment must be to an identifier, got constant.");
                    }
                    if (dynamic_cast<ParenthesizedExpression *>(postfix->getPrimaryExpression()))
                    {
                        throw SemanticException(node.getLine(), "Assigment must be to an identifier, got parenthesis.");
                    }

                    if (!dynamic_cast<IdentifierExpression *>(postfix->getPrimaryExpression()))
                    {
                        throw SemanticException(node.getLine(), "Assigment must be to an identifier 2.");
                    }
                }

                std::shared_ptr<SemanticType> tempType = inferredType;
                // check that type is not constant. Constants dont allow assignments.
                if (tempType->isConst())
                {
                    throw SemanticException(node.getLine(), "Cannot assign to a constant.");
                }
                acceptIfNotNull(node.getAssignmentOperator());
                acceptIfNotNull(node.getAssignmentExpression());
                if (!(inferredType && tempType && inferredType->equals(*tempType)))
                {
                    // if types dont match, check if it can be casted into function and check if return match is same type
                    if (auto functionType = std::dynamic_pointer_cast<FunctionType>(inferredType))
                    {
                        if (!functionType->getReturnType()->equals(*tempType))
                        {
                            throw SemanticException(node.getLine(), "Assignment types do not match.");
                        }
                    }
                    else
                    {

                        throw SemanticException(node.getLine(), "Assignment types do not match.");
                    }
                }
            }
            // IdentifierExpression
        }
    }

    // when in function call
    void visit(ArgumentsPostFixExpression &node) override
    {
        // cast inferred type to function type
        if (auto functionType = std::dynamic_pointer_cast<FunctionType>(inferredType))
        {
            // check if the number of arguments is the same as the number of parameters
            if (node.getArgumentList() && functionType->getParamTypes().size() != node.getArgumentList()->getAssignmentExpression().size() || !node.getArgumentList())
            {
                throw SemanticException(node.getLine(), "Number of arguments does not match number of parameters in function call.");
            }

            acceptIfNotNull(node.getArgumentList());
        }
        else
        {
            throw SemanticException(node.getLine(), "Cannot call non-function type.");
        }
        // acceptIfNotNull(node.getArgumentList());
    }

    void visit(DotOperatorPostfixExpression &node) override
    {
        if (auto structType = dynamic_cast<StructType *>(inferredType.get()))
        {
            // In this case, we just need to find the field within the struct
            const auto &fieldName = node.getIdentifier(); // Assuming this is the member name to access
            bool foundField = false;

            // Look for the field in the struct's fields
            for (const auto &field : structType->getFields())
            {
                if (field.first == fieldName)
                {
                    inferredType = field.second;
                    foundField = true;
                    break;
                }
            }

            if (!foundField)
            {
                throw SemanticException(node.getLine(), "Field '" + fieldName + "' not found in struct " + structType->getName());
            }
        }
        else
        {
            throw SemanticException(node.getLine(), "Cannot access member of non-struct type using dot operator.");
        }
    }

    void visit(ArrowOperatorPostfixExpression &node) override
    {
        if (auto pointerType = dynamic_cast<PointerType *>(inferredType.get()))
        {
            if (auto structType = dynamic_cast<StructType *>(pointerType->getPointeeType().get()))
            {
                const auto &fieldName = node.getIdentifier();
                bool foundField = false;

                for (const auto &field : structType->getFields())
                {
                    if (field.first == fieldName)
                    {
                        inferredType = field.second;
                        foundField = true;
                        break;
                    }
                }

                if (!foundField)
                {
                    throw SemanticException(node.getLine(), "Field '" + fieldName + "' not found in struct.");
                }
            }
            else
            {
                throw SemanticException(node.getLine(), "Pointer does not point to a struct type.");
            }
        }
        else
        {
            throw SemanticException(node.getLine(), "Cannot access member of non-pointer type using arrow operator.");
        }
    }

    void visit(IncrementDecrementPostfixExpression &node) override
    {
        if (inferredType->isConst())
        {
            throw SemanticException(node.getLine(), "Cannot increment ++ or -- at constant.");
        }
        if (node.getOp() != "++" && node.getOp() != "--")
        {
            throw SemanticException(node.getLine(), "Unknown increment/decrement operator: " + node.getOp());
        }
    }

    void visit(PostfixExpressions &node) override
    {
        acceptIfNotNull(node.getPrimaryExpression());

        for (const auto &expr : node.getPostfixExpressions())
        {
            acceptIfNotNull(expr);
        }
    }

    void visit(UnaryIncrementDecrementOperator &node) override
    {
        acceptIfNotNull(node.getUnaryExpr());
    }

    void visit(IdentifierList &node) override
    {
        for (const auto &id : node.getIdentifiers())
        {
        }
    }

    void visit(ArrayDirectDeclarator &node) override
    {
        // wrap current inferred type in an array type
        auto arrayType = std::make_shared<ArrayType>(inferredType, 0);
        acceptIfNotNull(node.getTypeQualifier());
        acceptIfNotNull(node.getArrSize());
        // check that expression in arraysize is a valid number
        if (!inferredType->equals(PrimitiveSemanticType(DataType::INT)))
        {
            throw SemanticException(node.getLine(), "Array size must be an integer.");
        }

        inferredType = arrayType;
    }

    void visit(ParameterDeclaration &node) override
    {
        acceptIfNotNull(node.getDeclarationSpecifiers());
        acceptIfNotNull(node.getDeclarator());
    }

    void visit(ParameterDeclarationList &node) override
    {
        // check that current type is a function type
        if (auto functionType = dynamic_cast<FunctionType *>(inferredType.get()))
        {
            for (const auto &param : node.getParameters())
            {
                auto tempType = inferredType;
                acceptIfNotNull(param);
                functionType->addParamType(inferredType);
                inferredType = tempType;
            }
        }
        else
        {
            throw SemanticException(node.getLine(), "Parameters can only be added to function types.");
        }
    }

    void visit(AnonimousStruct &node) override
    {
        acceptIfNotNull(node.getStructDeclarationList());
    }

    void visit(StructDeclaration &node) override
    {
        // create a type struct and save the identifier and the struct list
        auto structType = std::make_shared<StructType>(node.getIdentifier());
        inferredType = structType;

        acceptIfNotNull(node.getStructDeclarationList());
        // insert struct into symbol table
        if (auto structType = std::dynamic_pointer_cast<StructType>(inferredType))
        {
            if (!node.getStructDeclarationList())
            { // if struct is empty, then it is a struct instance
                if (symbolTable->lookupSymbol(node.getIdentifier()) == nullptr)
                {
                    throw SemanticException(node.getLine(), "Error: Struct " + node.getIdentifier() + " is not declared, cant instanciate a struct.");
                }
            }
            else
            {
                if (!symbolTable->insertSymbol(std::make_shared<StructSymbol>(node.getIdentifier(), structType)))
                {
                    throw SemanticException(node.getLine(), "Error: Struct " + node.getIdentifier() + " already declared.");
                }
            }
        }
    }

    void visit(StructMemberDeclarationList &node) override
    {

        // for each member encounter, add it to the struct type
        for (const auto &member : node.getStructMemberDeclarations())
        {
            acceptIfNotNull(member);
        }
    }

    void visit(StructMemberDeclaration &node) override
    {
        if (auto structType = std::dynamic_pointer_cast<StructType>(inferredType))
        {
            acceptIfNotNull(node.getTypeSpecifier());
            auto membersType = inferredType;

            if (node.getDeclaratorList())
            {
                for (const auto &decl : node.getDeclaratorList()->getDeclarators())
                {
                    auto currentMemberType = membersType;
                    if (decl)
                    {
                        acceptIfNotNull(decl->getDeclarator());
                        for (const auto &dirDecl : decl->getDirectDeclarators())
                        {
                            acceptIfNotNull(dirDecl);
                            currentMemberType = inferredType;
                        }

                        if (decl->isPointer())
                        {
                            currentMemberType = std::make_shared<PointerType>(currentMemberType, false);
                        }
                        // insert into struct
                        structType->addField(decl->getIdentifier(), currentMemberType);
                    }
                }
            }
            inferredType = structType;
        }
        else
        {
            throw SemanticException(node.getLine(), "Struct member declaration found outside of struct.");
        }
    }
};