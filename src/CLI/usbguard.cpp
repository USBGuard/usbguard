#include "Typedefs.hpp"
#include "Common/Utility.hpp"
#include "CLICommand.hpp"
#include "CLIArgv_Lexer.hpp"
#include "CLILine_Lexer.hpp"
#include "CLIParser.h"

#include "RuleAppend.hpp"
#include "DeviceSetPolicy.hpp"

#include <quex/code_base/multi.i>
#include <cstdlib>

namespace usbguard
{
#include "CLIParser.c"

  static void CLIParserDeleter(void *p)
  {
    CLIParserFree(p, &free);    
  }

  PointerVector<CLICommand> parseCLICommandsFromArgv(const int argc, char *argv[])
  {
    if (argc < 1 || argv == nullptr) {
      throw std::runtime_error("BUG: parseCLICommandsFromArgv: invalid arguments");
    }

    PointerVector<usbguard::CLICommand> commands;

    /*
     * If only argv[0] is set, there's nothing to parse.
     */
    if (argc == 1) {
      return std::move(commands);
    }

    CLIArgv::Lexer lexer((QUEX_TYPE_CHARACTER*)nullptr, 0);
    UniquePointer<void,void(*)(void *)> parser_data(CLIParserAlloc(&malloc),
						    CLIParserDeleter);

    void *arg_begin = nullptr;
    void *arg_end = nullptr;
    size_t arg_index = 0;

    /* NOTE: argv[0] is skipped by design */

    for (;;) {
      if (arg_begin == arg_end) {
	if (++arg_index >= (size_t)argc) {
	  /*
	   * Break out of the loop if there's no arg to process
	   */
	  break;
	}
	else {
	  /*
	   * Initialize argument value pointers for processing
	   */
	  arg_begin = argv[arg_index];
	  arg_end = (char *)arg_begin + strlen(argv[arg_index]);
	}
      }

      /* Copy arg value to the lexer */
      arg_begin = lexer.buffer_fill_region_append(arg_begin, arg_end);

      /* Lexer + Parse loop */
      for (;;) {
	CLIArgv::Token *token_ptr = nullptr;
	lexer.receive(&token_ptr);

	if (token_ptr == nullptr) {
	  throw std::runtime_error("BUG: token_ptr == nullptr after lexer.receive()");
	}

	if (token_ptr->type_id() != CLI_TOKEN_TERMINATION) {
	  CLIParser(parser_data.get(),
		    token_ptr->type_id(),
		    new String(CLIArgv::unicode_to_char(token_ptr->get_text())),
		    &commands);
	}
	else {
	  break;
	}
      }
    }

    /* Finalize parser state */
    CLIParser(parser_data.get(), 0, nullptr, &commands);

    return std::move(commands);
  }
} /* namespace usbguard */

int main(int argc, char *argv[])
{
  /* Parse */
  usbguard::PointerVector<usbguard::CLICommand> commands = \
    usbguard::parseCLICommandsFromArgv(argc, argv);

  usbguard::CLI cli;
  auto commands = usbguard::CLI::parseCommandsFromArgv(argc, argv);

  if (!cli->isConnected()) {
    // cannot connect to the daemon
    return EXIT_FAILURE;
  }

  for (auto command : commands) {
    command->prepare();
  }

  for (auto command : commands) {
    cli->execute(command);
  }

  return EXIT_SUCCESS;
}
