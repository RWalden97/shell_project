#ifndef command_h
#define command_h

#include "simpleCommand.hh"

// Command Data Structure
struct Command {
  int _numberOfAvailableSimpleCommands;
  int _numberOfSimpleCommands;
  SimpleCommand ** _simpleCommands;
  char * _outFile;
  char * _inputFile;
  char * _errFile;
  int _background;
  int _append;
  int _inCounter;
  int _outCounter;
  int _sub;

  void prompt();
  void print();
  void execute();
  void clear();
  int builtInCheck(int i);

  Command();
  void insertSimpleCommand( SimpleCommand * simpleCommand );
  int unsetEnv(char * t);

  static Command _currentCommand;
  static SimpleCommand *_currentSimpleCommand;
};
#endif
