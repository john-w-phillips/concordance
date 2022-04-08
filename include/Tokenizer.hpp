#ifndef _TOKENIZER_HPP_
#define _TOKENIZER_HPP_
#include <list>
#include <string>
#include <iostream>
#include <locale>
#include <algorithm>
namespace code_challenge
{
  class Tokenizer
  {
  public:
    const static std::string SENTENCE_END;

    const std::list<std::string>&
    scan(std::istream& stream)
    {
      tokens.clear();
      current_token.clear();
      is_current_token_special = false;
      tokenize_inner(stream);
      return tokens;
    }



  protected:
    const static std::list<std::string> SPECIAL_CHARS;

    void
    tokenize_inner(std::istream& stream)
    {
      current_token.clear();
      is_current_token_special = false;
      auto next = std::char_traits<char>::eof();
      for (; ;)
      {
	next = stream.get();
	if (std::isalnum(next))
	{
	  current_token.push_back(next);
	}
	else if (std::isblank(next)
		 || (next == std::char_traits<char>::eof()))
	{
	  maybe_push_back_token(stream);
	}
	else if (next == '.')
	{
	  handle_sentence_end(stream);
	}
	else
	{
	  /*
	    In this case we ignore the character (for example :) but
	    end the current token. This allows something like "this:"
	    to be tokenized as just 'this'.
	   */
	  maybe_push_back_token(stream);
	}
	if (next == std::char_traits<char>::eof())
	  break;
      }

    }
    void handle_sentence_end(std::istream& stream)
    {
      if (current_token.size() == 0)
      {
	tokens.push_back(SENTENCE_END);
	return;
      }
	  
      for (auto& i: SPECIAL_CHARS)
      {
	std::string temp_token = current_token;
	temp_token.push_back('.');
	// we have a 'special' word. For example, we the current token
	// might be i, we got '.', and we see that there is a word
	// starting with 'i.' (i.e.), so we set the special flag and continue.
	if(i.substr(0,
		    temp_token.size()) == temp_token)
	{
	  current_token.push_back('.');
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

    void maybe_push_back_token(std::istream& stream)
    {
      if (is_current_token_special)
      {
	auto it = std::find(SPECIAL_CHARS.begin(),
			    SPECIAL_CHARS.end(),
			    current_token);
	if (it == tokens.end())
	{
	  // no match.
	  while (current_token.back() != '.')
	  {
	    stream.putback(current_token.back());
	    current_token.pop_back();
	  }
	  current_token.pop_back();
	  tokens.push_back(current_token);
	  tokens.push_back(SENTENCE_END);
	  current_token.clear();
	  is_current_token_special = false;
	  return;	  
	}
      }
      is_current_token_special = false;
      if (current_token.size())
	tokens.push_back(current_token);
      current_token.clear();
    }

    bool is_current_token_special;
    std::string current_token;
    std::list<std::string> tokens;
  };
}
#endif
