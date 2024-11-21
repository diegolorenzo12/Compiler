#pragma once

#include "Symbol.h"
#include <unordered_map>
#include <memory>
#include <string>

class SymbolTable : public std::enable_shared_from_this<SymbolTable>
{
private:
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols;
    std::shared_ptr<SymbolTable> parent;

public:
    explicit SymbolTable(std::shared_ptr<SymbolTable> parent = nullptr);

    bool insertSymbol(std::shared_ptr<Symbol> symbol);

    std::shared_ptr<Symbol> lookupSymbol(const std::string &name) const;

    std::shared_ptr<SymbolTable> createChildScope();

    std::shared_ptr<SymbolTable> getParent() const;

    void printCurrentScope() const;
};
