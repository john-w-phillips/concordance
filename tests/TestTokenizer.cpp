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
    std::stringstream stream{"special words:\n e.g. i.e. Q.E.D."};
    Tokenizer tokenizer;
    auto tokens = tokenizer.scan(stream);
    ASSERT_EQ(tokens.size(), 5);
    ASSERT_EQ(tokens, (std::list<std::string>{"special", "words", "e.g.", "i.e.", "Q.E.D."}));
  }

  TEST(TestTokenizer, TestSpecialWordsFlexible)
  {
    std::stringstream stream{"special words:\n e.g. M.S., B.S., D.D.S., P.H.D."};
    Tokenizer tokenizer;
    std::list<std::string> new_special{
      "P.H.D.",
      "M.S.",
      "B.S.",
      "D.D.S.",
    };
    tokenizer.set_special_words(new_special);
    auto tokens = tokenizer.scan(stream);
    ASSERT_EQ(tokens.size(), 10);
    ASSERT_EQ(tokens, (std::list<std::string>{"special", "words", "e", ".", "g", ".", "M.S.", "B.S.", "D.D.S.", "P.H.D."}));
  }  
}
