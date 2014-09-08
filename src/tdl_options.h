#ifndef _TDL_OPTIONS_H
#define _TDL_OPTIONS_H

#include <vector>
#include <string>
#include <map>

typedef std::vector<std::string> tdl_opt;

class tdlOptions {
	public:
		tdlOptions() {};
		~tdlOptions();
		void read(std::string file);
		tdl_opt *lookup(std::string opt);
		std::string get(std::string opt);
		void set(std::string opt, std::string val);
		void add(std::string opt, std::string val);
		bool member(std::string opt, std::string val);

	private:
		std::map<std::string, tdl_opt *>_tdl_opts;
};
		
#endif
