#include "Symbol.h"
#include <sstream>

std::string dataTypeToString(DataType type)
{
    switch (type)
    {
    case DataType::INT:
        return "int";
    case DataType::FLOAT:
        return "float";
    case DataType::CHAR:
        return "char";
    case DataType::BOOL:
        return "bool";
    case DataType::SHORT:
        return "short";
    case DataType::DOUBLE:
        return "double";
    case DataType::LONG:
        return "long";
    case DataType::SIGNED:
        return "signed";
    case DataType::UNSIGNED:
        return "unsigned";
    case DataType::VOID:
        return "void";
    case DataType::STRUCT:
        return "struct";
    case DataType::POINTER:
        return "pointer";
    default:
        return "unknown";
    }
}

std::string Symbol::getName() const
{
    return ident;
}

VariableSymbol::VariableSymbol(std::string ident, DataType type, StorageClass storageClass, bool isConst,
                               std::optional<std::string> initializer, std::optional<size_t> arraySize)
    : type(type), storageClass(storageClass), isConst(isConst), initializer(initializer), arraySize(arraySize)
{
    this->ident = std::move(ident);
}

std::string VariableSymbol::toString() const
{
    std::stringstream ss;
    ss << getTypeAsString() << " " << ident;
    if (arraySize)
    {
        ss << "[" << *arraySize << "]";
    }
    if (initializer)
    {
        ss << " = " << *initializer;
    }
    return ss.str();
}

DataType VariableSymbol::getType() const
{
    return type;
}

std::string VariableSymbol::getTypeAsString() const
{
    return dataTypeToString(type);
}

FunctionSymbol::FunctionSymbol(std::string ident, DataType returnType, std::vector<VariableSymbol> params)
    : returnType(returnType), params(std::move(params))
{
    this->ident = std::move(ident);
}

std::string FunctionSymbol::toString() const
{
    std::stringstream ss;
    ss << returnTypeAsString() << " " << ident << "(";
    for (size_t i = 0; i < params.size(); ++i)
    {
        if (i != 0)
            ss << ", ";
        ss << params[i].toString();
    }
    ss << ")";
    return ss.str();
}

DataType FunctionSymbol::getType() const
{
    return returnType;
}

std::string FunctionSymbol::returnTypeAsString() const
{
    return dataTypeToString(returnType);
}

StructSymbol::StructSymbol(std::string ident, std::vector<std::shared_ptr<Symbol>> fields)
    : fields(std::move(fields))
{
    this->ident = std::move(ident);
}

std::string StructSymbol::toString() const
{
    std::stringstream ss;
    ss << "struct " << ident << " {";
    for (size_t i = 0; i < fields.size(); ++i)
    {
        if (i != 0)
            ss << ", ";
        ss << fields[i]->toString();
    }
    ss << "}";
    return ss.str();
}

DataType StructSymbol::getType() const
{
    return DataType::STRUCT;
}
