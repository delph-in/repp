#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>
#include <iostream>
#include <fstream>

class tToken {
	public:
		tToken(int s, int e, std::string t)
			:_start(s), _end(e), _surface(t) {};
		std::string surface() {return _surface;}
		void surface(std::string s) {_surface = s;}
		int start() {return _start;}
		void start(int a) {_start = a;}
		int end() {return _end;}
		void end(int a) {_end = a;}

	private:
		std::string _surface;
		int _start;
		int _end;
};

enum FormatType {STRING, LINE, TRIPLE, OFFSETS};
class tTokenPrinter {
	public:
		tTokenPrinter(std::string format, std::ostream &out);
		void tok_sep();
		void line_sep();
		void token(tToken *t);

	private:
		FormatType _format;
		std::ostream *_out;
};

#endif
