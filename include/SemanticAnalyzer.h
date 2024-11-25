#pragma once
#include "ASTVisitor.h"
#include <iostream>
#include "SymbolTable.h"
#include <memory>
#include <stdexcept>

/*
Semantic Analyzer ABSTRACT SYNTAX TREE visitor
*/

class SemanticAnalyzer : public ASTVisitor
{
private:
    std::shared_ptr<SymbolTable> symbolTable;
    std::shared_ptr<SemanticType> inferredType;

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
            throw std::runtime_error("Both operands of binary expression must have valid types.");
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
                throw std::runtime_error("Operands must be numeric for binary arithmetic operation.");
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
                throw std::runtime_error("Operands must be comparable for binary comparison operation.");
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
                throw std::runtime_error("Operands must be boolean for binary logical operation.");
            }
        }
        else
        {
            throw std::runtime_error("Unsupported operator for binary expression.");
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

        std::cout << std::endl
                  << std::endl
                  << "global Scope: " << std::endl;
        parentScope->printCurrentScope();

        std::cout << std::endl
                  << std::endl
                  << "Current Function Scope: " << std::endl;
        childScope->printCurrentScope();
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
        // std::cout << "Specifier:" << node.getSpecifier() << ", SpecifierType:" << node.getSpecifierType();
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
            std::cout << "function type " << functionType->toString() << std::endl;
            // if symbol table has parent, insert the function there
            if (symbolTable->getParent())
            {
                if (!symbolTable->getParent()->insertSymbol(std::make_shared<FunctionSymbol>(identifier, functionType)))
                {
                    throw std::runtime_error("Error: Variable " + identifier + " already declared.");
                }
                if (!symbolTable->insertSymbol(std::make_shared<FunctionSymbol>(identifier, functionType)))
                {
                    throw std::runtime_error("Error: Variable " + identifier + " already declared.");
                }
            }
            else
            {
                throw std::runtime_error("Error: Function definition outside of function scope.");
            }
        }
        else if (auto structType = std::dynamic_pointer_cast<StructType>(inferredType))
        {
            std::shared_ptr<Symbol> currentSymbol = symbolTable->lookupSymbol(structType->getName());
            if (currentSymbol && dynamic_cast<StructSymbol *>(currentSymbol.get()))
            {

                auto currentType = currentSymbol->getType();
                // std::cout << "currentType struct " << identifier << " with type " << currentType->toString() << std::endl;
                if (node.isPointer())
                {
                    currentType = std::make_shared<PointerType>(currentType, false);
                }
                if (!symbolTable->insertSymbol(std::make_shared<VariableSymbol>(identifier, currentType)))
                {
                    throw std::runtime_error("Error: Variable " + identifier + " already declared.");
                }
            }
            else
            {
                throw std::runtime_error("Error: Struct " + structType->getName() + " not declared.");
            }

        } // array, pointer, primitive type
        else
        {
            if (node.isPointer())
            {
                /*wrap in pointer*/
                inferredType = std::make_shared<PointerType>(inferredType, false);
            }
            // std::cout << "inserting symbol " << identifier << " with type " << inferredType->toString() << std::endl;
            if (!symbolTable->insertSymbol(std::make_shared<VariableSymbol>(identifier, inferredType)))
            {
                throw std::runtime_error("Error: Variable " + identifier + " already declared.");
            }
        }

        auto tempType = inferredType;
        acceptIfNotNull(node.getInitializer()); // in symbol initializer
        if (node.getInitializer())
        {
            // compare inferred type with initializer type
            if (!tempType->equals(*inferredType))
            {
                throw std::runtime_error("Initializer type does not match declared type.");
            }
        }
        inferredType = tempType;
    }

    void visit(DeclaratorList &node) override
    {
        // std::cout << "DeclaratorList {\n";
        auto currentType = inferredType;
        for (const auto &decl : node.getDeclarators())
        {
            acceptIfNotNull(decl);
        }
        // std::cout << "}\n";
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
            throw std::runtime_error("Invalid brace initializer not in array type.");
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
                    throw std::runtime_error("Initializer type does not match declared type.");
                }
            }
            else if (auto *braceInitializer = dynamic_cast<BraceInitializer *>(initializer.get()))
            {
                acceptIfNotNull(braceInitializer);
            }
            else
            {
                throw std::runtime_error("Invalid initializer type.");
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
            throw std::runtime_error("Case expression must be a constant.");
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
                            throw std::runtime_error("Error: Multiple default statements are not allowed in a switch body.");
                        }
                        hasDefault = true;
                        acceptIfNotNull(defaultStmt);
                    }
                    else
                    {
                        throw std::runtime_error("Error: Only LabeledStatement (Case or Default) is allowed in a Switch body.");
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
            throw std::runtime_error("Error: Switch body must be a BlockStmt.");
        }
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
        acceptIfNotNull(node.getBody());
    }

    void visit(DoWhileStatement &node) override
    {
        acceptIfNotNull(node.getCondition());
        acceptIfNotNull(node.getBody());
    }

    void visit(ForStatement &node) override
    {
        // create new scope
        auto parentScope = symbolTable;
        auto childScope = symbolTable->createChildScope();
        symbolTable = childScope;
        acceptIfNotNull(node.getInit());
        acceptIfNotNull(node.getStatement());
        symbolTable = parentScope;
    }

    void visit(ForWitDeclaration &node) override
    {
        // std::cout << "ForWitDeclaration ";
        acceptIfNotNull(node.getDeclaration());
        acceptIfNotNull(node.getExpr());
        acceptIfNotNull(node.getOptionalExpr());
    }

    void visit(ForWithExpression &node) override
    {
        // std::cout << "ForWithExpression ";
        acceptIfNotNull(node.getExpr());
        acceptIfNotNull(node.getExpr2());
        acceptIfNotNull(node.getOptionalExpr());
    }

    void visit(ReturnStatement &node) override
    {
        // std::cout << "ReturnStatement ";

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

                    throw std::runtime_error("Return type does not match function type.");
                }
            }
            else
            {
                throw std::runtime_error("Return statement found outside of function scope.");
            }
        }
    }

    void visit(ContinueStatement &node) override
    {
        // std::cout << "ContinueStatement ";
    }

    void visit(BreakStatement &node) override
    {
        // std::cout << "BreakStatement ";
    }

    void visit(ConditionalExpression &node) override
    {
        acceptIfNotNull(node.getLogicalOrExpression());
        auto savedType = inferredType;

        if (node.getExpression() != nullptr || node.getConditionalExpression() != nullptr)
        {
            if (!inferredType || !inferredType->equals(PrimitiveSemanticType(DataType::BOOL)))
            {
                throw std::runtime_error("Condition of conditional expression must be of type 'bool'.");
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
            throw std::runtime_error("Types of true and false branches must match in conditional expression.");
        }

        inferredType = trueType;
    }

    void visit(ParenthesizedExpression &node) override
    {
        acceptIfNotNull(node.getExpr());
        // std::cout << inferredType->toString() << " in parenthesis" << std::endl;
    }

    void visit(ConstantExpression &node) override
    {
        switch (node.getConstantType())
        {
        case INTEGER_CONSTANT:
            inferredType = std::make_shared<PrimitiveSemanticType>(DataType::INT, true);
            // std::cout << inferredType->toString() << " in const int " << std::endl;
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
            throw std::runtime_error("Unknown constant type.");
        }
    }

    void visit(IdentifierExpression &node) override
    {
        auto symbol = symbolTable->lookupSymbol(node.getIdentifier());
        if (!symbol)
        {
            throw std::runtime_error("Undeclared variable in expression: " + node.getIdentifier());
        }
        inferredType = symbol->getType();
        // std::cout << "type of symbol " << node.getIdentifier() << " is " << inferredType->toString() << std::endl;
    }

    void visit(ArrayPostFixExpression &node) override
    {
        // std::cout << "ArrayPostFixExpression ";
        if (inferredType->isConst())
        {
            throw std::runtime_error("Cannot access array at constant.");
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
            throw std::runtime_error("Cannot access array at non-array type.");
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
                    throw std::runtime_error("Assignment types do not match.");
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
        // std::cout << "AssignmentOperator: " << node.getOp();
    }

    void visit(AssignmentExpression &node) override
    {
        // have to check conditional expression is only an ident
        if (node.getConditionalExpression() != nullptr && (node.getConditionalExpression()->getExpression() != nullptr || node.getConditionalExpression()->getConditionalExpression() != nullptr))
        {
            throw std::runtime_error("lvalue required as left operand of assignment");
        }
        if (node.getConditionalExpression() != nullptr && node.getConditionalExpression()->getLogicalOrExpression() != nullptr)
        {
            acceptIfNotNull(node.getConditionalExpression());
            if (node.getAssignmentOperator() != nullptr && node.getAssignmentExpression() != nullptr)
            {

                if (auto logicalOrExpression = dynamic_cast<BinaryExpr *>(node.getConditionalExpression()->getLogicalOrExpression()))
                {
                    throw std::runtime_error("lvalue required as left operand of assignment");
                }

                if (auto postfix = dynamic_cast<PostfixExpressions *>(node.getConditionalExpression()->getLogicalOrExpression()))
                {
                    // check that postfix is type identifier expression
                    if (postfix->getPrimaryExpression() == nullptr)
                    {
                        throw std::runtime_error("Primary expression in lvalue not found during assignment.");
                    }
                    if (dynamic_cast<ConstantExpression *>(postfix->getPrimaryExpression()))
                    {
                        throw std::runtime_error("Assigment must be to an identifier, got constant.");
                    }
                    if (dynamic_cast<ParenthesizedExpression *>(postfix->getPrimaryExpression()))
                    {
                        throw std::runtime_error("Assigment must be to an identifier, got parenthesis.");
                    }

                    if (!dynamic_cast<IdentifierExpression *>(postfix->getPrimaryExpression()))
                    {
                        throw std::runtime_error("Assigment must be to an identifier 2.");
                    }
                }

                std::shared_ptr<SemanticType> tempType = inferredType;
                // check that type is not constant. Constants dont allow assignments.
                if (tempType->isConst())
                {
                    throw std::runtime_error("Cannot assign to a constant.");
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
                            throw std::runtime_error("Assignment types do not match.");
                        }
                    }
                    else
                    {

                        throw std::runtime_error("Assignment types do not match.");
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
                throw std::runtime_error("Number of arguments does not match number of parameters in function call.");
            }

            acceptIfNotNull(node.getArgumentList());
        }
        else
        {
            throw std::runtime_error("Cannot call non-function type.");
        }
        // acceptIfNotNull(node.getArgumentList());
    }

    void visit(DotOperatorPostfixExpression &node) override
    {
        // std::cout << "DotOperatorPostfixExpression " << node.getIdentifier() << " ";
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
                throw std::runtime_error("Field '" + fieldName + "' not found in struct " + structType->getName());
            }
        }
        else
        {
            throw std::runtime_error("Cannot access member of non-struct type using dot operator.");
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
                    throw std::runtime_error("Field '" + fieldName + "' not found in struct.");
                }
            }
            else
            {
                throw std::runtime_error("Pointer does not point to a struct type.");
            }
        }
        else
        {
            throw std::runtime_error("Cannot access member of non-pointer type using arrow operator.");
        }
    }

    void visit(IncrementDecrementPostfixExpression &node) override
    {
        if (inferredType->isConst())
        {
            throw std::runtime_error("Cannot increment ++ or -- at constant.");
        }
        if (node.getOp() != "++" && node.getOp() != "--")
        {
            throw std::runtime_error("Unknown increment/decrement operator: " + node.getOp());
        }
    }

    void visit(PostfixExpressions &node) override
    {
        // std::cout << "PostfixExpressions ";
        acceptIfNotNull(node.getPrimaryExpression());

        for (const auto &expr : node.getPostfixExpressions())
        {
            acceptIfNotNull(expr);
        }
    }

    void visit(UnaryIncrementDecrementOperator &node) override
    {
        // std::cout << "UnaryIncrementDecrementOperator " << node.getOp() << " {";
        acceptIfNotNull(node.getUnaryExpr());
        // std::cout << "}";
    }

    void visit(IdentifierList &node) override
    {
        // std::cout << "IdentifierList {\n";
        for (const auto &id : node.getIdentifiers())
        {
            // std::cout << id << " ";
        }
        // std::cout << "}\n";
    }

    void visit(ArrayDirectDeclarator &node) override
    {
        // wrap current inferred type in an array type
        auto arrayType = std::make_shared<ArrayType>(inferredType, 0);
        if (node.getIsStatic())
        {
            // std::cout << "IsStatic:True, ";
        }
        acceptIfNotNull(node.getTypeQualifier());
        acceptIfNotNull(node.getArrSize());
        // check that expression in arraysize is a valid number
        if (!inferredType->equals(PrimitiveSemanticType(DataType::INT)))
        {
            throw std::runtime_error("Array size must be an integer.");
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
            throw std::runtime_error("Parameters can only be added to function types.");
        }
    }

    void visit(AnonimousStruct &node) override
    {
        // std::cout << "AnonimousStruct {\n";
        acceptIfNotNull(node.getStructDeclarationList());
        // std::cout << "}\n";
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
                    throw std::runtime_error("Error: Struct " + node.getIdentifier() + " is not declared, cant instanciate a struct.");
                }
            }
            else
            {
                if (!symbolTable->insertSymbol(std::make_shared<StructSymbol>(node.getIdentifier(), structType)))
                {
                    throw std::runtime_error("Error: Struct " + node.getIdentifier() + " already declared.");
                }
            }
        }
    }

    void visit(StructMemberDeclarationList &node) override
    {
        // std::cout << "StructMemberDeclarationList {\n";

        // for each member encounter, add it to the struct type
        for (const auto &member : node.getStructMemberDeclarations())
        {
            acceptIfNotNull(member);
            // std::cout << "member type: " << inferredType->toString() << std::endl;
        }
        // std::cout << "}\n";
    }

    void visit(StructMemberDeclaration &node) override
    {
        if (auto structType = std::dynamic_pointer_cast<StructType>(inferredType))
        {
            // std::cout << "StructMemberDeclaration {\n";
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
            throw std::runtime_error("Struct member declaration found outside of struct.");
        }
    }

private:
    DataType inferType(Expr &expr)
    {
    }
};