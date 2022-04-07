#include "Tokenizer.hpp"

namespace code_challenge
{
  const std::string Tokenizer::SENTENCE_END = ".";
  const std::list<std::string> Tokenizer::SPECIAL_CHARS{
    "i.e.",
    "e.g.",
    "Q.E.D."
  };
}
