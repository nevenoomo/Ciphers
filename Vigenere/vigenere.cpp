#include "vigenere.h"

void envoke_error(const string &message, int code);
void delete_cntrl(string &m);
bool validate_key(string &k, int size);

int main(int argc, char const *argv[]) {
  if (argc != 2) envoke_error("Filename expected", COMMAND_SYNTAX_ERROR);
  ifstream input(argv[1]);
  if (!input.is_open()) envoke_error("File is not accessible", ACCESS_ERROR);
  string mode, key;
  int key_len = -1, text_len = -1;
  getline(input, mode);
  input >> key_len >> key >> text_len;
  if (!input.good()) envoke_error("File structure is incorect", SYNTAX_ERROR);
  delete_cntrl(mode);
  delete_cntrl(key);
  if (key_len <= 0 || text_len <= 0)
    envoke_error("Wrong size given", SYNTAX_ERROR);
  if (key_len > (int)key.size())
    envoke_error("Wrong key size given", SYNTAX_ERROR);
  if (!validate_key(key, key_len)) envoke_error("Bad key given", SYNTAX_ERROR);
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

bool validate_key(string &k, int size) {
  bool valid = true;
  for (int i = 0; i < size && valid; i++) {
    valid = valid && (isalpha(k[i]));
  }
  return valid;
}

void delete_cntrl(string &m) {
  while (!m.empty() && iscntrl(m.back())) {
    m = m.erase(m.size() - 1);
  }
}

void envoke_error(const string &message, int code) {
  cerr << "ERROR: " << message << endl;
  getchar();
  exit(code);
}
