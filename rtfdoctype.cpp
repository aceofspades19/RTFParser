#include "rtfdoctype.hpp"
#include <iostream>
#include "Parser.hpp"
using namespace docTypes;
using namespace std;
using namespace rtfSymbol;
using namespace Parser;

/**
@param p the symbol that you want the doctype to process
@return the processed symbol, if it doesn't accept the symbol then it will
return nothing
 */
string PlainText::process(const rtfSymbol::Symbol p){
  if(p.symbol == PT){
    return p.word;
   }
  else {
    return "";
  }
}
