#include <exception>

namespace rtfExceptions {
  class IncompatibleVersion: public std::exception {
    virtual const char* what() const throw()
    {
      return "Incompatible rtf version -- must be rtf1";
    }
  };
  class IncompatibleEncoding: public std::exception {
    virtual const char* what() const throw()
    {
      return "Incompatible enconding -- must be ansi";
    }
  };
  class EmptyString: public std::exception{
    virtual const char* what() const throw()
    {
      return "String is empty";
    }
  };
}
