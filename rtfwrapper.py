#!/usr/bin/env python
import _PyRtfParser

class RtfSymbol:
    def __init__(self, symbol, word):
        self.symbol = _PyRtfParser.PySymbol(symbol, word)
    def csymbol(self):
        return _PyRtfParser.symbol(self.symbol)
    def cword(self):
        return _PyRtfParser.word(self.symbol)
##
#@class RtfParser
#@brief a wrapper for the c module rtfparser
##
class RtfParser:
    def __init__(self, filename):
        self.parser = _PyRtfParser.PyRtfParser(filename)
    def parse(self):
        _PyRtfParser.parse(self.parser)
    def make_doc(self):
        return _PyRtfParser.make_doc(self.parser)
    def get_doc(self):
        return _PyRtfParser.get_doc(self.parser)

class RtfWriter:
    def __init__(self, filename, doc):
        self.writer = _PyRtfParser.PyRTFWriter(filename, len(doc), doc)
    def replace(self, strings):
        _PyRtfParser.replace(len(strings), strings, self.writer)
    def write(self):
        _PyRtfParser.write(self.writer)
"""
class Doctype:
    def __init__(self, process_func):
        print "yes"
        _PyRtfParser.pyset_callback(process_func)
class PlainTextDoctype(Doctype):
    def process(self, symbol):
        if symbol.symbol is 'p':
            return symbol.word
    def __init__(self):
        Doctype.__init__(self, self.process)

"""
def test():
    z = ["hello"]
    x = RtfParser("msrtftest1.rtf")
    x.parse()
    x.make_doc()
    y = RtfWriter("test1.rtf", x.get_doc())
    raw_input("Press enter to continue")
    y.replace(z)
    y.write()
