
#ifndef _IMAGELOADEREXCEPTION_HPP_
#define _IMAGELOADEREXCEPTION_HPP_

#include <cstdarg>

class ImageLoaderException : std::exception {

  private:

  std::string file;
  std::string message;

  public:

  ImageLoaderException(std::string file, const char *format, ...) {
    this->file = file;
    
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    
    this->message  = "Could not load file '";
    this->message += file;
    this->message += "': ";
    this->message += buffer;
  }
  
  virtual ~ImageLoaderException() throw() {  
  }

  const char* what() const throw() {
    return message.c_str();
  }

};

#endif
