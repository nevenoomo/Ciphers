#include "polybius.h"

void envoke_error(const string &message, int code);
bool validate_params(string &mode, string &lang, int num);
void delete_cntrl(string& m, string& n);

int main(int argc, char const *argv[]) {
  if (argc != 2) envoke_error("Filename expected", COMMAND_SYNTAX_ERROR);
  ifstream input(argv[1]);
  if (!input.is_open()) envoke_error("File is not accessible", ACCESS_ERROR);
  string mode, lang_and_number;
  getline(input, mode);
  getline(input, lang_and_number);
  delete_cntrl(mode, lang_and_number);
  if (!input.good()) envoke_error("File structure is incorect", SYNTAX_ERROR);
  string lang;
  int num = 0;
  stringstream s(lang_and_number);
  s >> lang >> num;
  if (num == 0 || lang == "")
    envoke_error("File structure is incorect", SYNTAX_ERROR);
  validate_params(mode, lang, num);
  Polybius(mode, lang, num, input).process();
  getchar();
  return 0;
}

void delete_cntrl(string& m, string& n) {
  bool first, second;
  while ((first = iscntrl(m.back())) || (second = iscntrl(n.back()))) {
    if (first) m = m.erase(m.size() - 1);
    if (second) n = n.erase(n.size() - 1);
  }
}

void envoke_error(const string &message, int code) {
  cerr << "ERROR: " << message << endl;
  getchar();
  exit(code);
}

bool validate_params(string &mode, string &lang, int num) {
  // num must be odd and only english is supported
  if (mode != "encrypt" && mode != "decrypt") {
    envoke_error("Mode is not recognosed", SYNTAX_ERROR);
  } else if (lang != "english") {
    envoke_error("Language is not supported", SYNTAX_ERROR);
  } else if (num % 2 == 0) {
    envoke_error("Odd number expected", SYNTAX_ERROR);
  } else if (num < 0) {
    envoke_error("Positive number expected", SYNTAX_ERROR);
  }
  return true;
}