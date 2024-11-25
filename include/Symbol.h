#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <variant>
#include "Type.h"

// enum class StorageClass
// {
//     AUTO,
//     STATIC,
//     REGISTER,
//     NONE
// };
class Symbol
{
protected:
    std::string ident;

public:
    virtual ~Symbol() = default;

    virtual std::string toString() const = 0;

    virtual std::shared_ptr<SemanticType> getType() const = 0;

    std::string getName() const { return ident; }
};

// Represents a variable symbol
class VariableSymbol : public Symbol
{
private:
    std::shared_ptr<SemanticType> type;
    std::optional<std::string> initializer;

public:
    explicit VariableSymbol(std::string ident, std::shared_ptr<SemanticType> type,
                            std::optional<std::string> initializer = std::nullopt);

    std::string toString() const override;

    std::shared_ptr<SemanticType> getType() const override;
};

// Represents a function symbol
class FunctionSymbol : public Symbol
{
private:
    std::shared_ptr<FunctionType> type;

public:
    explicit FunctionSymbol(std::string ident, std::shared_ptr<FunctionType> type);

    std::string toString() const override;

    std::shared_ptr<SemanticType> getType() const override;
};

// Represents a struct symbol
class StructSymbol : public Symbol
{
private:
    std::shared_ptr<StructType> type;

public:
    explicit StructSymbol(std::string ident, std::shared_ptr<StructType> type);

    std::string toString() const override;

    std::shared_ptr<SemanticType> getType() const override;
};