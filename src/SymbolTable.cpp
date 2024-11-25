#include "SymbolTable.h"
#include <iostream>

SymbolTable::SymbolTable(std::shared_ptr<SymbolTable> parent)
    : parent(std::move(parent))
{
}

bool SymbolTable::insertSymbol(std::shared_ptr<Symbol> symbol)
{
    auto result = symbols.emplace(symbol->getName(), symbol);
    return result.second;
}

std::shared_ptr<Symbol> SymbolTable::lookupSymbol(const std::string &name) const
{
    auto it = symbols.find(name);
    if (it != symbols.end())
    {
        return it->second;
    }
    if (parent)
    {
        return parent->lookupSymbol(name);
    }
    return nullptr;
}

std::shared_ptr<SymbolTable> SymbolTable::createChildScope()
{
    return std::make_shared<SymbolTable>(shared_from_this());
}

std::shared_ptr<SymbolTable> SymbolTable::getParent() const
{
    return parent;
}

void SymbolTable::printCurrentScope() const
{
    for (const auto &[name, symbol] : symbols)
    {
        std::cout << symbol->toString() << std::endl;
    }
}

std::shared_ptr<FunctionSymbol> SymbolTable::getFunctionSymbol() const
{
    for (const auto &[name, symbol] : symbols)
    {
        // Use dynamic_cast to check if the symbol is of type FunctionSymbol
        if (auto funcSymbol = std::dynamic_pointer_cast<FunctionSymbol>(symbol))
        {
            return funcSymbol;
        }
    }
    // Return nullptr if no FunctionSymbol exists in the scope
    return nullptr;
}