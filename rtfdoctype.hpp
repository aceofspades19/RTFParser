/*
Contains default doctypes
 */
#include <string> 
#include <vector>
#include "rtfsymbol.hpp"
#ifndef RTF_DOCTYPES_HEADER
#define RTF_DOCTYPES_HEADER
namespace docTypes{
  /**
     @class DocType
     @brief This is the class all doctypes have to inherit from
     must implment process.
   */
  class DocType {
  protected:
  public:
    virtual std::string process(const rtfSymbol::Symbol p) =0;
    virtual void write(std::string filename) =0;
  };
  
  /**
     @class PlainText
     @brief This class represents a plain text document
     process returns just the human readable text of the document
   */
  class PlainText: public DocType {
  public:
    virtual std::string process(const rtfSymbol::Symbol p);
    virtual void write(std::string filename);
  };

  class RTFText: public DocType {
  public:
    virtual std::string process(const rtfSymbol::Symbol  p);
    virtual void write(std::string filename);

  };
}

#endif
