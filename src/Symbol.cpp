#include "Symbol.h"

VariableSymbol::VariableSymbol(std::string ident, std::shared_ptr<SemanticType> type,
                               std::optional<std::string> initializer)
    : type(std::move(type)), initializer(std::move(initializer))
{
    this->ident = std::move(ident);
}

std::string VariableSymbol::toString() const
{
    return type->toString() + " " + ident +
           (initializer ? " = " + *initializer : "") + ";";
}

std::shared_ptr<SemanticType> VariableSymbol::getType() const
{
    return type;
}

// FunctionSymbol implementation
FunctionSymbol::FunctionSymbol(std::string ident, std::shared_ptr<FunctionType> type)
    : type(std::move(type))
{
    this->ident = std::move(ident);
}

std::string FunctionSymbol::toString() const
{
    return type->toString() + " " + ident + ";";
}

std::shared_ptr<SemanticType> FunctionSymbol::getType() const
{
    return type;
}

// StructSymbol implementation
StructSymbol::StructSymbol(std::string ident, std::shared_ptr<StructType> type)
    : type(std::move(type))
{
    this->ident = std::move(ident);
}

std::string StructSymbol::toString() const
{
    return type->toString() + ";";
}

std::shared_ptr<SemanticType> StructSymbol::getType() const
{
    return type;
}