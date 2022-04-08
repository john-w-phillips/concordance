#include "WordCompiler.hpp"
#include <getopt.h>
#include <ostream>
#include <fstream>

class Options
{
public:
  Options():
    input_filename{"-"}
  {}

  std::string get_input_filename()
  {
    return input_filename;
  }

  enum OptionCodes
  {
    FILENAME = 0,
    HELP = 1,
  };
  void parse(int argc, char **argv)
  {
    int longindex = 0;
    struct option longopts[] = {
      {"filename", true, nullptr, FILENAME},
      {"help", false, nullptr, HELP},      
    };
    optind = 1;
    int rval = -1;
    for (; (rval = getopt_long(argc, argv, "f:h", longopts, NULL)) != -1; )
    {
      switch (rval)
      {
      case FILENAME:
	input_filename = optarg;
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
    std::cout << "Usage: " << std::endl
	      << argv[0] << " --filename FILENAME" << std::endl
	      << "\t" << "Produce a concordance of FILENAME on standard output. If not provided, use standard input."
	      << std::endl << std::endl
	      << argv[0] << " --help" << std::endl
	      << "\t" << "Produce this help." << std::endl;
  }
  std::string input_filename;
};
  
int
main(int argc, char **argv)
{
  Options options;
  code_challenge::WordCompiler concordance;
  options.parse(argc, argv);
  std::istream *input_stream = nullptr;
  std::unique_ptr<std::istream> stream_mem{};
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
