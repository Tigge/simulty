#ifndef _SIMULTYEXCEPTION_HPP_
#define _SIMULTYEXCEPTION_HPP_

#define SIMULTYEXCEPTION(string) SimultyException(string, __FILE__, __LINE__)

#include <exception>
#include <string>
#include <sstream>
#include <cstdlib>

#ifdef unix
#ifdef __GNUC__
#include <execinfo.h>
#endif
#endif

class SimultyException: public std::exception {

  private:
  
  #ifdef unix
  #ifdef __GNUC__
  void *backtraceArray[30];
  size_t backtraceSize;
  char **backtraceStrings;
  #endif
  #endif
  
  std::string output;

  std::string message;
  std::string file;
  int         line;

  public:

  SimultyException(std::string message, std::string file, int line) throw() {
  
    this->message = message;
    this->file    = file;
    this->line    = line;
    
    std::stringstream ss; ss << message << " - " << file << ":" << line;

    #ifdef unix
    #ifdef __GNUC__
    backtraceSize    = backtrace(backtraceArray, 30);
    backtraceStrings = backtrace_symbols(backtraceArray, backtraceSize);    
    ss << std::endl << std::endl;    
    for (unsigned int i = 0; i < backtraceSize; i++)
      ss << backtraceStrings[i] << std::endl;
    #endif
    #endif

    this->output  = ss.str();
  }
  virtual ~SimultyException() throw() {
    #ifdef unix
    #ifdef __GNUC__
    free(backtraceStrings);
    #endif
    #endif
  }
  
  virtual std::string getOuptut() throw() {
    return output;
  }
  
  virtual std::string getMessage() throw() {
    return message;
  }
  
  virtual std::string getFile() throw() {
    return file;
  }
  
  virtual int getLine() throw() {
    return line;
  }

  virtual const char* what() const throw() {
    return (const char *)output.c_str();
  }
};


#endif
