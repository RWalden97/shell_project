#ifndef simplcommand_h
#define simplecommand_h

struct SimpleCommand {
  // Available space for arguments currently preallocated
  int _numberOfAvailableArguments;

  // Number of arguments
  int _numberOfArguments;
  char ** _arguments;

  SimpleCommand();
  void insertArgument( char * argument );

};

#endif
