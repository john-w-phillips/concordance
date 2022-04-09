#include "Tokenizer.hpp"

namespace code_challenge
{
  const std::string Tokenizer::SENTENCE_END = ".";
  const std::string Tokenizer::SENTENCE_ENDINGS = ".!?";

  const std::unordered_set<std::string> Tokenizer::SPECIALS_THAT_CANT_END_SENTENCES{
    "e.g.",
    "n.b.",
    "i.e.",
    "Mr.",
    "Mrs.",
    "Ms.",
    "Mme.",
    "Dr.",
    "Fr.",
    "Gov.",
    "Pres.",
    "Prof.",
    "Sen.",      
  };
  const std::list<std::string> Tokenizer::SPECIAL_WORDS{
    "i.e.",
      "e.g.",
      "Q.E.D.",
      "Mr.",
      "Mrs.",
      "Ph.D.",
      "M.D.",
      "Fr.",
      "Dr.",
      "Gov.",
      "Pres.",
      "Sen.",
      "Prof.",
      "Ms.",
      "mme.",
  };
}
