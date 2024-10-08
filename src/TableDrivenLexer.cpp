#include "TableDrivenLexer.h"


TableDrivenLexer::TableDrivenLexer(std::shared_ptr<std::fstream> sourceCodeStream) :sourceCodeStream(sourceCodeStream) {
    this->automata = Automata();
}

std::string removeSpaces(const std::string& token) {
    std::string cleanedToken = token;
    cleanedToken.erase(
        std::remove_if(cleanedToken.begin(), cleanedToken.end(), ::isspace),
        cleanedToken.end());
    return cleanedToken;
}

void TableDrivenLexer::tokenize() {
    char currentChar;
    int currentState = 0;  // Start state
    int lastAcceptedState = -1;  // Track the last valid (accepted) state
    std::string lastAcceptedToken = "";  // Track the token at the last accepted state
    std::streampos lastAcceptedPos;  // Position in the stream for backtracking
    int lineNumber = 1;

    TokenBuffer currentToken;

    while (sourceCodeStream->get(currentChar)) {
        if (currentChar == '\n')
        {
            lineNumber++; 
        }
        if (currentState == 0 && std::isspace(currentChar)) { // Skip whitespace when in state 0
            continue;  
        }

        int nextState = automata.makeTransition(currentState, currentChar);

        if (automata.isErrorState(nextState)) {
            if (lastAcceptedState == -1) {
                std::cerr << "Lexical Error: Unexpected character '" << currentChar << "' at state " << currentState << std::endl;
                return;
            }

            TokenType tokenType = StateTypeToTokenType(automata.getStateType(lastAcceptedState));
            tokens.push_back(Token(lastAcceptedToken, tokenType, lineNumber));
            //tokens.emplace_back(lastAcceptedToken, tokenType, lineNumber);

            sourceCodeStream->clear();
            sourceCodeStream->seekg(lastAcceptedPos);

            currentToken.clear();
            lastAcceptedToken.clear();

            //reset state
            currentState = 0;
            lastAcceptedState = -1;
            
        }
        else {
            currentToken.add(currentChar);
            currentState = nextState;
            lastAcceptedState = currentState;
            lastAcceptedToken = currentToken.getString();
            lastAcceptedPos = sourceCodeStream->tellg();  // Save stream position
        }
    }

    if(lastAcceptedState == -1){
        if( !currentToken.isEmpty()){
            std::cerr << "Lexical Error: Unexpected end of input while processing token '" << currentToken.getString() << "'." << std::endl;

        }
        return;
    }
    //if ended without processing
    TokenType tokenType = StateTypeToTokenType(automata.getStateType(lastAcceptedState));
    //tokens.emplace_back(lastAcceptedToken, tokenType, lineNumber);
    tokens.push_back(Token(lastAcceptedToken, tokenType, lineNumber));

}

TokenType TableDrivenLexer::StateTypeToTokenType(StateType stateType)
{
    switch (stateType)
    {
    case StateType::Keyword:
        return TokenType::KEYWORD;
    case StateType::Identifier:
        return TokenType::IDENTIFIER;
    case StateType::Punctuation:
        return TokenType::PUNCTUATION;
    case StateType::Constant:
        return TokenType::CONSTANT;
    case StateType::Operator:
        return TokenType::OPERATOR;
    case StateType::StringLiteral:
        return TokenType::STRING_LITERAL;
    case StateType::Comment:
        return TokenType::COMMENT;
    default:
        return TokenType::UNKNOWN;
    }
}


TokenTable &TableDrivenLexer::getTokens()
{
    return tokens;
}

