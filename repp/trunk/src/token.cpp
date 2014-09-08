/* Copyright (c) 2013 Rebecca Dridan (rdrid@dridan.com)
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
#include "token.h"

using namespace std;

tTokenPrinter::tTokenPrinter(string f, ostream &out)
	:_out(&out)
{
	if (f.compare("line") == 0)
		_format = LINE;
	else {
		if (f.compare("triple") == 0)
			_format = TRIPLE;
		else
			_format = STRING;
	}
}

void tTokenPrinter::tok_sep() 
{
	switch(_format) {
		case LINE:
			*_out << endl;
			break;
		case TRIPLE:
			*_out << endl;
			break;
		case STRING:
			*_out << " ";
			break;
	}
}

void tTokenPrinter::line_sep() 
{
	switch(_format) {
		case LINE:
			*_out << "\n" << endl;
			break;
		case TRIPLE:
			*_out << "\n" << endl;
			break;
		case STRING:
			*_out << endl;
			break;
	}
}

void tTokenPrinter::token(tToken *t)
{
	switch(_format) {
		case LINE:
			*_out << t->surface();
			break;
		case TRIPLE:
			*_out << "(" << t->start() << ", " << t->end() << ", "
				<< t->surface() << ")";
			break;
		case STRING:
			*_out << t->surface();
			break;
	}
}

