#ifndef _WORD_COMPILER_HPP_
#define _WORD_COMPILER_HPP_
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <list>
#include <sstream>
#include "Tokenizer.hpp"
namespace code_challenge
{
  class WordInformation
  {
  public:
    using SentenceList = std::list<unsigned>;
    WordInformation(const std::string& word):
      num_occurs{0},      
      word{word}
    {}

    const SentenceList&
    get_sentence_numbers() const
    {
      return sentence_nums;
    }

    void
    add_occur_in_sentence(unsigned sentence)
    {
      incr_num_occurs();
      sentence_nums.push_back(sentence);
    }
    

    void
    incr_num_occurs()
    {
      ++num_occurs;
    }

    unsigned
    get_num_occurs() const
    {
      return num_occurs;
    }

    std::string
    get_word() const
    {
      return word;
    }

    std::string
    get_report_string()
    {
      std::stringstream report{};
      report << word;
      report << " ";
      report << "{" << num_occurs << ":";
      for (auto i = sentence_nums.begin();
	   i != (--sentence_nums.end());
	   ++i)
      {
	report << *i;
	report << ",";
      }
      report << *sentence_nums.rbegin();
      report << "}";
      return report.str();
    }
  protected:
    SentenceList sentence_nums;
    unsigned num_occurs;
    std::string word;
  };

  

  class WordCompiler
  {
  public:
    WordCompiler(): words{}
    {}

    class WordIterator: public std::map<std::string,WordInformation>::iterator
    {
    public:
      using parent =  std::map<std::string,WordInformation>::iterator;

      WordIterator(parent init):
	parent(std::move(init))
      {}

      WordInformation&
      operator*()
      {
	return parent::operator*().second;
      }
      WordInformation *
      operator->()
      {
	return &parent::operator*().second;
      }      
    };

    std::string get_lowercase(const std::string& in)
    {
      std::string output;
      for (auto& i: in)
      {
	output.push_back(std::tolower(i));
      }
      return output;
    }

    void set_specialwords(std::istream& words_stream)
    {
      std::list<std::string> special_words;
      std::unordered_set<std::string> specials_cant_end_sentence;
      for (; words_stream;)
      {
	std::string line;
	std::getline(words_stream, line);
	if (line.empty() || line[0] == '#')
	  continue;
	if (line.find(':') != std::string::npos)
	{
	  std::string designator = line.substr(0, line.find(':'));
	  line = line.substr(line.find(':') + 1);	  
	  if (designator == "prefix" || designator == "nonender")
	    specials_cant_end_sentence.insert(line);
	  else
	  {
	    std::cerr << "Bad special line: " << line << " skipping..." << std::endl;
	    continue;
	  }

	}
	special_words.push_back(line);
      }
      tokenizer.set_special_words(special_words);
      tokenizer.set_specials_that_cant_end_sentences(specials_cant_end_sentence);
    }

    void compile_words_from_source(std::istream& char_stream)
    {
      auto token_stream = tokenizer.scan(char_stream);
      unsigned sentence = 1;
      for (const auto& i: token_stream)
      {
	if (i == Tokenizer::SENTENCE_END)
	  ++sentence;
	else
	{
	  std::string lcase_word = get_lowercase(i);
	  auto word_value = words.try_emplace(lcase_word, lcase_word);
	  auto iterator_to_word = word_value.first;
	  auto& word_reference = iterator_to_word->second;
	  word_reference.add_occur_in_sentence(sentence);
	}
      }
    }

    const WordInformation&
    get_info_for_word(const std::string& word)
    {
      return words.at(word);
    }

    WordIterator
    begin_words()
    {
      return WordIterator{words.begin()};
    }

    WordIterator
    end_words()
    {
      return WordIterator{words.end()};
    }


  protected:
    Tokenizer tokenizer;
    std::map<std::string,WordInformation> words;
  };
}
#endif
