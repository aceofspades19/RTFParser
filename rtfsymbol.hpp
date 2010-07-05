#include <string>

#ifndef RTF_SYMBOL_H
#define RTF_SYMBOL_H
namespace rtfSymbol {
 /**
     @class Symbol
     @brief This is class basically contains: a. how the word in a line of source is important to us
     by have a character to what symbol it is, b. the line number and c. the actual word itself.
     This class is soley designed to be used with/for a parser
   */
  struct Symbol {
    int symbol;
    std::string word;
    int line;
    Symbol(int nsymbol, std::string nword, int nline){
      symbol = nsymbol;
      word = nword;
      line = nline;
    }
    Symbol(int nsymbol, const char* nword, int nline){
      symbol = nsymbol;
      word = *nword;
      line = nline;
    }
    Symbol(int nsymbol, char nword, int nline){
      symbol = nsymbol;
      word = nword;
      line = nline;
    }
    Symbol(){}
  };
}
#endif
