
#ifndef _IMAGELOADEREXCEPTION_HPP_
#define _IMAGELOADEREXCEPTION_HPP_

class ImageLoaderException : std::exception {

  private:

  std::string file;
  std::string message;

  void setFile(const char *file) {
    this->file = file;
    this->message  = "Could not load file '";
    this->message += file;
    this->message += "'";
  }

  public:

  ImageLoaderException(std::string file) {
    setFile(file.c_str());
  }
  
  ImageLoaderException(const char *file) {
    setFile(file);
  }
  
  virtual ~ImageLoaderException() throw() {  
  }

  const char* what() const throw() {
    return message.c_str();
  }

};

#endif
