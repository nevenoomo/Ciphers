#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#define COMMAND_SYNTAX_ERROR 1
#define ACCESS_ERROR 2
#define SYNTAX_ERROR 3
#define UNVALID_TEXT 4
#define ALPHA_L 26

void envoke_error(const string& message, int code);

class Vigenere {
  const string& key;
  int key_s;
  istream& text;
  int text_size;

 public:
  Vigenere(const string& key, int key_s, istream& input, int text_size)
      : key(key), key_s(key_s), text(input), text_size(text_size) {
    int c = ' ';
    text.ignore();
    while (isspace(c = text.get())) {
      cout << (char)c;
    }
    text.unget();
  }

  void encrypt() {
    int c = ' ', i = 0, j = 0;
    bool was_lower = false;
    char ciphered = ' ';
    while (i < text_size && (c = text.get()) != EOF) {
      if (isspace(c) || ispunct(c) || isdigit(c)) {
        cout << (char)c;
        if (c != '\n') i++;
        continue;
      }
      if (!isalpha(c)) envoke_error("Bad text", UNVALID_TEXT);
      was_lower = islower(c);
      c = toupper(c);
      ciphered = ((c - 'A') + (toupper(key[j % key_s]) - 'A')) % ALPHA_L + 'A';
      if (was_lower) ciphered = tolower(ciphered);
      cout << ciphered;
      i++;
      j++;
    }
    if (i != text_size) envoke_error("Wrong length given", SYNTAX_ERROR);
  }

  void decrypt() {
    int c = ' ', i = 0, j = 0;
    bool was_lower = false;
    char d = ' ';
    while (i < text_size && (c = text.get()) != EOF) {
      if (isspace(c) || ispunct(c) || isdigit(c)) {
        cout << (char)c;
        if (c != '\n') i++;
        continue;
      }
      if (!isalpha(c)) envoke_error("Bad text", UNVALID_TEXT);
      was_lower = islower(c);
      c = toupper(c);
      d = ((c - 'A') - (toupper(key[j % key_s]) - 'A') + ALPHA_L) % ALPHA_L +
          'A';
      if (was_lower) d = tolower(d);
      cout << d;
      i++;
      j++;
    }
    if (i != text_size) envoke_error("Wrong length given", SYNTAX_ERROR);
  }
};