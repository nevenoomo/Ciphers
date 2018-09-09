#include <algorithm>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Cipherer {
  char** table;
  int* key;
  int rows;
  int capacity;
  int columns;
  ifstream* text;

  int gettextsize() const {
    int pos = text->tellg();
    text->seekg(0, ios_base::end);
    int textsize = (int)text->tellg() - pos;
    text->seekg(pos);

    return textsize;
  }

  void fillEnc() {
    char buf[columns];
    int textsize = gettextsize();
    int times = textsize / columns;
    if (textsize % columns != 0) times += 1;
    for (int i = 0; i < times; i++) {
      memset(buf, ' ', columns * sizeof(char));  // TODO works??
      text->read(buf, columns);
      if (rows == capacity) {
        table = (char**)realloc(
            table, (capacity += 10) * sizeof(char*));  // TODO check for error
      }
      table[rows] = (char*)malloc(columns * sizeof(char));
      memcpy(table[rows], buf, columns * sizeof(char));
      rows++;
    }
  }

  void fillDec() {
    int textsize = gettextsize();
    rows = textsize / columns;  // TODO if (textsize%columns!=0) then text has
                                // not been ecrypted correctly
    if (rows > capacity) {
      table =
          (char**)realloc(table, rows * sizeof(char*));  // TODO check for error
    }

    for (int i = 0; i < rows; i++) {
      table[i] = (char*)malloc(sizeof(char) * columns);
    }

    char buf[rows];
    for (int i = 0; i < columns; i++) {
      if (text->bad())
        ;  // TODO errorhandle
      text->read(buf, rows);
      setcolumn(key[i], buf);
    }
  }

 public:
  Cipherer(int* header, const int ncols, const ifstream& input)
      : rows(0), capacity(10), columns(ncols), text((ifstream*)&input) {
    table = (char**)malloc(sizeof(char*) * 10);
    key = (int*)malloc(sizeof(int) * columns);
    for (int i = 0; i < columns; i++) {
      key[header[i] - 1] = i;
    }
  }

  ~Cipherer() {
    for (int i = 0; i < rows; i++) {
      free(table[i]);
    }
    free(table);
    free(key);
  }

  void encrypt() {
    fillEnc();

    for (int i = 0; i < columns; i++) {
      cout << getcolumn(key[i]);
    }
  }

  void decrypt() {
    fillDec();

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        cout << table[i][j];
      }
    }
  }

  string getcolumn(int j) {
    stringstream s;

    for (int i = 0; i < rows; i++) {
      s << table[i][j];
    }

    return s.str();
  }

  void setcolumn(int j, char buf[]) {  // TODO buf[rows] ok?
    for (int i = 0; i < rows; i++) {
      table[i][j] = buf[i];
    }
  }
};