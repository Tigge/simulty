#ifndef _SIMULTYEXCEPTION_HPP_
#define _SIMULTYEXCEPTION_HPP_

#define SIMULTYEXCEPTION(string) SimultyException(string, __FILE__, __LINE__)

#include <exception>
#include <string>
#include <sstream>

class SimultyException: public std::exception {

  private:
  
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
    this->output  = ss.str();
  }
  virtual ~SimultyException() throw() {
  
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
