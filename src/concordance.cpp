#include "WordCompiler.hpp"
#include <getopt.h>
#include <ostream>
#include <fstream>

class Options
{
public:
  Options():
    input_filename{"-"},
    specialwords_filename{}
  {}

  std::string get_input_filename()
  {
    return input_filename;
  }

  std::string get_specialwords_filename()
  {
    return specialwords_filename;
  }

  enum OptionCodes
  {
    FILENAME = 0,
    HELP = 1,
    SPECIALWORDS_FNAME = 2,
  };
  void parse(int argc, char **argv)
  {
    int longindex = 0;
    struct option longopts[] = {
      {"filename", true, nullptr, FILENAME},
      {"help", false, nullptr, HELP},
      {"special-words", true, nullptr, SPECIALWORDS_FNAME},            
    };
    optind = 1;
    int rval = -1;
    for (; (rval = getopt_long(argc, argv, "f:s:h", longopts, NULL)) != -1; )
    {
      switch (rval)
      {
      case FILENAME:
	input_filename = optarg;
	break;
      case SPECIALWORDS_FNAME:
	specialwords_filename = optarg;
	break;
      case HELP:
	usage(argc, argv);
	exit(0);
	break;
      default:
	usage(argc, argv);
	exit(1);
      }
    }
    if (optind != argc && argc != 1)
    {
      usage(argc, argv);
      exit(0);
    }
  }

protected:
  void usage(int argc, char **argv)
  {
    std::string big_indent = "    ";
    std::string bigbig_indent = "      ";    
    std::string small_indent = "  ";
    std::cout << "Usage: " << std::endl
	      << argv[0] << " [OPTIONS...] {-f|--filename} FILENAME" << std::endl
	      << big_indent << "Produce a concordance of FILENAME on standard output. If not provided, use standard input." << std::endl
	      << small_indent << "Options: " << std::endl
	      << big_indent << "{-s|--special-words} SPECIALWORDS_FILENAME" << std::endl
	      << bigbig_indent << "read SPECIALWORDS_FILENAME for a list of 'special words'." << std::endl
	      << bigbig_indent << "These are words that have periods inside of them that should not" << std::endl
	      << bigbig_indent << "be considered sentence-ending periods, for example e.g." << std::endl
	      << bigbig_indent << "The format is one 'special' word per line." << std::endl
	      << std::endl << std::endl
	      << argv[0] << " {-h|--help}" << std::endl
	      << "\t" << "Produce this help." << std::endl;
  }
  std::string input_filename;
  std::string specialwords_filename;
};
  
int
main(int argc, char **argv)
{
  Options options;
  code_challenge::WordCompiler concordance;
  options.parse(argc, argv);
  std::istream *input_stream = nullptr;
  std::unique_ptr<std::istream> stream_mem{};

  if (options.get_specialwords_filename() != "")
  {

    std::ifstream words_file{options.get_specialwords_filename(),
      std::ios_base::in};
    concordance.set_specialwords(words_file);
  }

  if (options.get_input_filename() == "-")
  {
    input_stream = &std::cin;
  }
  else
  {
    stream_mem = std::unique_ptr<std::istream>(
      new std::ifstream(
	options.get_input_filename(),
	std::ios_base::in));
    input_stream = stream_mem.get();
  }

  concordance.compile_words_from_source(*input_stream);
  for (auto i = concordance.begin_words();
       i != concordance.end_words();
       ++i)
  {
    std::cout << i->get_report_string() << std::endl;
  }
}
