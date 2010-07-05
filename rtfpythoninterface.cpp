#include "Python.h"
#include "rtfparser.hpp"
#include "rtfdoctype.hpp"
#include <vector>
#include <iostream>
#include <fstream>
using Parser::rtfParser::RTFParser;
using Parser::rtfParser::RTFWriter;
#ifdef WIN32
__declspec(dllexport)
#endif
static PyObject *RTFError;
static PyObject *processf = NULL;

/**
Writing stuff
*/
static PyObject* wrap_replace(PyObject* self, PyObject* args){
  std::ofstream file("log");
  PyObject* list;
  void *temp;
  int len;
  PyObject* item;
  std::string* stemp;
  PyObject *writer;
  std::vector<std::string> sy;
  if(!PyArg_ParseTuple(args, "iOO", &len, &list, &writer)){
      return NULL;
  }
  if (!PySequence_Check(list)) {
    PyErr_SetString(PyExc_TypeError, "expected sequence");
    return NULL;
  }
  for(int i=0;i<len;++i){
    item = PySequence_GetItem(list, i);
    if(!PyString_Check(item)){
      PyErr_SetString(PyExc_TypeError, "expected string");
      return NULL;
    }
    const char * ctemp = (const char*)PyString_AsString(item);
    stemp = new std::string(ctemp);
    sy.push_back(*stemp);
  }
  temp = PyCObject_AsVoidPtr(writer);
  RTFWriter * thiswriter = static_cast<RTFWriter *>(temp);
  thiswriter->replace(sy);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject* wrap_write(PyObject* self, PyObject* args){
  PyObject *writer;
  if(!PyArg_ParseTuple(args, "O", &writer)){
      return NULL;
    }
   void * temp = PyCObject_AsVoidPtr(writer);
    RTFWriter * thiswriter = static_cast<RTFWriter *>(temp);
    thiswriter->write();
    Py_INCREF(Py_None);
    return Py_None;
}

static void PyDelPyWriter(void *ptr){
  RTFWriter* oldp = static_cast<RTFWriter *>(ptr);
  delete oldp;
  return;
}

static PyObject* wrap_pywriter(PyObject* self, PyObject* args){
  const char* filename;
  int len;
  PyObject *list;
  PyObject *item;
  void* temp;
  rtfSymbol::Symbol* stemp;
  std::vector<rtfSymbol::Symbol> sy;
  //len required because we are dealing c stuff :(
  if(!PyArg_ParseTuple(args, "siO", &filename, &len, &list)){
    return NULL;
  }
  if (!PySequence_Check(list)) {
    PyErr_SetString(PyExc_TypeError, "expected sequence");
    return NULL;
  }
  for(int i=0;i<len;++i){
    item = PySequence_GetItem(list, i);
    temp = PyCObject_AsVoidPtr(item);
    stemp = static_cast<rtfSymbol::Symbol *>(temp);
    sy.push_back(*stemp);
  }
  RTFWriter* rw = new RTFWriter(filename, sy);
  return PyCObject_FromVoidPtr(rw, PyDelPyWriter);
  
}

/*Symbol python interface stuff*/
static PyObject* wrap_getline(PyObject *self, PyObject* args){
  PyObject* symbol = 0;
  if(!PyArg_ParseTuple(args, "O", &symbol)){
    return NULL;
  }
  void *temp = PyCObject_AsVoidPtr(symbol);
  rtfSymbol::Symbol* s = static_cast<rtfSymbol::Symbol *>(temp);
  return Py_BuildValue("i", s->line);
}

static PyObject* wrap_getword(PyObject *self, PyObject* args){
  PyObject* symbol = 0;
  if(!PyArg_ParseTuple(args, "O", &symbol)){
    return NULL;
  }
  void *temp = PyCObject_AsVoidPtr(symbol);
  rtfSymbol::Symbol* s = static_cast<rtfSymbol::Symbol *>(temp);
  return Py_BuildValue("s", s->word.c_str());
}
/**
This function gets the symbol member of Symbol
 */
static PyObject* wrap_getsymbol(PyObject *self, PyObject* args){
  PyObject* symbol = 0;
  if(!PyArg_ParseTuple(args, "O", &symbol)){
    return NULL;
  }
  void *temp = PyCObject_AsVoidPtr(symbol);
  rtfSymbol::Symbol* s = static_cast<rtfSymbol::Symbol *>(temp);
  return Py_BuildValue("s", s->symbol.c_str());

}

static void PyDelSymbol(void *ptr){
  rtfSymbol::Symbol *oldp = static_cast<rtfSymbol::Symbol *>(ptr);
  delete oldp;
  return;
}

static PyObject* wrap_Symbol(PyObject* self, PyObject* args){
  const char* symbol;
  const char* word;
  if(!PyArg_ParseTuple(args, "ss", &symbol, &word)){
    return NULL;
  }
  //wierd bug that doesn't let me pass an int, and line number is never used anyway so it doesn't matter
  rtfSymbol::Symbol *s = new rtfSymbol::Symbol(*symbol, word, 0);
  return PyCObject_FromVoidPtr(s, PyDelSymbol);
} 
/**
A python interface to inherit from the doctype class
it has a dummy process so you can "inherit from it"
 */
class PyDoctype : public docTypes::DocType{
public:
  virtual std::string process(const rtfSymbol::Symbol p){
    PyObject *result=0;
    result = PyEval_CallObject(processf, Py_BuildValue("(O)", wrap_Symbol(result, Py_BuildValue("(ss)", p.symbol.c_str(), p.word.c_str()))));
    if(result == NULL){
      return NULL;
    }
    Py_DECREF(result);
  }
};

static PyObject * pydoctype_callback(PyObject *dummy, PyObject *args)
{
    PyObject *result = NULL;
    PyObject *temp;

    if (PyArg_ParseTuple(args, "O:set_callback", &temp)) {
        if (!PyCallable_Check(temp)) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable");
            return NULL;
        }
        Py_XINCREF(temp);       
        Py_XDECREF(processf);  
        processf = temp;       
        Py_INCREF(Py_None);
        result = Py_None;
    }
    return result;
}


static void PyDelPyDoctype(void *ptr){
  PyDoctype* oldp = static_cast<PyDoctype *>(ptr);
  delete oldp;
  return;
}
static PyObject* wrap_pydoctype(PyObject* self, PyObject* args){
  PyDoctype *dt = new PyDoctype();
  return PyCObject_FromVoidPtr(dt, PyDelPyDoctype);
}
/*All plaintext doctype stuff*/
static void PyDelPTDoctype(void *ptr){
  docTypes::PlainText* oldp = static_cast<docTypes::PlainText *>(ptr);
  delete oldp;
  return;
}

static PyObject* wrap_plain_text_doctype(PyObject* self, PyObject* args){
  docTypes::PlainText* pt = new docTypes::PlainText();
  return PyCObject_FromVoidPtr( pt, PyDelPTDoctype);
}
/*All RtfParser stuff*/

static PyObject* wrap_get_doc(PyObject* self, PyObject* args){
  PyObject* parser = 0;
  rtfSymbol::Symbol* stemp=0;
  void* ptemp = 0;
  if(!PyArg_ParseTuple(args, "O", &parser)){
      return NULL;
    }
    void * temp = PyCObject_AsVoidPtr(parser);
    RTFParser *thisparser = static_cast<RTFParser *>(temp);
    PyObject* plist = PyList_New(0);
    std::vector<rtfSymbol::Symbol> sit = thisparser->getDoc();
    for(std::vector<rtfSymbol::Symbol>::const_iterator it=sit.begin();it!=sit.end();++it){
      stemp = new rtfSymbol::Symbol(*it);
      ptemp = static_cast<void *>(stemp);
      if(PyList_Append(plist, PyCObject_FromVoidPtr(ptemp, PyDelSymbol)) != 0){
	PyErr_Format(PyExc_TypeError, "Cannot get document");
	return NULL;
      }
    }
    
    Py_INCREF(plist); 
    return plist;
}

/**
   @param args accepts a rtfparser object as an argument
   @return a list with all the lines of the document
 */
static PyObject* wrap_make_doc(PyObject* self, PyObject* args){
  std::vector<std::string> doc;
  PyObject* parser = 0;
  if(!PyArg_ParseTuple( args, "O", &parser)){
      return NULL;
    }
  void * temp = PyCObject_AsVoidPtr(parser);
  RTFParser * thisparser = static_cast<RTFParser *>(temp);
  doc = thisparser->makeDoc();
  PyObject* plist = PyList_New(0);
  std::vector<std::string>::const_iterator it;
  for(it = doc.begin();it!=doc.end();++it){    
    if(PyList_Append( plist, PyString_FromString(it->c_str())) != 0){
      PyErr_Format(PyExc_TypeError, "Cannot construct document");
      return NULL;
    }
  }
  return plist;
}

/**
   @param args accepts an rtfparser
   @return nothing just runs the parse method on rtfparser
 */
static PyObject* wrap_Parse(PyObject* self, PyObject* args){
  PyObject* parser = 0;
  if(!PyArg_ParseTuple( args, "O", &parser)){
      return NULL;
    }
    void * temp = PyCObject_AsVoidPtr(parser);
    RTFParser * thisparser = static_cast<RTFParser *>(temp);
    thisparser->parse();
    Py_INCREF(Py_None);
    return Py_None;
}

static void PyDelParser(void *ptr)
{
  RTFParser* oldp = static_cast<RTFParser *>(ptr);
  delete oldp;
  return;
}

/**
   @param args a filename
   @return a rtfparser object
 */
static PyObject * wrap_RTFParser(PyObject * self, PyObject * args){
  const char *s = 0;
  if(!PyArg_ParseTuple( args, "s", &s)){
    return NULL;
  }	
  std::string ss(s);
  RTFParser *parser = new RTFParser(ss);
  return PyCObject_FromVoidPtr( parser, PyDelParser);
}

static PyMethodDef rtfparserMethods[] = 
{
  { "PyRtfParser", wrap_RTFParser, METH_VARARGS, 
    "PyRtfParser(string, DocType)->new RTFParser object"},
  {"parse", wrap_Parse, METH_VARARGS, "Parses the file"},
  {"make_doc", wrap_make_doc, METH_VARARGS, "Gets strings from file"},
  {"PyPlainTextDoctype", wrap_plain_text_doctype, METH_VARARGS,
   "makes a plain text doctype"},
  {"PySymbol", wrap_Symbol, METH_VARARGS,
   "PySymbol(string, string, int)->new Symbol object"},
  {"symbol", wrap_getsymbol, METH_VARARGS, "gets the symbol in a Symbol instance"},
  {"word", wrap_getword, METH_VARARGS, "gets the word in a Symbol instance"},
  {"line", wrap_getline, METH_VARARGS, "gets the line number in a Symbol instance"}, 
  {"PyDoctype", wrap_pydoctype, METH_VARARGS, "PyDoctype->new PyDoctype object"},
  {"pyset_callback", pydoctype_callback, METH_VARARGS, "Set the call back for pydoctype_callback"},
  {"PyRTFWriter", wrap_pywriter, METH_VARARGS, "PyRTFWriter(string, int, list<Symbol>)->new writer"},
  {"write", wrap_write, METH_VARARGS, "write(PyRTFWriter) -> rtf file"},
  {"get_doc", wrap_get_doc, METH_VARARGS, "gets the document stored in the parser"},
  {"replace", wrap_replace, METH_VARARGS, "replaces the strings in the document with the suppliet strings"},
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC init_PyRtfParser(void)
{
  PyObject *m = 
    Py_InitModule4(
		   "_PyRtfParser",   // name of the module
		   rtfparserMethods,  // name of the method table
		   "C++ RTF parser class", // doc string for module
		   0,   // last two never change
		   PYTHON_API_VERSION);
  if (m == NULL){
    return;
  }
  RTFError = PyErr_NewException("_PyRtfParser.error", NULL, NULL);
  Py_INCREF(RTFError);
  PyModule_AddObject(m, "error", RTFError);
}



