#include <stdexcept>
#include "Parser.h"
#include <unordered_set>

void Parser::consume()
{
    tokens.pop_front();
    this->currentToken = tokens.front();
}

void Parser::expect(TokenType expected)
{
    if (currentToken.getType() != expected)
    {
        throw std::runtime_error("Unexpected token: " + currentToken.getValue());
    }
    consume();
}

std::unique_ptr<ASTNode> Parser::parseProgram()
{
    auto programNode = std::make_unique<ProgramNode>();
    programNode->push_back_node(parseGlobalDeclarations());
    programNode->push_back_vec(parseProgramPrime());

    if (currentToken.getType() != TokenType::END_OF_FILE)
    {
        throw std::runtime_error("Unexpected token: " + currentToken.getValue());
    }
    return programNode;
}

// PROGRAM_PRIME -> GLOBAL_DECLARATIONS PROGRAM_PRIME | ϵ
std::unique_ptr<ListNode> Parser::parseProgramPrime()
{
    auto programPrimeNode = std::make_unique<GlobalDeclarationsNode>();

    // Check if the current token is the first token for PROGRAM_PRIME
    while (isFirstTokenForProgramPrime()) // Continue as long as there are valid tokens
    {
        // Parse another set of global declarations
        auto additionalDeclarations = parseGlobalDeclarations();
        programPrimeNode->push_back_node(std::move(additionalDeclarations));
    }

    return programPrimeNode;
}

// GLOBAL_DECLARATIONS -> DECLARATION | FUNCTION_DEFINITION
std::unique_ptr<ASTNode> Parser::parseGlobalDeclarations()
{
    if (currentToken.getType() == TokenType::KEYWORD && currentToken.getValue() == "func")
    {
        auto declarationNode = std::make_unique<DeclarationNode>();
        declarationNode->setDeclaration(parseFunctionDefinition());
        return declarationNode;
    }

    if (isBasicType())
    {
        auto declarationNode = std::make_unique<DeclarationNode>();
        declarationNode->setDeclaration(parseDeclaration());
        return declarationNode;
    }

    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseFunctionDefinition()
{
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseDeclaration()
{
    return nullptr;
}
// helper functions:
bool Parser::isBasicType()
{
    std::string tokenValue = currentToken.getValue();
    return tokenValue == "bool" || tokenValue == "char" || tokenValue == "double" ||
           tokenValue == "float" || tokenValue == "int" || tokenValue == "long" ||
           tokenValue == "short" || tokenValue == "signed" || tokenValue == "struct" ||
           tokenValue == "unsigned" || tokenValue == "void";
}

bool Parser::isFirstTokenForProgramPrime()
{
    if (currentToken.getType() != TokenType::KEYWORD)
    {
        return false; // If it's not a keyword, return false
    }

    const std::unordered_set<std::string> validKeywords = {
        "bool", "char", "double", "float", "func",
        "int", "long", "short", "signed", "struct",
        "unsigned", "void"};

    // Check if the current token's value is in the set of valid keywords
    return validKeywords.find(currentToken.getValue()) != validKeywords.end();
}
