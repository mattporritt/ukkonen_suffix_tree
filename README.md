[![Build Status](https://travis-ci.org/mattporritt/ukkonen_suffix_tree.svg?branch=master)](https://travis-ci.org/mattporritt/ukkonen_suffix_tree)

# Ukkonen's Suffix Tree
The aim of this project is to provide a full featured Generalised Suffix Tree library based on Ukkonen's Algorithm. Including a reference C application and Python interface.

Current features:

* Build and display Suffix Tree
* Get Longest Common Substring (LCS)
* Check if string contains substring
* Check for all occurrences of substring in string

## Command Line Build
To build the Command Line Interface (CLI) reference executable run the following after cloning the repository:
```
./configure
make
```
Once built the reference executable can be run by:
```
./suffix_tree
```

## Testing
The project has a set of self tests that can be used to check if any changes have broken existing functionality.
To run the self tests:
```
./configure
make
make test
```
Or to run after the CLI executable has been built:
```
./suffix_tree -t
```
## Development
The best place to start when extending or exploring current functionality is the `self_test()` function in the `main.c` file

## Usage
Basic usage can be found by running `suffix_tree --help`. This will output
```
Usage: suffix_tree [OPTION...] STRING [STRING]

  -l, --lcs                  Find longest common substring
  -p, --print                Print the tree for the supplied string
  -t, --test                 Run self tests
  -?, --help                 Give this help list
      --usage                Give a short usage message

```

## TODO
* Provide command line options for currently implemented features
* Implement functionality to get all common substrings between two strings
* Much more...
