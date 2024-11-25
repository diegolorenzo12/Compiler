#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <iostream>

// Base class for all types
class SemanticType
{
public:
    virtual ~SemanticType() = default;

    virtual std::string toString() const = 0;
    virtual bool equals(const SemanticType &other) const = 0;
    virtual bool isConst() const { return false; }
};

// Primitive types
enum class DataType
{
    INT,
    FLOAT,
    CHAR,
    BOOL,
    SHORT,
    DOUBLE,
    LONG,
    SIGNED,
    UNSIGNED,
    VOID,
    STRUCT,
    POINTER
};

inline std::string dataTypeToString(DataType type)
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

inline DataType stringToDataType(const std::string &str)
{
    if (str == "int")
    {
        return DataType::INT;
    }
    if (str == "float")
    {
        return DataType::FLOAT;
    }
    if (str == "char")
    {
        return DataType::CHAR;
    }
    if (str == "bool")
    {
        return DataType::BOOL;
    }
    if (str == "short")
    {
        return DataType::SHORT;
    }
    if (str == "double")
    {
        return DataType::DOUBLE;
    }
    if (str == "long")
    {
        return DataType::LONG;
    }
    if (str == "signed")
    {
        return DataType::SIGNED;
    }
    if (str == "unsigned")
    {
        return DataType::UNSIGNED;
    }
    if (str == "void")
    {
        return DataType::VOID;
    }
    if (str == "struct")
    {
        return DataType::STRUCT;
    }
    if (str == "pointer")
    {
        return DataType::POINTER;
    }
    return DataType::INT;
}

class PrimitiveSemanticType : public SemanticType
{
private:
    DataType name; // Name of the primitive type (e.g., "int", "float")
    bool isConstQualified;

public:
    explicit PrimitiveSemanticType(const DataType &name, bool isConstQualified = false)
        : name(name), isConstQualified(isConstQualified) {}

    std::string toString() const override
    {
        return (isConstQualified ? "const " : "") + dataTypeToString(name);
    }

    bool equals(const SemanticType &other) const override
    {
        const auto *otherPrimitive = dynamic_cast<const PrimitiveSemanticType *>(&other);
        return otherPrimitive && name == otherPrimitive->name;
    }

    bool isConst() const override { return isConstQualified; }
};

// Pointer types
class PointerType : public SemanticType
{
private:
    std::shared_ptr<SemanticType> pointeeType;
    bool isConstQualified;

public:
    explicit PointerType(std::shared_ptr<SemanticType> pointeeType, bool isConstQualified = false)
        : pointeeType(std::move(pointeeType)), isConstQualified(isConstQualified) {}

    std::string toString() const override
    {
        return pointeeType->toString() + (isConstQualified ? " const" : "") + "*";
    }

    std::shared_ptr<SemanticType> getPointeeType() const { return pointeeType; }

    bool equals(const SemanticType &other) const override
    {
        const auto *otherPointer = dynamic_cast<const PointerType *>(&other);
        return otherPointer && pointeeType->equals(*otherPointer->pointeeType);
    }

    bool isConst() const override { return isConstQualified; }
};

class ArrayType : public SemanticType
{
private:
    std::shared_ptr<SemanticType> elementType;
    size_t size;

public:
    ArrayType(std::shared_ptr<SemanticType> elementType, size_t size)
        : elementType(std::move(elementType)), size(size) {}

    std::string toString() const override
    {
        return elementType->toString() + "[" + std::to_string(size) + "]";
    }

    std::shared_ptr<SemanticType> getElementType() const { return elementType; }

    bool equals(const SemanticType &other) const override
    {
        const auto *otherArray = dynamic_cast<const ArrayType *>(&other);
        return otherArray && elementType->equals(*otherArray->elementType) && size == otherArray->size;
    }
};

class FunctionType : public SemanticType
{
private:
    std::shared_ptr<SemanticType> returnType;
    std::vector<std::shared_ptr<SemanticType>> paramTypes;

public:
    FunctionType(std::shared_ptr<SemanticType> returnType, std::vector<std::shared_ptr<SemanticType>> paramTypes)
        : returnType(std::move(returnType)), paramTypes(std::move(paramTypes)) {}

    FunctionType(std::shared_ptr<SemanticType> returnType)
        : returnType(std::move(returnType)) {}

    void addParamType(std::shared_ptr<SemanticType> paramType)
    {
        paramTypes.push_back(std::move(paramType));
    }

    const std::vector<std::shared_ptr<SemanticType>> &getParamTypes() const { return paramTypes; }

    std::shared_ptr<SemanticType> getReturnType() const { return returnType; }

    void addParameters(std::vector<std::shared_ptr<SemanticType>> params)
    {
        paramTypes = std::move(params);
    }

    std::string toString() const override
    {
        std::string result = returnType->toString() + "(";
        for (size_t i = 0; i < paramTypes.size(); ++i)
        {
            result += paramTypes[i]->toString();
            if (i != paramTypes.size() - 1)
            {
                result += ", ";
            }
        }
        result += ")";
        return result;
    }

    bool equals(const SemanticType &other) const override
    {
        const auto *otherFunc = dynamic_cast<const FunctionType *>(&other);
        if (!otherFunc || !returnType->equals(*otherFunc->returnType) ||
            paramTypes.size() != otherFunc->paramTypes.size())
        {
            return false;
        }
        for (size_t i = 0; i < paramTypes.size(); ++i)
        {
            if (!paramTypes[i]->equals(*otherFunc->paramTypes[i]))
            {
                return false;
            }
        }
        return true;
    }
};

class StructType : public SemanticType
{
private:
    std::string name;
    std::vector<std::pair<std::string, std::shared_ptr<SemanticType>>> fields;

public:
    explicit StructType(std::string name, std::vector<std::pair<std::string, std::shared_ptr<SemanticType>>> fields)
        : name(std::move(name)), fields(std::move(fields)) {}

    explicit StructType(std::string name)
        : name(std::move(name)) {}

    void addField(const std::string &fieldName, std::shared_ptr<SemanticType> fieldType)
    {
        // check that the field does not already exist before adding
        for (const auto &field : fields)
        {
            if (field.first == fieldName)
            {
                throw std::runtime_error("Field '" + fieldName + "' already exists in struct " + name);
            }
        }
        fields.emplace_back(fieldName, std::move(fieldType));
    }

    std::string toString() const override
    {
        std::string result = "struct " + name + " { ";
        for (const auto &field : fields)
        {
            result += field.second->toString() + " " + field.first + "; ";
        }
        result += "}";
        return result;
    }

    const std::string &getName() const { return name; }

    const std::vector<std::pair<std::string, std::shared_ptr<SemanticType>>> &getFields() const { return fields; }

    bool equals(const SemanticType &other) const override
    {
        const auto *otherStruct = dynamic_cast<const StructType *>(&other);
        if (!otherStruct || name != otherStruct->name || fields.size() != otherStruct->fields.size())
        {
            return false;
        }
        for (size_t i = 0; i < fields.size(); ++i)
        {
            if (fields[i].first != otherStruct->fields[i].first ||
                !fields[i].second->equals(*otherStruct->fields[i].second))
            {
                return false;
            }
        }
        return true;
    }
};
