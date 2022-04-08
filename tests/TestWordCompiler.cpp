#include "WordCompiler.hpp"
#include "gtest/gtest.h"

namespace
{
  using namespace code_challenge;
  TEST(TestSentenceRecorder, TestWordInfo)
  {
    WordInformation information{"word"};
    ASSERT_EQ(information.get_num_occurs(), 0);
    ASSERT_EQ(information.get_word(), "word");
    information.add_occur_in_sentence(1);
    ASSERT_EQ(information.get_num_occurs(), 1);
    ASSERT_EQ(information.get_sentence_numbers(),
	      (std::list<unsigned>{1,}));

    information.add_occur_in_sentence(5);
    ASSERT_EQ(information.get_num_occurs(), 2);
    ASSERT_EQ(information.get_sentence_numbers(),
	      (std::list<unsigned>{1,5}));

  }

  const std::string PROMPT_TEXT{
      "Given an arbitrary text document written in English, write a program that will generate a\n"
      "concordance, i.e. an alphabetical list of all word occurrences, labeled with word frequences.\n\n"
      "Bonus: label each word with the sentence numbers in which each occurrence appeared.\n"};
  
  TEST(TestSentenceRecorder, TestSimpleText)
  {
    WordCompiler compiler;
    std::stringstream stream{PROMPT_TEXT};
    compiler.compile_words_from_source(stream);
    auto& info = compiler.get_info_for_word("a");
    ASSERT_EQ(info.get_num_occurs(), 2);
    ASSERT_EQ(info.get_word(), "a");
    ASSERT_EQ(info.get_sentence_numbers(), (std::list<unsigned>{1, 1}));
  }

  const std::list<std::string> EXAMPLE_CONCORDNACE_LIST{
    "a {2:1,1}",
    "all {1:1}",
    "alphabetical {1:1}",
    "an {2:1,1}",
    "appeared {1:2}",
    "arbitrary {1:1}",
    "bonus {1:2}",
    "concordance {1:1}",
    "document {1:1}",
    "each {1:2}",
    "english {1:1}",
    "frequencies {1:1}",
    "generate {1:1}",
    "given {1:1}",
    "i.e. {1:1}",
    "in {2:1,2}",
    "it {1:2}",
    "label {1:2}",
    "labeled {1:1}",
    "list {1:1}",
    "numbers {1:2}",
    "occurrences {1:1}",
    "of {1:1}",
    "program {1:1}",
    "sentence {1:2}",
    "text {1:1}",
    "that {1:1}",
    "the {1:2}",
    "which {1:2}",
    "will {1:1}",
    "with {2:1,2}",
    "word {3:1,1,2}",
    "write {1:1}",
    "written {1:1}",
  };
  TEST(TestSentenceRecorder, TestOutput)
  {
    WordCompiler compiler;
    std::stringstream stream{PROMPT_TEXT};
    compiler.compile_words_from_source(stream);
    auto correct_text = EXAMPLE_CONCORDNACE_LIST.begin();
    for (auto i = compiler.begin_words();
	 i != compiler.end_words();
	 ++i)
    {
      std::cout << "text: " << i->second.get_report_string() << std::endl;
      ASSERT_EQ(i->second.get_report_string(), *correct_text);
      ++correct_text;
    }
  }
}
