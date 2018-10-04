#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define COMMAND_SYNTAX_ERROR 1
#define ACCESS_ERROR 2
#define SYNTAX_ERROR 3
#define UNVALID_TEXT 4
#define ENG_DIM 5

void envoke_error(const string &message, int code);

class Polybius {
  bool enc;  // true => encrypt, false => decrypt
  enum lang { ENGLISH } lang;
  ifstream &text;
  int rotation;
  vector<bool> was_lower;
  vector<int> cipher;

  void fill() {
    int c = ' ', coordinate = 0, offset = 0;
    while ((c = text.get()) != EOF) {
      if (iscntrl(c)) continue;
      if (!isalpha(c)) envoke_error("Text is not valid", UNVALID_TEXT);
      was_lower.push_back(islower(c));
      c = toupper(c);
      if (c == 'J') c = 'I';
      offset = c - 'A' + ((c > 'I') ? (-1) : 0);
      coordinate = (offset / ENG_DIM + 1) * 10 + (offset % ENG_DIM + 1);
      cipher.push_back(coordinate);
    }
  }

  void rotate() {
    if (cipher.empty()) return;
    rotation %= (cipher.size() * 2);
    rotation /= 2;
    if (enc) {
      if (rotation != 0)
        std::rotate(cipher.begin(), cipher.begin() + rotation, cipher.end());
      rot1l();
    } else {
      if (rotation != 0)
        std::rotate(cipher.rbegin(), cipher.rbegin() + rotation, cipher.rend());
      rot1r();
    }
  }

  void rot1r() {
    int save = cipher.back() % 10, newcoord = 0;

    for (size_t i = 0; i < cipher.size(); i++) {
      newcoord = save * 10 + cipher[i] / 10;
      save = cipher[i] % 10;
      cipher[i] = newcoord;
    }
  }

  void rot1l() {
    int save = cipher.front() / 10, newcoord = 0;

    for (int i = cipher.size() - 1; i >= 0; i--) {
      newcoord = (cipher[i] % 10) * 10 + save;
      save = cipher[i] / 10;
      cipher[i] = newcoord;
    }
  }

  void print() {
    int offset = 0;
    char c = 0;
    for (size_t i = 0; i < cipher.size(); ++i) {
      offset = (cipher[i] / 10 - 1) * ENG_DIM + (cipher[i] % 10 - 1);
      c = 'A' + offset;
      if (c > 'I') c++;
      if (was_lower[i]) c = tolower(c);
      cout << c;
    }
  }

 public:
  Polybius(string &mode, string &language, int num, ifstream &input)
      : text(input), rotation(num) {
    if (language == "english") {
      lang = ENGLISH;
    }
    enc = mode == "encrypt";
    if (num < 0){
      this->enc = !enc;
      this->rotation = -num;
    }
  }

  void process() {
    fill();
    rotate();
    print();
  }
};
