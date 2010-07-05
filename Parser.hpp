#include <vector>
#include <string>
#include <fstream>
#include "rtfdoctype.hpp"
#include "rtfsymbol.hpp"
#include "boost/tuple/tuple.hpp"


#ifndef PARSER_HEADER_FILE
#define PARSER_HEADER_FILE

namespace Parser {
  /**
     PT is plain text
     IT is italic text
     BT is bold text
     UT is underlined text
     C is command
     N is number
     CS is control symbol, eg {
   */
  enum codes { PT=0, IT, BT, UT, C, N, CS } pcodes;
  
  /**
     @class Document
     @brief This class controls the data for an rtf file, it stores the doctype and it stores the data in a vector
    */
  class Document {
  protected:
    std::vector<rtfSymbol::Symbol> doc;
    docTypes::DocType* type;
    
  public:
    Document( docTypes::DocType* nType){
      type = nType;
    }
    Document( docTypes::DocType* nType, std::vector<rtfSymbol::Symbol> document){
      type = nType;
      doc = document;
    }
    /**
       default constructor
    */
    Document(){}
    void add(rtfSymbol::Symbol& toAdd);
    std::vector<std::string> makeDoc();
    std::vector<rtfSymbol::Symbol> getDoc();
  };
  
  
  
  class AbstractParser {
  protected:
    std::fstream file;
    Document *doc;
  public:
    virtual void parse() =0;
    virtual std::vector<rtfSymbol::Symbol> getDoc() =0;
    virtual std::vector<std::string> makeDoc() =0;
    virtual ~AbstractParser(){}
  };

}


#endif
