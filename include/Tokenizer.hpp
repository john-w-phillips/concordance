#ifndef _TOKENIZER_HPP_
#define _TOKENIZER_HPP_
#include <list>
#include <string>
#include <iostream>
#include <locale>
#include <algorithm>
#include <unordered_set>

namespace concordance
{
  class Tokenizer
  {
  public:
    const static std::string SENTENCE_END;
    Tokenizer():
      is_current_token_special{false},
      last_token_may_have_ended_sentence{false},
      special_words{SPECIAL_WORDS},
      specials_that_cant_end_sentences{SPECIALS_THAT_CANT_END_SENTENCES}
    {}

    const std::list<std::string>&
    scan(std::istream& stream)
    {
      tokens.clear();
      current_token.clear();
      is_current_token_special = false;
      last_token_may_have_ended_sentence = false;
      tokenize_inner(stream);
      return tokens;
    }

    
    void set_special_words(const std::list<std::string>& new_words)
    {
      special_words = new_words;
    }

    void
    set_specials_that_cant_end_sentences(const std::unordered_set<std::string>& newset)
    {
      specials_that_cant_end_sentences = newset;
    }

    const std::list<std::string>&
    get_special_words()
    {
      return special_words;
    }

    const std::unordered_set<std::string>&
    get_specials_that_cant_end_sentences()
    {
      return specials_that_cant_end_sentences;
    }

  protected:
    std::list<std::string> special_words;
    std::unordered_set<std::string> specials_that_cant_end_sentences;    
    const static std::list<std::string> SPECIAL_WORDS;
    const static std::unordered_set<std::string> SPECIALS_THAT_CANT_END_SENTENCES;    
    const static std::string SENTENCE_ENDINGS;
    void
    tokenize_inner(std::istream& stream)
    {
      current_token.clear();
      is_current_token_special = false;
      auto next = std::char_traits<char>::eof();
      for (; ;)
      {
	next = stream.get();
	/*
	  We include apostophe in a word so words like "that's" are
	  words. This behavior is somewhat 'up for debate', in that
	  you could argue that we should treat "that's" as "that" and
	  throw away the s.
	 */
	if (std::isalnum(next) || next == '\'')
	{
	  insert_sentence_ender_if_new_sentence_detected(next);
	  current_token.push_back(next);
	}
	else if (std::isblank(next)
		 || (next == std::char_traits<char>::eof()))
	{
	  process_blank(stream);
	}
	else if (SENTENCE_ENDINGS.find(next) != std::string::npos)
	{
	  handle_sentence_end(stream, next);
	}
	else
	{
	  /*
	    In this case we ignore the character (for example :) but
	    end the current token. This allows something like "this:"
	    to be tokenized as just 'this'.
	   */
	  process_blank(stream);
	  last_token_may_have_ended_sentence = false;	  
	}
	if (!current_token.empty())
	  last_token_may_have_ended_sentence = false;
	if (next == std::char_traits<char>::eof() && stream.eof())
	  break;
      }

    }

    /*
      Handling a period (sentence end) is complicated by the fact that
      some 'words' like i.e., e.g., M.S., and so on, contain periods
      that do not end a sentence. So we have to check for this
      contingency against a database of 'special words'.
     */
    void handle_sentence_end(std::istream& stream, int endchar)
    {
      if (current_token.size() == 0)
      {
	tokens.push_back(SENTENCE_END);
	return;
      }
	  
      for (auto& i: get_special_words())
      {
	std::string temp_token = current_token;
	temp_token.push_back(endchar);

	if(i.substr(0,
		    temp_token.size()) == temp_token)
	{
	  // we have a 'special' word. For example, we the current token
	  // might be i, we got '.', and we see that there is a word
	  // starting with 'i.' (i.e.), so we set the special flag and continue.	  
	  current_token.push_back(endchar);
	  is_current_token_special = true;
	  break;
	}
      }

      if (!is_current_token_special)
      {
	tokens.push_back(current_token);
	tokens.push_back(SENTENCE_END);
	current_token.clear();
      }
    }

    void process_blank(std::istream& stream)
    {
      /*
	A token is 'special' when at some point it matched a special
	word. However, we need to make sure it still does after the
	blank.

	So, if we find the complete current token in the special
	words, we push it onto the stream. Otherwise, we remove back
	to the first sentence ender and see if _that_ matches a token
	(this allows us to do longest-matching for special words), and
	repeat.
       */
      bool was_special = false;
      for (; is_current_token_special; )
      {
	if (current_token_found_in_special_words())
	{
	  was_special = true;
	  break;
	}
	else
	{
	  putback_ending_of_token_upto_sentence_end(stream);
	}
      }
      finalize_current_token_to_stream();
      if (was_special)
	check_for_possible_sentence_ending_abbrev();
    }

    void check_for_possible_sentence_ending_abbrev()
    {
      std::string last_tok = tokens.back();
      if (get_specials_that_cant_end_sentences().find(last_tok)
	  == get_specials_that_cant_end_sentences().end()
	  && last_tok.back() == '.')
	last_token_may_have_ended_sentence = true;
      else
	last_token_may_have_ended_sentence = false;
    }

    void insert_sentence_ender_if_new_sentence_detected(int curchar)
    {
      if (last_token_may_have_ended_sentence
	  && (curchar >= 'A' && curchar <= 'Z')
	  && current_token.empty())
      {
	tokens.push_back(Tokenizer::SENTENCE_END);
      }      
    }

    void putback_ending_of_token_upto_sentence_end(std::istream& stream)
    {
      std::string temp_token_holder;
      /*
	If we happened to hit EOF at this last character but need
	to putback, we have to clear stream state first.
      */
      reset_stream_flags_if_necessary(stream);
      while (!current_token.empty()
	     && current_token_does_not_end_with_sentence_ender())
      {
	temp_token_holder.insert(temp_token_holder.begin(), current_token.back());
	current_token.pop_back();
      }
      if (current_token.empty())
      {
	current_token = temp_token_holder;
	finalize_current_token_to_stream();
      }
      else
      {

	for (auto i = temp_token_holder.rbegin();
	     i != temp_token_holder.rend(); ++i)
	  stream.putback(*i);
	stream.putback(current_token.back());
	current_token.pop_back();
      }      
    }

    void reset_stream_flags_if_necessary(std::istream& stream)
    {
      if (!stream)
	stream.clear();      
    }

    bool current_token_does_not_end_with_sentence_ender()
    {
      return SENTENCE_ENDINGS.find(current_token.back()) == std::string::npos;
    }

    void finalize_current_token_to_stream()
    {
      is_current_token_special = false;
      if (current_token.size())
	tokens.push_back(current_token);
      current_token.clear();      
    }

    bool current_token_found_in_special_words()
    {
      return (std::find(get_special_words().begin(),
			get_special_words().end(),
			current_token)
	      != get_special_words().end());
    }

    
    bool is_current_token_special;
    bool last_token_may_have_ended_sentence;
    std::string current_token;
    std::list<std::string> tokens;
  };
}
#endif
