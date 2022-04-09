#include "Tokenizer.hpp"

namespace code_challenge
{
  const std::string Tokenizer::SENTENCE_END = ".";
  const std::string Tokenizer::SENTENCE_ENDINGS = ".!?";  
  const std::list<std::string> Tokenizer::SPECIAL_WORDS{
    "i.e.",
    "e.g.",
    "Q.E.D."
  };
}
