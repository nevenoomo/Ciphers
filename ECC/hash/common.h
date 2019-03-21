#pragma once

#include <cstdio>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring>

#define BLOCK_SIZE 64
#define HASH512 true
#define HASH256 false
typedef uint8_t V512[BLOCK_SIZE];

enum mode_names{NONE, BINARY, TEXT, CHECK};
enum check_options_names{IGNORE, QUIET, STATUS, STRICT, WARN};

using namespace std;

