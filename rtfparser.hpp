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
#include "Parser.hpp"
#include "boost/smart_ptr/shared_ptr.hpp"
//#include <exception>
#define RTFPARSER_TEST
#ifndef RTFPARSER_HEADER_FILE
#define RTFPARSER_HEADER_FILE

namespace Parser{
  namespace rtfParser {
    typedef std::vector<std::string>::const_iterator strit; 
    typedef std::vector<std::string> strvec;
  
    /**
       @class RTFParser
       @brief this class is the actual RTFParser, it inherit from Parser and contains a keyword class for
       all the keywords in the rtf format
    */
    class RTFParser: AbstractParser {
    private:
      bool testChar(char c);
      std::string trimEnd(std::string s);
      rtfSymbol::Symbol* commandHandler(std::string& command);
      boost::tuples::tuple<std::string, std::string, std::string> getParamAndCom(std::string& command);
    
    public:
      std::vector<rtfSymbol::Symbol> getDoc();
      void parse();
      RTFParser(std::string& nfile){
	file.open(nfile.c_str());
	docTypes::DocType *t = new docTypes::PlainText();
	doc = new Parser::Document(t);

      }
      RTFParser(std::string& nfile, docTypes::DocType* d){
	file.open(nfile.c_str());
	doc = new Parser::Document(d);
      }
      RTFParser(const char* nfile){
	file.open(nfile);
	docTypes::DocType *t;
	doc = new Parser::Document(t);
      }
      std::vector<std::string> makeDoc();
      ~RTFParser(){}
    };
    class WriteDocument:Document {
    public:
      WriteDocument(std::vector<rtfSymbol::Symbol> document, docTypes::DocType* dt):Document(dt, document){
      }
   
      void write(std::string filename);
      void replace(const std::vector<std::string>& lines);
      WriteDocument(const WriteDocument& wd):Document(wd.type){
	doc = wd.doc;
      }
      WriteDocument(){}
    };

    class RTFWriter {
      std::string filename;
      WriteDocument wd;
    public:
      RTFWriter();
      RTFWriter(std::string file, std::vector<rtfSymbol::Symbol> doc): wd(doc, (new docTypes::RTFText())){
	filename = file;
      }
      RTFWriter(std::string file):filename(file){}
      void write();
      void replace(const std::vector<std::string>& lines);
    };
  
  }
}

#endif
