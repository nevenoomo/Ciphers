#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define COMMAND_SYNTAX_ERROR 1
#define ACCESS_ERROR 2
#define FILE_SYNTAX_ERROR 3
#define PERMUTATION_ERROR 4

using namespace std;

void envokeError(const string& message, int code);

class Cipherer {
  char** table;
  int* key;
  size_t columns;
  size_t rows;
  ifstream* text;
  bool size_rounded;

  size_t gettextsize() const {
    size_t pos = text->tellg();
    text->seekg(0, ios_base::end);
    size_t textsize = (size_t)text->tellg() - pos;
    text->seekg(pos);

    return textsize;
  }

  void fillEnc() const {
    char buf[columns];
    for (size_t i = 0; i < rows; i++) {
      memset(buf, ' ', columns * sizeof(char));
      text->read(buf, columns);
      memcpy(table[i], buf, columns * sizeof(char));
    }
  }

  void fillDec() const {
    if (size_rounded)
      envokeError(string("Text has not been encrypted correctly"),
                  FILE_SYNTAX_ERROR);

    char* buf = (char*)malloc(rows * sizeof(char));
    for (size_t i = 0; i < columns; i++) {
      text->read(buf, rows);
      setcolumn(key[i], buf);
    }
    free(buf);
  }

 public:
  Cipherer(int* header, const int ncols, const ifstream& input)
      : columns(ncols), text((ifstream*)&input) {
    size_t textsize = gettextsize();
    rows = textsize / columns;
    size_rounded = textsize % columns != 0;
    if (size_rounded) rows += 1;
    table = (char**)malloc(sizeof(char*) * rows);
    for (size_t i = 0; i < rows; ++i) {
      table[i] = (char*)malloc(sizeof(char) * columns);
    }
    key = (int*)malloc(sizeof(int) * columns);
    for (size_t i = 0; i < columns; i++) {
      key[header[i] - 1] = i;
    }
  }

  ~Cipherer() {
    for (size_t i = 0; i < rows; i++) {
      free(table[i]);
    }
    free(table);
    free(key);
  }

  void encrypt() const {
    fillEnc();
    for (size_t i = 0; i < columns; i++) {
      cout << getcolumn(key[i]);
    }
  }

  void decrypt() const {
    fillDec();
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < columns; j++) {
        cout << table[i][j];
      }
    }
  }

  string getcolumn(int j) const {
    stringstream s;

    for (size_t i = 0; i < rows; i++) {
      s << table[i][j];
    }

    return s.str();
  }

  void setcolumn(int j, char buf[]) const {
    for (size_t i = 0; i < rows; i++) {
      table[i][j] = buf[i];
    }
  }
};