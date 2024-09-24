#include <gtest/gtest.h>
#include "TableDrivenLexer.h"
#include "TokenTable.h"
#include "Token.h"
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>


TEST(LexerTest, BasicKeywordAndIdentifier)
{
    std::string sourceCode = "int main() { return 0; }";
    std::shared_ptr<std::fstream> stream = std::make_shared<std::fstream>();
    stream->open("temp_source_code.txt", std::ios::out);
    *stream << sourceCode;
    stream->close();

    stream->open("temp_source_code.txt", std::ios::in);

    TableDrivenLexer lexer(stream);
    lexer.tokenize();
    TokenTable &tokens = lexer.getTokens(); 

    ASSERT_EQ(tokens.size(), 9); // Expect 9 tokens: int, main, (, ), {, return, 0, ;}

    EXPECT_EQ(tokens.front().getType(), TokenType::KEYWORD);
    EXPECT_EQ(tokens.front().getValue(), "int");
    tokens.pop_front();

    EXPECT_EQ(tokens.front().getType(), TokenType::IDENTIFIER);
    EXPECT_EQ(tokens.front().getValue(), "main");
    tokens.pop_front();

    EXPECT_EQ(tokens.front().getType(), TokenType::PUNCTUATION);
    EXPECT_EQ(tokens.front().getValue(), "(");
    tokens.pop_front();

    EXPECT_EQ(tokens.front().getType(), TokenType::PUNCTUATION);
    EXPECT_EQ(tokens.front().getValue(), ")");
    tokens.pop_front();

    EXPECT_EQ(tokens.front().getType(), TokenType::PUNCTUATION);
    EXPECT_EQ(tokens.front().getValue(), "{");
    tokens.pop_front();

    EXPECT_EQ(tokens.front().getType(), TokenType::KEYWORD);
    EXPECT_EQ(tokens.front().getValue(), "return");
    tokens.pop_front();

    EXPECT_EQ(tokens.front().getType(), TokenType::CONSTANT);
    EXPECT_EQ(tokens.front().getValue(), "0");
    tokens.pop_front();

    EXPECT_EQ(tokens.front().getType(), TokenType::PUNCTUATION);
    EXPECT_EQ(tokens.front().getValue(), ";");
    tokens.pop_front();

    EXPECT_EQ(tokens.front().getType(), TokenType::PUNCTUATION);
    EXPECT_EQ(tokens.front().getValue(), "}");

    // Clean up temporary file
    stream->close();
    std::remove("temp_source_code.txt");
}

TEST(LexerTest, KeywordTest){
    std::string sourceCode = "auto break case const continue default do double else extern float for goto if int long register return short signed sizeof static struct switch typedef union unsigned void volatile while";
    std::shared_ptr<std::fstream> stream = std::make_shared<std::fstream>();
    stream->open("temp_source_code.cmm", std::ios::out);
    *stream << sourceCode;
    stream->close();

    stream->open("temp_source_code.cmm", std::ios::in);
    TableDrivenLexer lexer(stream);

    lexer.tokenize();

    TokenTable &tokens = lexer.getTokens();

    ASSERT_EQ(tokens.size(), 30); // 30 total tokens

    while(!tokens.isEmpty()){

        EXPECT_EQ(tokens.front().getTypeAsString(), "KEYWORD");
        tokens.pop_front();
    }


    stream->close();
    std::remove("temp_source_code.cmm");
}
