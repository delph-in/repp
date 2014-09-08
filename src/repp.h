/* Copyright (c) 2011 -- 2013 Rebecca Dridan (rdrid@dridan.com)
 * Modified from the version in PET to remove dependencies unnecessary for
 * standalone operation - Rebecca Dridan rdrid@dridan.com 2013
 *
 * PET
 * Platform for Experimentation with efficient HPSG processing Techniques
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef _REPP_H_
#define _REPP_H_

#include <vector>
#include <string>
#include <map>
#include <set>
#include <boost/regex.hpp>
#include <boost/regex/icu.hpp>
#include "tdl_options.h"
#ifdef REPP_STANDALONE
#include "token.h"
typedef std::string myString;
typedef std::vector<tToken *> inp_list;
#else
#include "input-modules.h"
#endif

#define RPP_EXT ".rpp"

// defines three related classes: 
// * tReppTokenizer as the PET tokenizer class
// * tRepp as a class for each repp module
// * tReppRule virtual class which has implemented classes for each rule type


typedef boost::u32regex tRegex;
class tReppRule; //forward declaration of REPP rule class
class tRepp; //forward declaration of REPP class
typedef std::vector<tReppRule *> tReppGroup;

#ifdef REPP_STANDALONE
class tReppTokenizer {
#else
class tReppTokenizer : public tTokenizer {
#endif
  public:
    tReppTokenizer(tdlOptions *s=NULL);
    ~tReppTokenizer();
    void tokenize(myString s, inp_list &result);
    #ifndef REPP_STANDALONE
    std::string description() { return "REPP tokenizer"; }
    position_map position_mapping() {return NO_POSITION_MAP;}
    #endif
    

    tRepp *getRepp(std::string name){return _repps[name];}
    void addStartMap(std::vector<int> *map){_startmap.push_back(map);}
    void addEndMap(std::vector<int> *map){_endmap.push_back(map);}
    std::string repp_dir() {return _repp_dir;}

  private:
    std::string _format; //output format

    std::map<std::string, tRepp *> _repps; //modules

    std::vector<std::vector<int> *> _startmap; // for keeping track of 
    std::vector<std::vector<int> *> _endmap;   // characterisation

    std::string _repp_dir; //directory where the REPP rule files live

    tdlOptions *_repp_opts;
};

class tRepp {
  public:
    tRepp(std::string name, tReppTokenizer *parent);
    ~tRepp();
    tReppGroup *getGroup(int id){return _groups[id];}
    std::vector<tReppRule *> &rules(){return _rules;}
    void read_file(const char *fname) {}; //included files, unimplemented
    tRegex _tokenizer; // tokenisation pattern
    tReppTokenizer *getParent() {return _parent;}

  private:
    std::string _id; //repp name

    std::string _version; 
    std::map<int, tReppGroup *> _groups; //iterative groups
    tReppTokenizer *_parent; //tokenizer
    std::vector<tReppRule *> _rules; //repp rules in file order
};

class tReppRule {
  public:
    tReppRule(std::string type):_type(type){};
    std::string get_type() { return _type; };
    virtual std::string name() { return _type; };
    virtual std::string apply(tRepp *r, std::string item){ return
    std::string(); };

  protected:
    std::string _type;
};

class tReppFSRule: public tReppRule {
  public:
    tReppFSRule(std::string type, const char *target, const char *format);
    std::string apply(tRepp *r, std::string item);
    std::string name() { return _targetstr; };

  private:
    tRegex _target; //lhs of rule
    std::string _targetstr; //mostly for debugging
    std::string _format; //rhs of rule
    std::vector<int> _cgroups; //offsets of capture groups in format
};

class tReppGroupRule: public tReppRule {
  public:
    tReppGroupRule(std::string type, int group)
      :tReppRule(type), _group_id(group){};
    std::string apply(tRepp *r, std::string item);
    std::string name();

  private:
    int _group_id;
};

class tReppIncludeRule: public tReppRule {
  public:
    tReppIncludeRule(std::string type, std::string iname)
      :tReppRule(type), _iname(iname){};
    std::string apply(tRepp *r, std::string item);
    std::string name() {return _iname;};

  private:
    std::string _iname;
};

#endif
