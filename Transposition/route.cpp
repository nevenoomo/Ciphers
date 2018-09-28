#include "route.h"

vector<int> parseNums(string numbers);
bool is_valid(vector<int> v);
void process(const string& mode, const Cipherer& c);
void delete_cntrl(string& m, string& n);

int main(int argc, char const* argv[]) {
  if (argc != 2)
    envokeError(string("USAGE - ") + string(argv[0]) + string(" file"),
                COMMAND_SYNTAX_ERROR);

  ifstream input(argv[1]);
  if (!input.is_open())
    envokeError(string("File ") + string(argv[1]) + string(" cannot be opened"),
                ACCESS_ERROR);

  string mode, numbers;
  getline(input, mode);
  getline(input, numbers);
  delete_cntrl(mode, numbers);
  if (!input.good())
    envokeError(string("File structure is incorect"), FILE_SYNTAX_ERROR);

  vector<int> nums = parseNums(numbers);
  Cipherer c(nums.data(), nums.size(), input);  //&nums[0] if earlier then c++11
  process(mode, c);
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

void process(const string& mode, const Cipherer& c) {
  if (mode == "encrypt") {
    c.encrypt();
  } else if (mode == "decrypt") {
    c.decrypt();
  } else {
    envokeError(string("Mode ") + mode + string(" is not recognised"),
                FILE_SYNTAX_ERROR);
  }
}

void envokeError(const string& message, int code) {
  cerr << "ERROR: " << message << endl;
  getchar();
  exit(code);
}

vector<int> parseNums(string numbers) {
  stringstream num;
  vector<int> v;
  bool comma = numbers[0] == ',';
  if (comma || numbers.size() == 0)
    envokeError(numbers + " is not a permutation", PERMUTATION_ERROR);

  for (size_t i = 0; i < numbers.size(); i++) {
    if (isdigit(numbers[i])) {
      num << numbers[i];
      comma = false;
    } else if (numbers[i] == ',') {
      if (comma)
        envokeError(numbers + " is not a permutation", PERMUTATION_ERROR);
      v.push_back(stoi(num.str()));
      num.str(string());  // reset stream
      comma = true;
    } else {
      envokeError(numbers + " is not a permutation", PERMUTATION_ERROR);
    }
  }
  v.push_back(stoi(num.str()));
  if (comma || !is_valid(v))
    envokeError(numbers + " is not a permutation", PERMUTATION_ERROR);
  return v;
}

bool is_valid(vector<int> v) {
  sort(v.begin(), v.end());
  bool valid = true;

  for (int i = 0; i < (int)v.size() && valid; i++) {
    valid = v[i] == i + 1;
  }

  return valid;
}