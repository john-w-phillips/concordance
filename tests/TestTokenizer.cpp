#include "Tokenizer.hpp"
#include <sstream>
#include "gtest/gtest.h"

namespace
{
  using namespace code_challenge;
  TEST(TestTokenizer, TestBasicText)
  {
    std::stringstream stream{"hello    world."};
    Tokenizer tokenizer;
    auto tokens = tokenizer.scan(stream);
    ASSERT_EQ(tokens.size(), 3);
    ASSERT_EQ(tokens.front(), "hello");
    tokens.pop_front();
    ASSERT_EQ(tokens.front(), "world");
    tokens.pop_front();
    ASSERT_EQ(tokens.front(), Tokenizer::SENTENCE_END);
  }

  TEST(TestTokenizer, TestSpecialWords)
  {
    std::stringstream stream{"special words: e.g. i.e. Q.E.D."};
    Tokenizer tokenizer;
    auto tokens = tokenizer.scan(stream);
    ASSERT_EQ(tokens.size(), 5);
    ASSERT_EQ(tokens, (std::list<std::string>{"special", "words", "e.g.", "i.e.", "Q.E.D."}));
  }
}
