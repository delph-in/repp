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
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "repp.h"
#include "unicode.h"
#include "tdl_options.h"
#include "token.h"

using namespace std;
namespace boostfs = boost::filesystem;

typedef vector<tToken *>::iterator tok_iter;
void parse_options(int ac, char **argv, ifstream *inputfile, 
	tdlOptions *repp_opts);

int main(int argc, char **argv)
{
	ifstream inputfile;
	tdlOptions *repp_opts = new tdlOptions();
	parse_options(argc, argv, &inputfile, repp_opts);
	initialize_encoding_converter("utf-8");
	tReppTokenizer *tok;
	try {
		tok = new tReppTokenizer(repp_opts);
	} catch (exception &e) {
		cerr << "Error initialising tokeniser: " << e.what() << endl;
		exit(1);
	}
	tTokenPrinter *printer = new tTokenPrinter(repp_opts->get("format"), cout);

	istream &input = (inputfile.is_open()?inputfile:cin);
	string line;
	int count = 0;
	getline(input, line);
	while (!input.eof()) {
		count++;
		if (line.empty()) {
			getline(input, line);
			continue;
		}
		vector<tToken *> tokens;
		try {
			tok->tokenize(line, tokens);
			for (tok_iter t = tokens.begin(); t != tokens.end(); ++t) {
				if (t != tokens.begin()) printer->tok_sep();
				printer->token(*t);
			}
			printer->line_sep();
			for (tok_iter t = tokens.begin(); t != tokens.end(); ++t)
				delete *t;
		} catch (exception &e) {
			cerr << "Error tokenising line " << count << ":" << e.what() << endl;
		}
		getline(input, line);
	}
	if (inputfile.is_open()) inputfile.close();

	finalize_encoding_converter();
	return 0;
}

void parse_options(int ac, char **av, ifstream *ifile, tdlOptions *repp_opts)
{
	string format, rpp_dir;
	namespace po = boost::program_options;
	po::options_description opts("Options");
	opts.add_options()
		("help,h,?", "This usage information.")
		("config,c", po::value<string>(), "Configuration file (REQUIRED).")
		("format", po::value<string>(&format),
			"Token format: string, line, triple (default string).")
		("rpp,r", po::value<string>(&rpp_dir),
			"Specify non-default location of directory containing repp modules."
			" (Default locations are relative to the config file. See README.)")

	;
	po::options_description hidden("Hidden options");
	hidden.add_options()
		("input-file", po::value<string>(), "input file")
	;
	po::options_description cmd_line("Command line options");
	cmd_line.add(opts).add(hidden);
	po::positional_options_description p;
	p.add("input-file", 1);

	po::variables_map vm;
	po::store(po::command_line_parser(ac, av).
		options(cmd_line).positional(p).run(), vm);
	notify(vm);

	if (vm.count("help")) {
		cout << "Usage: " << av[0] << " [options] [input-file]" << endl;
		cout << opts << endl;
		exit(0);
	}

	if (!vm.count("config")) {
		cerr << "No configuration file given, exiting.\n" << endl;
		cerr << "Usage: " << av[0] << " [options] [input-file]" << endl;
		cerr << opts << endl;
		exit(1);
	}
	try {
		string cfname = vm["config"].as<string>();
		boostfs::path cpath(cfname);
		if (cpath.has_parent_path())
			repp_opts->set("repp-configdir", cpath.parent_path().string());
		else
			repp_opts->set("repp-configdir", boostfs::current_path().string());
		repp_opts->read(cfname);
	} catch (exception &e) {
		cerr << "Error reading config file: " << e.what() << endl;
		exit(1);
	}
	if (vm.count("format")) {
		repp_opts->set("format", format);
	}
	if (repp_opts->lookup("format") == NULL) repp_opts->set("format", "string");
	else {
		string cformat = repp_opts->get("format");
		if (!(cformat.compare("string") == 0 || cformat.compare("line") == 0||
				cformat.compare("triple") == 0)) { 
			cerr << "Warning: invalid format ``" << format << "''. "
				<< "Setting format=string." << endl;
			repp_opts->set("format", "string");
		}
	}
	if (vm.count("rpp"))
		repp_opts->set("repp-directory", rpp_dir);

	if (vm.count("input-file")) {
		ifile->open(vm["input-file"].as<string>().c_str());
		if (!ifile->is_open()) {
			cerr << "Couldn't open " << vm["input-file"].as<string>() << endl;
			exit(1);
		}
	}
}

