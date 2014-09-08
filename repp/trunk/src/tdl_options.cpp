#include "tdl_options.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

string strip_quotes(string orig);

tdlOptions::~tdlOptions() {
	for (map<string, vector<string> *>::iterator it = _tdl_opts.begin();
			it != _tdl_opts.end(); ++it)
		delete it->second;
}

vector<string> *tdlOptions::lookup(string opt)
{
	if (_tdl_opts.count(opt) > 0)
		return _tdl_opts[opt];
	else
		return NULL;
}

//return first value for opt, else empty string. If you need to distinguish
//between an unset option and an empty option, use lookup()
string tdlOptions::get(string opt)
{
   if (_tdl_opts.count(opt) > 0)
      return (*_tdl_opts[opt])[0];
   else
      return string();
}

void tdlOptions::set(string opt, string val)
{
   if (_tdl_opts.count(opt) == 0)
      _tdl_opts[opt] = new vector<string>;
   else
      _tdl_opts[opt]->clear();
   _tdl_opts[opt]->push_back(val);
}

void tdlOptions::add(string opt, string val)
{
   if (_tdl_opts.count(opt) == 0)
      _tdl_opts[opt] = new vector<string>;
   _tdl_opts[opt]->push_back(val);
}

bool tdlOptions::member(string opt, string val) {
   if (_tdl_opts.count(opt) == 0)
      return false;
   for (vector<string>::iterator it = _tdl_opts[opt]->begin();
         it != _tdl_opts[opt]->end(); ++it) {
      if (it->compare(val) == 0) return true;
   }
   return false;
}

//read a PET-style settings file: 
// - a line starting with a semicolon is a comment
// - options are given as 
//    option := val1 val2 val3.
//   and can stretch over multiple lines.
// - multiple values should be given in one statement
// - only the last instantiation of an option is kept
void tdlOptions::read(string fname)
{
   ifstream cf(fname.c_str());
   enum FileState {OUT, ID, EQ};
   if (cf.is_open()) {
      int count = 0;
      FileState state = OUT;
      string line, opt, val;
      getline(cf, line);
      while (!cf.eof()) {
         count++;
         if (line.compare(0,1,";") == 0) {//comment
            getline(cf, line);
            continue;
         }
         istringstream linestr(line);
         string tok;
         while (linestr >> tok) {
            if (state == OUT) {
               opt = tok;
               if (_tdl_opts.count(opt) == 0)
                  _tdl_opts[opt] = new vector<string>;
					else
                  _tdl_opts[opt]->clear();
               state = ID;
            } else {
               if (state == ID) {
                  if (tok.compare(":=") == 0)
                     state = EQ;
                  else {
                     cerr << "Invalid configuration file " << fname
                        << ", expecting := at line " << count << "." << endl;
                     exit(1);
                  }
               } else {
                  if (state == EQ) {
                     if (tok[tok.length()-1] == '.') {
                        if (tok.length() > 1) {
                           tok.erase(tok.length()-1, 1);
                           _tdl_opts[opt]->push_back(strip_quotes(tok));
                        }
                        state = OUT;
                     } else {
                        _tdl_opts[opt]->push_back(strip_quotes(tok));
                     }
                  } else {
                     cerr << "huh? state is " << state << "?" << endl;
                     exit(1);
                  }
               }
            }
         }
         getline(cf, line);
      }
   } else {
      cerr << "Couldn't open " << fname << endl;
      exit(1);
   }
}

string strip_quotes(string orig)
{
   if (orig[0] == '"') orig.erase(0,1);
   if (orig[orig.length()-1] == '"') orig.erase(orig.length()-1, 1);
   return orig;
}

