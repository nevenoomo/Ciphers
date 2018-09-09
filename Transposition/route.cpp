#include "route.h"

vector<int> parseNums(string numbers);
void validate(vector<int> v);

int main(int argc, char const *argv[]) {
  ifstream input(argv[1]);
  // TODO error check
  string mode, numbers;
  input >> mode;
  input >> numbers;
  while (input.peek() == '\n' || input.peek() == '\r') {  // skip endl
    input.ignore();
  }

  vector<int> nums = parseNums(numbers);

  Cipherer c(nums.data(), nums.size(), input);  //&nums[0] if earlier then c++11
  if (mode == "encrypt") {
    c.encrypt();
  } else if (mode == "decrypt") {
    c.decrypt();
  } else {
    // TODO error
  }
  getchar();
  return 0;
}

vector<int> parseNums(string numbers) {
  stringstream num;
  vector<int> v;
  bool comma = numbers[0] == ',';
  if (comma) {
    // TODO error in num order
  }
  if (numbers.size() == 0) {
    // TODO empty string error
  }

  for (size_t i = 0; i < numbers.size(); i++) {
    if (isdigit(numbers[i])) {
      num << numbers[i];
      comma = false;
    } else if (numbers[i] == ',') {
      if (comma)
        ;  // TODO error
      v.push_back(stoi(num.str()));
      num.str(string());  // reset stream
      comma = true;
    } else {
      // TODO error
    }
  }
  v.push_back(stoi(num.str()));
  if (comma)
    ;  // TODO nums end with comma error
  validate(v);
  return v;
}

void validate(vector<int> v) {  // TODO check if copies vector correctly
  sort(v.begin(), v.end());
  bool valid = true;

  for (int i = 0; i < (int)v.size() && valid; i++) {
    valid = v[i] == i + 1;
  }

  if (!valid) {
    // TODO not valid num sequence
  }
}