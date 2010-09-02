/*****************************************************************************************************************
 *Copyright (c) 2009, Jordan Evans
 *All rights reserved.
 *
 *Redistribution and use in source and binary forms, with or without modification, are permitted provided that 
 *the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice, this list of conditions and the 
 *following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the 
 *following disclaimer in the documentation and/or other materials provided with the distribution.
 *   * Neither the name of the xclerk.net nor the names of its contributors may be used to endorse or promote 
 *products derived from this software without specific prior written permission.
 *
 *THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 *INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 *DISCLAIMED. IN *NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, \
 *SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 *SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 *WHETHER IN 
 *CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
 *THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ********************************************************************************************************************/
#include "rtfparser.hpp"
#include <iostream>
#include <cctype>
#include "rtfexceptions.hpp"
//#include <stdexcept>
#undef RTFPARSER_TEST
#ifdef RTFPARSER_TEST
#include "/boost/boost/test/included/unit_test.hpp"
#include <fstream>
#include "boost/smart_ptr/shared_ptr.hpp"

using namespace boost::unit_test;
#endif
using namespace Parser;
using namespace Parser::rtfParser;
using namespace std;
using namespace boost::tuples;
using namespace rtfSymbol;


vector<Symbol> Document::getDoc(){
  return doc;
}

vector<Symbol> RTFParser::getDoc(){
  return doc->getDoc();
}
/**
adds a symbol to the queue in the Document class
 */
void Document::add(Symbol& toAdd){
  if((!doc.empty())){
      if((doc.back().word == "\\") && (toAdd.word == "\\")){
	return;
      }
  }
  if(toAdd.word == ""){
    return;
  }
  doc.push_back(toAdd);
}

/**
   @return a vector<string> with each line on a string, the contents depends on
   the DocType you use
 */
vector<string> Document::makeDoc(){
  vector<Symbol>::const_iterator it;
  vector<string> finalDoc;
  for(it = doc.begin();it!=doc.end();++it){
    if(!(it->word.empty())){
      finalDoc.push_back(type->process(*it));
    }
  }
  return finalDoc;
}

bool RTFParser::testChar(char c){
  if((c == '}') || (c == '\\') || (c == '{')){
    return true;
  }
  return false;
}

string RTFParser::trimEnd(string s){
  for(unsigned int i=0;i<s.length();++i){
    if(testChar(s.at(i))){
      return s.substr(0, i);
    }
  }
  return s;
}
/**
@param the command which you want to process, must contain the command and the parameter(s)!
@return a tuple with the command as the first one in the tuple and the parameter as the second memeber, and/or 2nd parameter as the 3rd memeber of the tuple
 */
tuple<string, string, string> RTFParser::getParamAndCom(string& command){
  string com;
  string param[] = {"", ""};
  bool state=false;
  string ncommand;
  if(command.empty()){
    throw rtfExceptions::EmptyString();
  }
  else if(testChar(command.at(0))){
    Symbol s('b', command.at(0), 0);
    doc->add(s);
    ncommand = command.substr(1, command.length());
  }
  else {
    ncommand = command;
  }
  ncommand = trimEnd(ncommand);
  const char*  s = ncommand.c_str();
  int i=0;
  //have to do it old school way else it doesn't work
  //here be dragons!
  while(*s != '\0'){
    if(isdigit(*s)){
      if(com.empty()){
	com = ncommand.substr(0, i);
      }
      param[0] += ncommand.at(i);
      state = 1;
    }
    else if(*s == ' '){
      state = 1;
      if(com.empty()){
	com = ncommand.substr(0, i);
      }
      ++s;
      while(*s != '\0'){
	param[1] += *s;
	++s;
      }
      goto end;
    }
    ++s;
    ++i;
  }
  //for commands that have no parameter
  if(!state){
    com = ncommand;
  }
 end:
  return make_tuple(com, param[0], param[1]);
 }

 /**
 handles the parsing for the rtf parser
  */
 void RTFParser::parse(){
   string line;
   bool brace=false;\
   Symbol* s;
   string c;
   int counter;
   while(getline(file, line, '\n')){
     for(unsigned int i=0;i<line.length();++i){
       switch(line.at(i)){
       case '{':
	 brace = true;
	 s = new Symbol('b', line.at(i), 0);
	 doc->add(*s);
	 break;
       case '}':
	 s = new Symbol('b', line.at(i), 0);
	 doc->add(*s);
	 brace = false;
	 break;
       case '\\':
	 c = line.substr(i, line.length());
	 s = commandHandler(c);
	 counter = atoi(s->word.c_str());
	 ++s;
	 for(int i=0;i<counter;++i){
	   doc->add(*s);
	   ++s;
	 }
	 break;
       default:
	 break;
       }

     }
   }
 }

 /**
 @param command the full command you want to use, must contain the command and the param!
 @return an array of symbols
  */
 Symbol* RTFParser::commandHandler(string& command){
   Symbol *s;
   tuple<string, string, string> t;
   try {
     t = getParamAndCom(command);
   }
   catch(rtfExceptions::EmptyString e){
     s = new Symbol[2];
     s[0].word = "0";
     return s;
   }
   
     Symbol temp('c', t.get<0>(), 0);
     Symbol temp1('n', t.get<1>(), 0);
     Symbol temp2;
     string space = " ";
     if(!t.get<2>().empty()){
       Symbol temp3('u', (space+=t.get<2>()), 0);
       s = new Symbol[4];
       s[3] = temp3;
       temp2.word = "3";
     }
     else {
       s = new Symbol[3];
       temp2.word = "2";
     }
     if(temp2.word == "3"){
       if(t.get<0>() == "insrsid"){
	 s[3].symbol = PT;
       }
       if((t.get<0>() == "b") || (t.get<0>() == "i") || (t.get<0>() == "ulnone")){
	 s[3].symbol = PT;
       }
       if(t.get<0>() == "generator"){
	 s[3].word = " xclerk.net generator";
       }
     }
     s[0] = temp2;
     s[1] = temp;
     s[2] = temp1;
     return s;
 }

/**
@return the document, with each line as a seperate string
 */
 vector<string> RTFParser::makeDoc(){
   return doc->makeDoc();
 }


void WriteDocument::write(string filename){
  /**ofstream file(filename.c_str());
  string word;
  int size;
  if(file.is_open()){
    for(vector<Symbol>::iterator it=doc.begin();it!=doc.end();++it){
      word = it->word;
      size = word.size();
      if(it->word.size() != 0){
	if(it->word.at((it->word.size()-1)) == '\r'){
	  it->word += '\n';
	}
      }
      file << it->word;
    }
    }*/
  type->write(filename);
}

void RTFWriter::write(){
  wd.write(filename);
}

void WriteDocument::replace(const vector<string>& lines){
  string space = " ";
  //bool finished = false;
  ofstream log("log");
  vector<Symbol>::iterator it=doc.begin();
  vector<string>::const_iterator sit=lines.begin();
  while(1){
    if((sit!=lines.end())){
      if((it->symbol == PT)){
	for(unsigned int i=0;i<it->word.size();++i){
	  if(it->word.at(i) == '$'){
	    log << it->word;
	    it->word = space + *sit + space;
	    ++sit;
	    break;
	  }
	}
      }
    }
    else {
      break;
    }
    ++it;
    }
}

void RTFWriter::replace(const vector<string>& lines){
  wd.replace(lines);
}

/*
Example of how to use the parser to write a file
int main(){
  string f = "borat";
  string fo = "test1.rtf";
  string f3 = "pretest1.rtf";
  vector<string> r;
  r.push_back(" It is three in the morning and I am in the nest of something");
  r.push_back(" They have cleverly shifted their shapes; one of them has taken the form of a little old women");
  boost::shared_ptr<RTFParser> p(new RTFParser(f));
  p->parse();
  vector<string> x = p->makeDoc();
  boost::shared_ptr<RTFWriter> w(new RTFWriter(fo, p->getDoc()));
   w->replace(r);
   w->write();
  return 0;
  }

*/
