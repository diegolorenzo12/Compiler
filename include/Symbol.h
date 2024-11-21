#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <variant>

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

enum class StorageClass
{
    AUTO,
    STATIC,
    REGISTER,
    NONE
};

std::string dataTypeToString(DataType type);

class Symbol
{
protected:
    std::string ident;

public:
    virtual ~Symbol() = default;
    virtual std::string toString() const = 0;
    virtual DataType getType() const = 0;
    std::string getName() const;

    virtual std::optional<std::shared_ptr<Symbol>> getPointedToType() const { return std::nullopt; }
};

class VariableSymbol : public Symbol
{
private:
    DataType type;
    StorageClass storageClass;
    bool isConst;
    std::optional<std::string> initializer;
    std::optional<size_t> arraySize;

public:
    explicit VariableSymbol(std::string ident, DataType type, StorageClass storageClass, bool isConst,
                            std::optional<std::string> initializer = std::nullopt,
                            std::optional<size_t> arraySize = std::nullopt);

    std::string toString() const override;
    DataType getType() const override;

private:
    std::string getTypeAsString() const;
};

class FunctionSymbol : public Symbol
{
private:
    DataType returnType;
    std::vector<VariableSymbol> params;

public:
    explicit FunctionSymbol(std::string ident, DataType returnType, std::vector<VariableSymbol> params);

    std::string toString() const override;
    DataType getType() const override;

private:
    std::string returnTypeAsString() const;
};

class StructSymbol : public Symbol
{
private:
    std::vector<std::shared_ptr<Symbol>> fields;

public:
    explicit StructSymbol(std::string ident, std::vector<std::shared_ptr<Symbol>> fields);

    std::string toString() const override;
    DataType getType() const override;
};
