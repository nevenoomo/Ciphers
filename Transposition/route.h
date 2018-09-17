#include <algorithm>
#include <cctype>
#include <cstdlib>
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
  ifstream& text;
  bool size_rounded;

  size_t gettextsize() const {
    size_t pos = text.tellg(), cntrl = 0, textsize = 0;
    int c = ' ';

    while ((c = text.get()) != EOF) {
      if (iscntrl(c)) {
        cntrl++;
        continue;
      }
      textsize++;
    }

    text.clear();             // unset eof flag
    text.seekg(pos - cntrl);  // it's a hack, just seekg(pos) doesn't work
    return textsize;
  }

  void read_to_buf(char* buf, size_t size) const {
    int c = ' ';
    size_t i = 0;
    while (i < size) {
      if ((c = text.get()) == EOF) return;
      if (iscntrl(c)) continue;
      buf[i] = (char)c;
      i++;
    }
    return;  // DEBUG
  }

  void fillEnc() const {
    char buf[columns];
    for (size_t i = 0; i < rows; i++) {
      memset(buf, ' ', columns * sizeof(char));
      read_to_buf(buf, columns);
      memcpy(table[i], buf, columns * sizeof(char));
    }
  }

  void fillDec() const {
    if (size_rounded)
      envokeError(string("Text has not been encrypted correctly"),
                  FILE_SYNTAX_ERROR);

    char* buf = (char*)malloc(rows * sizeof(char));
    for (size_t i = 0; i < columns; i++) {
      read_to_buf(buf, rows);
      setcolumn(key[i], buf);
    }
    free(buf);
  }

 public:
  Cipherer(int* header, const int ncols, ifstream& input)
      : columns(ncols), text(input) {
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