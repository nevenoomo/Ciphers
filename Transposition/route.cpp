#include "route.h"

vector<int> parseNums(string numbers);
bool is_valid(vector<int> v);

int main(int argc, char const *argv[]) {
  if (argc != 2)
    envokeError(string("USAGE - ") + string(argv[0]) + string(" file"),
                COMMAND_SYNTAX_ERROR);  // UGLY

  ifstream input(argv[1]);
  if (!input.is_open())
    envokeError(string("File ") + string(argv[1]) + string(" cannot be opened"),
                ACCESS_ERROR);

  string mode, numbers;
  input >> mode;
  input >> numbers;
  if (!input.good())
    envokeError(string("File structure is incorect"), FILE_SYNTAX_ERROR);
  while (input.peek() == '\n' || input.peek() == '\r') {  // skip endl
    input.ignore();
  }
  if (!input.good())
    envokeError(string("File structure is incorect"), FILE_SYNTAX_ERROR);

  vector<int> nums = parseNums(numbers);

  Cipherer c(nums.data(), nums.size(), input);  //&nums[0] if earlier then c++11
  if (mode == "encrypt") {
    c.encrypt();
  } else if (mode == "decrypt") {
    c.decrypt();
  } else {
    envokeError(string("Mode ") + mode + string(" is not recognised"),
                FILE_SYNTAX_ERROR);
  }
  getchar();
  return 0;
}

void envokeError(const string &message, int code) {
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