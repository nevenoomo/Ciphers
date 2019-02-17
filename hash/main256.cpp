#include "common.h"
#include "hash.h"

typedef void (*handler)(string &);

bool is_512 = HASH256;
vector<string> files;
mode_names mode = NONE;
bool check_options[5] = {false};
int exit_code = 0;

void print_help(ostream &);
bool is_help_flag(vector<string> &);
void process_flags(vector<string>);
void verify_state();
void process_files(handler);
void process_file_text(string &name);
void process_file_binary(string &name);
void process_file_check(string &name);
void delete_cntrl(string &m);
bool verify_line(string &m);
bool ishexdigit(char c);
FILE *parse_filename(string &);
uint8_t *parse_hash(string &);
uint8_t hexchartodig(char c);
void print_ending(size_t, size_t, size_t);

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        print_help(cout);
        exit(0);
    }
    try {
        vector<string> v(argv + 1, argv + argc);
        process_flags(v);
        verify_state();
        handler h;
        if (mode == TEXT) {
            h = process_file_text;
        } else if (mode == BINARY) {
            h = process_file_binary;
        } else {
            h = process_file_check;
        }
        process_files(h);
    } catch (logic_error e) {
        cerr << e.what() << endl;
        print_help(cerr);
        exit(1);
    }

    exit(exit_code);
}

void print_help(ostream &o) {
    o << "USAGE: ";
    o << ((is_512) ? "g512sum" : "g256sum");
    o << " [OPTION]... [FILE]...\n";
    o << "Compute checksum for FILE. If none or -, then stdin is used.\n\n"
         "  --help            Print description\n"
         "File processing options:\n"
         "  -t, --text        Read files as text. (Default)\n"
         "  -b, --binary      Read files as binary.\n"
         "  -c, --check       Check files from the configuration file:\n"
         "                    [given_sum] {'*', ' '}[filename]\n"
         "                    Compare given_sum with computed for filename. "
         "' ' - read as text\n"
         "                    '*' - read as binary.\n"
         "  --                Treat all following arguements as filenames.\n"
         "Check mode options:\n"
         "  --ignore-missing  Ignore missing files in configurations file.\n"
         "  --quiet           Do not print OK status for files.\n"
         "  --status          Do not print status to stdin. Use exit code "
         "instead.\n"
         "  --strict          Exit with non-null code if string format in "
         "configuration file is\n"
         "                    wrong."
         "  -w, --warn        Print warnings about incorrect string format "
         "in stderr.\n";
}

bool is_help_flag(vector<string> &fl) {
    vector<string>::iterator help = find(fl.begin(), fl.end(), "--help");
    vector<string>::iterator all_files = find(fl.begin(), fl.end(), "--");
    return (help != fl.end()) && (help < all_files);
}

void process_flags(vector<string> fl) {
    if (is_help_flag(fl)) {
        print_help(cout);
        exit(0);
    }
    for (size_t i = 0; i < fl.size(); ++i) {
        if (fl[i] == "-t" || fl[i] == "--text") {
            if (mode != NONE) {
                throw logic_error("The mode is ambiguous");
            } else {
                mode = TEXT;
            }
        } else if (fl[i] == "-b" || fl[i] == "--binary") {
            if (mode != NONE) {
                throw logic_error("The mode is ambiguous");
            } else {
                mode = BINARY;
            }
        } else if (fl[i] == "-c" || fl[i] == "--check") {
            if (mode != NONE) {
                throw logic_error("The mode is ambiguous");
            } else {
                mode = CHECK;
            }
        } else if (fl[i] == "--") {
            files.insert(files.end(), fl.begin() + i + 1, fl.end());
            break;
        } else if (fl[i] == "--ignore-missing") {
            check_options[IGNORE] = true;
        } else if (fl[i] == "--quiet") {
            check_options[QUIET] = true;
        } else if (fl[i] == "--status") {
            check_options[STATUS] = true;
        } else if (fl[i] == "--strict") {
            check_options[STRICT] = true;
        } else if (fl[i] == "-w" || fl[i] == "--warn") {
            check_options[WARN] = true;
        } else {
            if (fl[i][0] == '-' && fl[i] != "-") {
                throw logic_error(string("Unknown flag: ") + fl[i]);
            } else {
                files.push_back(fl[i]);
            }
        }
    }
    if (mode == NONE) {
        mode = TEXT;
    }
}

void verify_state() {
    bool check = false;
    for (size_t i = 0; i < 5; ++i) {
        check = check || check_options[i];
    }
    if (mode != CHECK && check) {
        throw logic_error("Cannot use check flags in non-check mode");
    }
    if (files.size() == 0) {
        files.push_back("-");
    }
}

void process_files(handler h) {
    for (string name : files) {
        (*h)(name);
    }
}

void process_file_text(string &name) {
    FILE *in;
    if (name == "-") {
        in = stdin;
    } else {
        in = fopen(name.data(), "r");
    }
    if (in == NULL) {
        cerr << ((is_512) ? "g512sum" : "g256sum") << ": " << name
             << ": No such file or directory" << endl;
        return;
    }
    uint8_t *hash = Streebog(in, is_512).H();
    for (size_t i = ((is_512) ? BLOCK_SIZE : BLOCK_SIZE / 2); i > 0; --i) {
        printf("%02x", hash[i-1]);
    }
    // for (size_t i = 0; i < ((is_512) ? BLOCK_SIZE : BLOCK_SIZE / 2); ++i) {
    //     printf("%02x", hash[i]);
    // }
    //This additional output is removed due to task renewal
    // cout << "  " << name << endl;
    fclose(in);
    free(hash);
}

void process_file_binary(string &name) {
    FILE *in;
    if (name == "-") {
        FILE *ret = freopen(NULL, "rb", stdin);
        if (ret == NULL) {
            cerr << ((is_512) ? "g512sum" : "g256sum") << ": "
                 << "Cannot read stdin in binary" << endl;
            return;
        }
        in = stdin;
    } else {
        in = fopen(name.data(), "rb");
    }
    if (in == NULL) {
        cerr << ((is_512) ? "g512sum" : "g256sum") << ": " << name
             << ": No such file or directory" << endl;
        return;
    }
    uint8_t *hash = Streebog(in, is_512).H();
    for (size_t i = ((is_512) ? BLOCK_SIZE : BLOCK_SIZE / 2); i > 0; --i) {
        printf("%02x", hash[i-1]);
    }
    // for (size_t i = 0; i < ((is_512) ? BLOCK_SIZE : BLOCK_SIZE / 2); ++i) {
    //     printf("%02x", hash[i]);
    // }
    //This additional output is removed due to task renewal
    // cout << " *" << name << endl;
    fclose(in);
    free(hash);
}

void process_file_check(string &name) {
    size_t hash_len = ((is_512) ? BLOCK_SIZE : BLOCK_SIZE / 2) * 2;
    istream *conf = NULL;
    ifstream f;
    if (name == "-") {
        conf = &cin;
    } else {
        f.open(name, ifstream::in);
        if (!f.is_open()) {
            cerr << ((is_512) ? "g512sum" : "g256sum") << ": " << name
                 << ": No such file or directory" << endl;
            return;
        }
        conf = &f;
    }
    size_t incorrect_format_lines = 0, not_read = 0, not_match = 0,
           line_num = 0;
    int c = ' ';

    while ((c = conf->get()) != EOF) {
        conf->unget();
        line_num++;
        string line;
        getline(*conf, line);
        delete_cntrl(line);
        if (!verify_line(line)) {
            if (check_options[STRICT]) {
                exit(1);
            } else if (check_options[WARN]) {
                cerr << ((is_512) ? "g512sum" : "g256sum") << ": '" << name
                     << "': " << line_num << ": improperly formatted "
                     << ((is_512) ? "g512" : "g256") << " checksum line"
                     << endl;
            }
            incorrect_format_lines++;
            continue;
        }
        FILE *in = parse_filename(line);
        if (in == NULL) {
            not_read++;
            continue;
        }

        uint8_t *assume_hash = parse_hash(line);
        uint8_t *real_hash = Streebog(in, is_512).H();

        if (memcmp(assume_hash, real_hash,
                   ((is_512) ? BLOCK_SIZE : (BLOCK_SIZE / 2))) != 0) {
            not_match++;
            if (!check_options[STATUS]) {
                cout << line.substr(hash_len + 2) << ": FAILED\n";
            }

        } else if (!check_options[QUIET] && !check_options[STATUS]) {
            cout << line.substr(hash_len + 2) << ": OK\n";
        }
        fclose(in);
        free(real_hash);
        free(assume_hash);
    }
    if (!check_options[STATUS] && incorrect_format_lines == line_num) {
        cerr << ((is_512) ? "g512sum" : "g256sum") << ": '" << name
             << "': no properly formated " << ((is_512) ? "g512" : "g256")
             << " checksum lines found" << endl;
    } else {
        print_ending(incorrect_format_lines, not_read, not_match);
    }
    if ((!check_options[IGNORE] && not_read != 0) || not_match != 0 ||
        line_num == incorrect_format_lines) {
        exit_code++;
    }
}

void delete_cntrl(string &m) {
    while (!m.empty() && iscntrl(m.back())) {
        m = m.erase(m.size() - 1);
    }
}

bool verify_line(string &line) {
    size_t hash_len = ((is_512) ? BLOCK_SIZE : BLOCK_SIZE / 2) * 2;
    // hash then 1 space, 1 space or *, at least 1 char for filenme
    if (line.size() <= hash_len + 3) {
        return false;
    }

    for (size_t i = 0; i < hash_len; ++i) {
        if (!ishexdigit(line[i])) {
            return false;
        }
    }

    if (line[hash_len] != ' ' ||
        (line[hash_len + 1] != ' ' && line[hash_len + 1] != '*') ||
        isspace(line[hash_len + 2]) || iscntrl(line[hash_len + 2])) {
        return false;
    }

    for (size_t i = hash_len + 2; i < line.size(); ++i) {
        // FIXME space may be in fname
        if (!(isalnum(line[i]) || ispunct(line[i]))) {
            return false;
        }
    }
    return true;
}

bool ishexdigit(char c) {
    return isdigit(c) || c == 'a' || c == 'b' || c == 'c' || c == 'd' ||
           c == 'e' || c == 'f';
}

FILE *parse_filename(string &line) {
    size_t hash_len = ((is_512) ? BLOCK_SIZE : BLOCK_SIZE / 2) * 2;
    string fname = line.substr(hash_len + 2);
    FILE *res = NULL;
    if (line[hash_len + 1] == ' ') {
        res = fopen(fname.c_str(), "r");
    } else {
        res = fopen(fname.c_str(), "rb");
    }

    if (res == NULL) {
        if (!check_options[IGNORE]) {
            cerr << ((is_512) ? "g512sum" : "g256sum") << ": " << fname
                 << ": No such file or directory" << endl;
            if (!check_options[STATUS]) {
                cout << fname << ": "
                     << "FAILED open or read" << endl;
            }
        }
    }
    return res;
}

uint8_t *parse_hash(string &line) {
    size_t hash_len = ((is_512) ? BLOCK_SIZE : (BLOCK_SIZE / 2));
    uint8_t *hash = (uint8_t *)malloc(hash_len);

    for (size_t i = 0; i < hash_len; ++i) {
        hash[i] =
            hexchartodig(line[2 * i]) * 16 + hexchartodig(line[2 * i + 1]);
    }

    return hash;
}

uint8_t hexchartodig(char c) {
    if (isdigit(c)) {
        return c - '0';
    } else if (c == 'a') {
        return 10;
    } else if (c == 'b') {
        return 11;
    } else if (c == 'c') {
        return 12;
    } else if (c == 'd') {
        return 13;
    } else if (c == 'e') {
        return 14;
    } else if (c == 'f') {
        return 15;
    }
    return 0;
}

void print_ending(size_t incorrect_format_lines, size_t not_read,
                  size_t not_match) {
    if (!check_options[STATUS]) {
        if (incorrect_format_lines != 0) {
            cerr << ((is_512) ? "g512sum" : "g256sum")
                 << ": WARNING: " << incorrect_format_lines << " line"
                 << ((incorrect_format_lines == 1) ? " is" : "s are")
                 << " improperly formatted" << endl;
        }
        if (not_read != 0 && !check_options[IGNORE]) {
            cerr << ((is_512) ? "g512sum" : "g256sum")
                 << ": WARNING: " << not_read << " listed file"
                 << ((not_read == 1) ? "" : "s") << " could not be read"
                 << endl;
        }
        if (not_match != 0) {
            cerr << ((is_512) ? "g512sum" : "g256sum")
                 << ": WARNING: " << not_match << " computed checksum"
                 << ((not_match == 1) ? "" : "s") << " did NOT match" << endl;
        }
    }
}