# REPP

REPP (the Regular Expression Pre-Processor) uses a set of rules to
tokenise and normalise text, while maintaining character offset
information for each token.  For details, see
https://github.com/delph-in/docs/wiki/ReppTop and
https://aclanthology.org/P12-2074/.


## Installation

### Dependencies

You will need the following:

- a C++ compiler (g++, clang, etc)
- Boost Program_options
- Boost Filesystems
- Boost Regex
- ICU

### Build Steps

In the top-level directory, run:

    autoreconf -i
    ./configure
    make

If the above `configure` command fails with an error about missing ICU
libraries, try the following instead
([source](https://unicode-org.github.io/icu/userguide/icu4c/build.html#recommended-build-options)):

    CXXFLAGS="-DU_USING_ICU_NAMESPACE=1"` ./configure
    make


## Usage

    Usage: src/repp [options] [input-file]
    Options:
      -? [ --help ]         This usage information.
      -c [ --config ] arg   Configuration file (REQUIRED).
      --format arg          Token format: string, line, offsets, triple (default
                            string).
      -r [ --rpp ] arg      Specify non-default location of directory containing
                            repp modules. (Default locations are relative to the
                            config file. See README.)


A configuration file containing required options (see
[Configuration](#configuration) must be given. Input text will be
taken from input-file if given, else from STDIN.

The `--format` option is optional (and can be set in the config file). The
currently implemented output formats are:

- `string`: Tokens are delimited by spaces, line breaks as in the input.
- `line`: One token per line, double newline for each line in the input.
- `triple`: As for line, but tokens are represented as a start, end,
  token triple, where the start and end positions represent line-based
  inter-character positions in the input text.

The `--rpp` option is also optional and settable in the config file. It specifies
where to look for the repp modules. If not specified, it will look in three
standard places, in the following order:

- the directory where the configuration file is
- an `rpp/` subdirectory of the directory where the configuration file is
- an `rpp/` sibling directory of the directory where the configuration file is

To reduce complication and confusion, all modules are expected to be in the same
directory.


## Example

    cat test.txt | src/repp -c erg/repp.set --format triple


## Configuration

The format for options in the config file is:

    option := val1 val2 val3.

Multiple values are whitespace separated. Each option can be written over more
than one line, but must end with `.`.
Only the last instantiation of an option is retained.
Lines beginning with a semicolon are comments.
Some example configuration files are included in the [erg/](erg/) directory. The rules
used along with the English Resource Grammar for tokenising various styles of
English are included in the [erg/](erg/) directory.

The configuration file must define:

- `repp-modules`: the list of modules that are read on initialisation
- `repp-tokenizer`: the top level rule file

Other options:

- `repp-calls`: the subset of repp modules to be used at any one time. If this is
  not set, only the `repp-tokenizer` module will run. This is set separately
  from repp-modules to allow more fine-grained realtime control.
- `format`: token output format
- `repp-directory`: the directory where the repp rule files can be found
