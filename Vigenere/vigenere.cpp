#include "vigenere.h"

void envoke_error(const string& message, int code);
void delete_cntrl(string& m, string& n);

int main(int argc, char const* argv[]) {
  if (argc != 2) envoke_error("Filename expected", COMMAND_SYNTAX_ERROR);
  ifstream input(argv[1]);
  if (!input.is_open()) envoke_error("File is not accessible", ACCESS_ERROR);
  string mode, key;
  int key_len = -1, text_len = -1;
  getline(input, mode);
  input >> key_len >> key >> text_len;
  delete_cntrl(mode, key);
  if (!input.good()) envoke_error("File structure is incorect", SYNTAX_ERROR);
  if (key_len == -1 || text_len == -1)
    envoke_error("Wrong size given", SYNTAX_ERROR);
  if (key_len != (int)key.size())
    envoke_error("Wrong key size given", SYNTAX_ERROR);

  Vigenere v(key, key_len, input, text_len);
  if (mode == "encrypt") {
    v.encrypt();
  } else if (mode == "decrypt") {
    v.decrypt();
  } else {
    envoke_error("Mode not recognised", SYNTAX_ERROR);
  }
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

void envoke_error(const string& message, int code) {
  cerr << "ERROR: " << message << endl;
  getchar();
  exit(code);
}
