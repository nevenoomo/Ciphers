#pragma once
#include <iostream>
#include <inttypes.h>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <exception>

using namespace std;
typedef int (*prnd_func)(uint8_t *buf, int num);