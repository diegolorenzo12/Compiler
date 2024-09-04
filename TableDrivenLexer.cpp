#include "TableDrivenLexer.h"
#include <algorithm>


TableDrivenLexer::TableDrivenLexer(std::shared_ptr<std::fstream> sourceCodeStream) :sourceCodeStream(sourceCodeStream) {

    /*
    INITIALIZE STATE TYPES 
    */
    for (auto i : this->stateTypes) {
        i = StateType::Error;
    }

    stateTypes[0] = StateType::Start;
    stateTypes[1] = StateType::Keyword;
    stateTypes[2] = StateType::Identifier;
    stateTypes[3] = StateType::Punctuation;
    stateTypes[4] = StateType::Constant;
    stateTypes[5] = StateType::Operator;
    stateTypes[6] = StateType::Error;
    stateTypes[7] = StateType::StringLiteral;
    stateTypes[8] = StateType::Comment;

    stateTypes[127] = StateType::Operator;
    stateTypes[11] = StateType::Operator;
    stateTypes[12] = StateType::Operator;
    stateTypes[13] = StateType::Operator;
    stateTypes[14] = StateType::Operator;
    stateTypes[15] = StateType::Operator;
    stateTypes[16] = StateType::Operator;
    stateTypes[17] = StateType::Operator;
    stateTypes[18] = StateType::Operator;
    stateTypes[19] = StateType::Operator;

    //in between all keywords before being one, example "doubl" before having the last "e"
    for (int i = 21; i <= 126; i++) {
        stateTypes[i] = StateType::Identifier;
    }
    //overwrite q44 do keyword, because "do" and "double" overlap
    stateTypes[44] = StateType::Keyword;



    /*
    INITIALIZE TRANSITION TABLE
    */
    for (int i = 0; i < NUM_STATES; ++i) {
        for (int j = 0; j < ASCII_SIZE; ++j) {
            transitionTable[i][j] = 6; //everthing transitions to error at the start
        }
    }
    transitionTable[0][' '] = 0;
    transitionTable[0]['\n'] = 0;


    //NUMERIC CONSTANTS
    addTransitionForSpecificChars(0, 4, { '0', '1', '2', '3', '4' , '5' , '6' , '7' , '8' , '9' });
    addTransitionForSpecificChars(4, 4, { '0', '1', '2', '3', '4' , '5' , '6' , '7' , '8' , '9' });

    //STRING LITERALS
    transitionTable[0]['"'] = 9;
    addTransitionExcludingChars(9, 9, { '"' });
    transitionTable[9]['"'] = 7;

    //COMMENTS 
    transitionTable[0]['/'] = 127;

    //single line
    transitionTable[127]['/'] = 128;
    addTransitionExcludingChars(128, 128, { '\n' });
    transitionTable[128]['\n'] = 8;

    //multiline
    transitionTable[127]['*'] = 129;
    addTransitionExcludingChars(129, 129, { '*' });
    transitionTable[129]['*'] = 130;
    addTransitionExcludingChars(130, 129, { '*', '/' });
    transitionTable[130]['*'] = 130;
    transitionTable[130]['/'] = 8;


    //PUNCTUATION
    addTransitionForSpecificChars(0, 3, { '{' , '}' , '[' , ']' , '(' , ')', ';' });


    //OPERATORS

    // !, *, %, ^, =
    addTransitionForSpecificChars(0, 11, { '!' , '*' , '%' , '^' , '=' });

    //!=, *=, %=,^=, ==
    transitionTable[11]['='] = 5;
    
    //+, +=, ++
    transitionTable[0]['+'] = 12;
    transitionTable[12]['+'] = 5;
    transitionTable[12]['='] = 5;

    //?, ., ~, ,
    addTransitionForSpecificChars(0, 5, { '?' , '.' , '~' , ',' });

    //-, --, -=
    transitionTable[0]['-'] = 13;
    transitionTable[13]['-'] = 5;
    transitionTable[13]['='] = 5;

    //&, &&, &=
    transitionTable[0]['&'] = 14;
    transitionTable[14]['&'] = 5;
    transitionTable[14]['='] = 5;

    // |, ||
    transitionTable[0]['|'] = 15;
    transitionTable[15]['|'] = 5;

    // <, <<, <=, <<=
    transitionTable[0]['<'] = 16;
    transitionTable[16]['<'] = 18;
    transitionTable[16]['='] = 5;
    transitionTable[18]['='] = 5;

    // >, >>, >=, >>=
    transitionTable[0]['>'] = 17;
    transitionTable[17]['>'] = 19;
    transitionTable[17]['='] = 5;
    transitionTable[19]['='] = 5;

    //::
    transitionTable[0][':'] = 20;
    transitionTable[20][':'] = 5;


    /*
    IDENTIFIER
    */
    // Identifiers start with alpha char or underscore, later this will be overwiten by keywords.
    for (int c = 0; c < ASCII_SIZE; ++c) {
        char currentChar = static_cast<char>(c);
        if (std::isalpha(currentChar) || currentChar == '_') {
            this->transitionTable[0][static_cast<unsigned char>(currentChar)] = 2;
        }
    }

    // Identifiers start with alpha char or underscore, later this will be overwiten by keywords.
    for (int c = 0; c < ASCII_SIZE; ++c) {
        char currentChar = static_cast<char>(c);
        if (std::isalnum(currentChar) || currentChar == '_') {
            this->transitionTable[2][static_cast<unsigned char>(currentChar)] = 2;
        }
    }

    //all states from 21 to 126 should point to indentifier given an alphhum or undersore character, this will be later overwiiten for keywords. 
    for (int state = 21; state <= 126; ++state) {
        for (int c = 0; c < ASCII_SIZE; ++c) {
            char currentChar = static_cast<char>(c);
            if (std::isalnum(currentChar) || currentChar == '_') {
                transitionTable[state][static_cast<unsigned char>(currentChar)] = 2;
            }
        }
    }


    /*
    KEYWORDS
    */


    //auto
    transitionTable[0]['a'] = 21;
    transitionTable[21]['u'] = 22;
    transitionTable[22]['t'] = 23;
    transitionTable[23]['0'] = 1;


    //break
    transitionTable[0]['b'] = 24;
    transitionTable[24]['r'] = 25;
    transitionTable[25]['e'] = 26;
    transitionTable[26]['a'] = 27;
    transitionTable[27]['k'] = 1;


    //case
    transitionTable[0]['c'] = 28;
    transitionTable[28]['a'] = 29;
    transitionTable[29]['s'] = 30;
    transitionTable[30]['e'] = 1;

    //continue
    transitionTable[0]['c'] = 28; //duplicate
    transitionTable[28]['o'] = 31;
    transitionTable[31]['n'] = 32;
    transitionTable[32]['t'] = 34;
    transitionTable[34]['i'] = 35;
    transitionTable[35]['n'] = 36;
    transitionTable[36]['u'] = 37;
    transitionTable[37]['e'] = 1;

    //const 
    transitionTable[0]['c'] = 28; //ducplicate
    transitionTable[28]['o'] = 31; //duplicate
    transitionTable[31]['n'] = 32; //duplicate
    transitionTable[32]['s'] = 33;
    transitionTable[33]['t'] = 1;

    //default
    transitionTable[0]['d'] = 38;
    transitionTable[38]['e'] = 39;
    transitionTable[39]['f'] = 40;
    transitionTable[40]['a'] = 41;
    transitionTable[41]['u'] = 42;
    transitionTable[42]['l'] = 43;
    transitionTable[43]['t'] = 1;


    //do
    transitionTable[0]['d'] = 38; //ducplicate
    transitionTable[38]['o'] = 44;

    //double
    transitionTable[0]['d'] = 38; //ducplicate
    transitionTable[38]['o'] = 44; //ducplicate
    transitionTable[44]['u'] = 45;
    transitionTable[45]['b'] = 46;
    transitionTable[46]['l'] = 47;
    transitionTable[47]['e'] = 1;

    //else
    transitionTable[0]['e'] = 48;
    transitionTable[48]['l'] = 49;
    transitionTable[49]['s'] = 50;
    transitionTable[50]['e'] = 1;


    //extern
    transitionTable[0]['e'] = 48;
    transitionTable[48]['x'] = 51;
    transitionTable[52]['t'] = 52;
    transitionTable[53]['e'] = 53;
    transitionTable[54]['r'] = 54;
    transitionTable[1]['n'] = 1;

    //float
    transitionTable[0]['f'] = 55;
    transitionTable[55]['l'] = 56;
    transitionTable[56]['o'] = 57;
    transitionTable[57]['a'] = 58;
    transitionTable[58]['t'] = 1;



    //for
    transitionTable[0]['f'] = 55;
    transitionTable[55]['o'] = 59;
    transitionTable[59]['r'] = 1;


    //goto
    transitionTable[0]['g'] = 60;
    transitionTable[60]['o'] = 61;
    transitionTable[61]['t'] = 62;
    transitionTable[62]['o'] = 1;


    //if
    transitionTable[0]['i'] = 63;
    transitionTable[63]['f'] = 1;


    //int
    transitionTable[0]['i'] = 63;
    transitionTable[63]['n'] = 64;
    transitionTable[64]['t'] = 1;

    //long
    transitionTable[0]['l'] = 65;
    transitionTable[65]['o'] = 66;
    transitionTable[66]['n'] = 67;
    transitionTable[67]['g'] = 1;


    //return
    transitionTable[0]['r'] = 68;
    transitionTable[68]['e'] = 69;
    transitionTable[69]['t'] = 75;
    transitionTable[75]['u'] = 76;
    transitionTable[76]['r'] = 77;
    transitionTable[77]['n'] = 1;

    //register
    transitionTable[0]['r'] = 68;
    transitionTable[68]['e'] = 69;
    transitionTable[69]['g'] = 70;
    transitionTable[70]['i'] = 71;
    transitionTable[71]['s'] = 72;
    transitionTable[72]['t'] = 73;
    transitionTable[73]['e'] = 74;
    transitionTable[74]['r'] = 1;



    //short
    transitionTable[0]['s'] = 78;
    transitionTable[78]['h'] = 79;
    transitionTable[79]['o'] = 80;
    transitionTable[80]['r'] = 81;
    transitionTable[81]['t'] = 1;


    //signed
    transitionTable[0]['s'] = 78;
    transitionTable[78]['i'] = 82;
    transitionTable[82]['g'] = 83;
    transitionTable[83]['n'] = 84;
    transitionTable[84]['e'] = 85;
    transitionTable[85]['d'] = 1;


    //sizeof
    transitionTable[0]['s'] = 78;
    transitionTable[78]['i'] = 82;
    transitionTable[82]['z'] = 86;
    transitionTable[86]['e'] = 87;
    transitionTable[87]['o'] = 88;
    transitionTable[88]['f'] = 1;


    //static
    transitionTable[0]['s'] = 78;
    transitionTable[78]['t'] = 89;
    transitionTable[89]['a'] = 90;
    transitionTable[90]['t'] = 91;
    transitionTable[91]['i'] = 92;
    transitionTable[92]['c'] = 1;

    //struct
    transitionTable[0]['s'] = 78;
    transitionTable[78]['t'] = 89;
    transitionTable[89]['r'] = 93;
    transitionTable[93]['u'] = 94;
    transitionTable[94]['c'] = 95;
    transitionTable[95]['t'] = 1;



    //switch
    transitionTable[0]['s'] = 78;
    transitionTable[78]['w'] = 96;
    transitionTable[96]['i'] = 97;
    transitionTable[97]['t'] = 98;
    transitionTable[98]['c'] = 99;
    transitionTable[99]['h'] = 1;


    //typedef
    transitionTable[0]['t'] = 100;
    transitionTable[100]['y'] = 101;
    transitionTable[101]['p'] = 102;
    transitionTable[102]['e'] = 103;
    transitionTable[103]['d'] = 104;
    transitionTable[104]['e'] = 105;
    transitionTable[105]['f'] = 1;


    //union
    transitionTable[0]['u'] = 106;
    transitionTable[106]['n'] = 107;
    transitionTable[107]['i'] = 108;
    transitionTable[108]['o'] = 109;
    transitionTable[109]['n'] = 1;

    //unsigned
    transitionTable[0]['u'] = 106;
    transitionTable[106]['n'] = 107;
    transitionTable[107]['s'] = 110;
    transitionTable[110]['i'] = 111;
    transitionTable[111]['g'] = 112;
    transitionTable[112]['n'] = 113;
    transitionTable[113]['e'] = 114;
    transitionTable[114]['d'] = 1;


    //volatile
    transitionTable[0]['v'] = 115;
    transitionTable[115]['o'] = 116;
    transitionTable[116]['l'] = 118;
    transitionTable[118]['a'] = 119;
    transitionTable[119]['t'] = 120;
    transitionTable[120]['i'] = 121;
    transitionTable[121]['l'] = 122;
    transitionTable[122]['e'] = 1;


    //void
    transitionTable[0]['v'] = 115;
    transitionTable[115]['o'] = 116;
    transitionTable[116]['i'] = 117;
    transitionTable[117]['d'] = 1;


    //white
    transitionTable[0]['w'] = 123;
    transitionTable[123]['h'] = 124;
    transitionTable[124]['i'] = 125;
    transitionTable[125]['l'] = 126;
    transitionTable[126]['e'] = 1;


}



void TableDrivenLexer::addTransitionExcludingChars(int sourceState, int destinationState, const std::vector<char>& excludedChars) {
    // Loop through all ASCII characters
    for (int c = 0; c < ASCII_SIZE; ++c) {
        char currentChar = static_cast<char>(c);  // Convert ASCII code to character
        // If the character is not in the excluded list, add the transition
        if (std::find(excludedChars.begin(), excludedChars.end(), currentChar) == excludedChars.end()) {
            this->transitionTable[sourceState][c] = destinationState;
        }
    }
}

void TableDrivenLexer::addTransitionForSpecificChars(int sourceState, int destinationState, const std::vector<char>& specificChars) {
    for (char c : specificChars) {
        this->transitionTable[sourceState][static_cast<unsigned char>(c)] = destinationState;
    }
}


std::string TableDrivenLexer::getTokenTypeString(StateType stateType) {
    switch (stateType) {
    case StateType::Keyword: return "Keyword";
    case StateType::Identifier: return "Identifier";
    case StateType::Constant: return "Constant";
    case StateType::Operator: return "Operator";
    case StateType::Punctuation: return "Punctuation";
    case StateType::StringLiteral: return "StringLiteral";
    case StateType::Comment: return "Comment";
    case StateType::Error: return "Error";
    default: return "Unknown";
    }
}




std::string removeSpaces(const std::string& token) {
    std::string cleanedToken = token;
    cleanedToken.erase(
        std::remove_if(cleanedToken.begin(), cleanedToken.end(), ::isspace),
        cleanedToken.end());
    return cleanedToken;
}

void TableDrivenLexer::tokenize() {
    if (!sourceCodeStream || !sourceCodeStream->is_open()) {
        std::cerr << "Error: Unable to read from source stream." << std::endl;
        return;
    }

    char currentChar;
    int currentState = 0;  // Start state
    int lastAcceptedState = -1;  // Track the last valid (accepted) state
    std::string currentToken = "";  // Accumulate characters for the current token
    std::string lastAcceptedToken = "";  // Track the token at the last accepted state
    std::streampos lastAcceptedPos;  // Position in the stream for backtracking

    while (sourceCodeStream->get(currentChar)) {
        if (currentState == 0 && std::isspace(currentChar)) {
            continue;  // Skip whitespace when in state 0
        }
        // Get the next state from the transition table
        int nextState = transitionTable[currentState][static_cast<unsigned char>(currentChar)];

        // If the next state is an error, we need to backtrack
        if (nextState == static_cast<int>(State::Error)) {
            if (lastAcceptedState != -1) {
                // Backtrack: Use the last accepted token and state
                std::string finalToken = lastAcceptedToken;
                if (stateTypes[lastAcceptedState] != StateType::StringLiteral && stateTypes[lastAcceptedState] != StateType::Comment) {
                    finalToken = removeSpaces(finalToken);
                }

                std::cout << "Token: " << finalToken << ", Type: " << getTokenTypeString(stateTypes[lastAcceptedState]) << std::endl;

                // Reset stream to the last accepted position
                sourceCodeStream->clear();
                sourceCodeStream->seekg(lastAcceptedPos);

                // Clear current token and reset state
                currentToken.clear();
                currentState = 0;
                lastAcceptedState = -1;  // Reset accepted state
            }
            else {
                // No accepted state found, throw an error
                std::cerr << "Lexical Error: Unexpected character '" << currentChar << "' at state " << currentState << std::endl;
                return;
            }
        }
        else {
            // Append the character to the current token
            currentToken += currentChar;
            currentState = nextState;

            // Check if the next state is an accepted state
            if (stateTypes[currentState] != StateType::Error && stateTypes[currentState] != StateType::Start) {
                // Update last accepted state and token
                lastAcceptedState = currentState;
                lastAcceptedToken = currentToken;
                lastAcceptedPos = sourceCodeStream->tellg();  // Save stream position
            }
        }
    }

    // Handle any remaining accepted token at the end of the input
    if (lastAcceptedState != -1) {
        std::string finalToken = lastAcceptedToken;

        // Remove spaces if not a string literal
        if (stateTypes[lastAcceptedState] != StateType::StringLiteral && stateTypes[lastAcceptedState] != StateType::Comment) {
            finalToken = removeSpaces(finalToken);
        }
        std::cout << "Token: " << finalToken << ", Type: " << getTokenTypeString(stateTypes[lastAcceptedState]) << std::endl;
    }
    else if (!currentToken.empty()) {
        // Handle error if we have leftover tokens with no accepted state
        std::cerr << "Lexical Error: Unexpected end of input while processing token '" << currentToken << "'." << std::endl;
    }
}