#include <gtest/gtest.h>
#include "TableDrivenLexer.h"
#include <memory>
#include <fstream>
#include <sstream>

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
    const std::vector<Token> &tokens = lexer.getTokens();

    ASSERT_EQ(tokens.size(), 9); // Expect 9 tokens: int, main, (, ), {, return, 0, ;}

    EXPECT_EQ(tokens[0].getType(), TokenType::KEYWORD);
    EXPECT_EQ(tokens[0].getValue(), "int");

    EXPECT_EQ(tokens[1].getType(), TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].getValue(), "main");

    EXPECT_EQ(tokens[2].getType(), TokenType::PUNCTUATION);
    EXPECT_EQ(tokens[2].getValue(), "(");

    EXPECT_EQ(tokens[3].getType(), TokenType::PUNCTUATION);
    EXPECT_EQ(tokens[3].getValue(), ")");

    EXPECT_EQ(tokens[4].getType(), TokenType::PUNCTUATION);
    EXPECT_EQ(tokens[4].getValue(), "{");

    EXPECT_EQ(tokens[5].getType(), TokenType::KEYWORD);
    EXPECT_EQ(tokens[5].getValue(), "return");

    EXPECT_EQ(tokens[6].getType(), TokenType::CONSTANT);
    EXPECT_EQ(tokens[6].getValue(), "0");

    EXPECT_EQ(tokens[7].getType(), TokenType::PUNCTUATION);
    EXPECT_EQ(tokens[7].getValue(), ";");

    EXPECT_EQ(tokens[8].getType(), TokenType::PUNCTUATION);
    EXPECT_EQ(tokens[8].getValue(), "}");

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
    const std::vector<Token> &tokens = lexer.getTokens();

    ASSERT_EQ(tokens.size(), 30); // 30 total tokens


    for(auto i: tokens){
        EXPECT_EQ(i.getTypeAsString(), "KEYWORD");
    }

    stream->close();
    std::remove("temp_source_code.cmm");
}
