#include "modes.h"

enum ARG { H, HELP, ECB, CTR, OFB, CBC, CFB, MAC, E, D, K, I, O, V };
typedef ARG MODE;

MODE fill_context(vector<string> &args);
void print_help(ostream &o);
void check(vector<string> &args, bool p[14], size_t &idx);
bool get_Key(const string &s);
bool get_IV(const string &s, MODE m);
bool get_IV32(FILE *f);
bool get_IV64(FILE *f);
bool get_I(const string &s);
bool get_O(const string &s);
void validate_params(bool p[14]);
Modes::Mode *get_cipher(MODE m);

FILE *i = stdin, *o = stdout;
vector<uint64_t> IV(1, 0);
uint32_t IV32 = 0;
string IV_filename("");
vector<uint32_t> Key(8, 0);
bool enc = true;

int main(int argc, char const *argv[]) {
    freopen(NULL, "rb", stdin);
    freopen(NULL, "wb", stdout);
    try {
        if (argc < 2) throw logic_error("Few arguments given");
        vector<string> args(argv, argv + argc);
        MODE m = fill_context(args);
        if (i == NULL || o == NULL)
            throw logic_error("Cannot reopen stdin and stdout");
        Modes::Mode *cipher = get_cipher(m);
        cipher->run(enc);
        delete cipher;
    } catch (logic_error e) {
        cerr << e.what() << endl;
        print_help(cerr);
        fclose(i);
        fclose(o);
        exit(1);
    }
    fclose(i);
    fclose(o);
    return 0;
}

MODE fill_context(vector<string> &args) {
    MODE m = ECB;
    bool params[14] = {false};
    for (size_t i = 1; i < args.size(); i++) 
        check(args, params, i);
    if (params[CTR])
        m = CTR;
    else if (params[OFB])
        m = OFB;
    else if (params[CBC])
        m = CBC;
    else if (params[CFB])
        m = CFB;
    else if (params[MAC])
        m = MAC;
    get_IV(IV_filename, m);
    validate_params(params);
    return m;
}

void print_help(ostream &i) {
    i << "USAGE: \n";
    i << "\tmagma [-h|--help]\n";
    i << "\tmagma [--ecb|--ctr|--ofb|--cbc|--cfb] {-e|-d} -k <key file> "
         "[options]\n";
    i << "\tmagma --mac -k <key file> [options]\n";
    i << "Encrypt/decrypt text/ciphertext\n\n";
    i << " -h --help        display this help and exit\n";
    i << " --ecb            Electronic Codebook mode, used default\n";
    i << " --ctr            Counter mode\n";
    i << " --ofb            Output Feedback mode\n";
    i << " --cbc            Cipher Block Chaining mode\n";
    i << " --cfb            Cipher Feedback mode\n";
    i << " --mac            Message authentication code\n";
    i << " -e               Encrypt given text\n";
    i << " -d               Decrypt given ciphertext\n";
    i << " -k <key_file>    File with key in binary format\n";
    i << "\nOptions:\n";
    i << " -i <input file>  Input file. If none given, stdin used until EOF\n";
    i << " -o <output file> Output file. If none given, stdout used\n";
    i << " -v <iv file>     Initialization vector. 32 bit for CTR; of size\n"
         "                  64*z where z is Natural for other modes. 0 if\n"
         "                  none given\n";
}

void check(vector<string> &args, bool p[14], size_t &idx) {
    string s = args[idx];
    if (s == "-h" || s == "--help") {
        print_help(cout);
        exit(0);
    } else if (s == "--ecb") {
        p[ECB] = true;
    } else if (s == "--ctr") {
        p[CTR] = true;
    } else if (s == "--ofb") {
        p[OFB] = true;
    } else if (s == "--cbc") {
        p[CBC] = true;
    } else if (s == "--cfb") {
        p[CFB] = true;
    } else if (s == "--mac") {
        p[MAC] = true;
    } else if (s == "-e") {
        p[E] = true;
        enc = true;
    } else if (s == "-d") {
        p[D] = true;
        enc = false;
    } else if (s == "-k") {
        if (p[K]) throw logic_error("Key file is ambiguous");
        if (idx < args.size() - 1)
            p[K] = get_Key(args[++idx]);
        else
            throw logic_error("Key file expected");
    } else if (s == "-v") {
        if (p[V]) throw logic_error("Initialization vector is ambiguous");
        if (idx < args.size() - 1) {
            p[V] = true;
            IV_filename = args[++idx];
        } else
            throw logic_error("Initialization vector expected");
    } else if (s == "-i") {
        if (p[I]) throw logic_error("Input file is ambiguous");
        if (idx < args.size() - 1)
            p[I] = get_I(args[++idx]);
        else
            throw logic_error("Input file expected");
    } else if (s == "-o") {
        if (p[O]) throw logic_error("Output file is ambiguous");
        if (idx < args.size() - 1)
            p[O] = get_O(args[++idx]);
        else
            throw logic_error("Output file expected");
    } else {
        throw logic_error(s + " option is not recognized");
    }

    if (p[ECB] + p[CTR] + p[CBC] + p[OFB] + p[CFB] + p[MAC] > 1)
        throw logic_error("Mode is ambiguous");
    if (p[E] + p[D] > 1)
        throw logic_error("Cannot decrypt and encrypt simultaneously");
}

bool get_Key(const string &s) {  // it is OK if key file is bigger then needed
    FILE *key_file = fopen(s.data(), "rb");
    uint8_t buf[4] = {0};
    if (key_file == NULL) throw logic_error(s + " - cannot open file");
    size_t bytes_read = 0;
    for (size_t i = 0; i < Key.size(); i++) {
        bytes_read = fread(buf, sizeof(uint8_t), 4, key_file);
        if (bytes_read != 4) throw logic_error("Key file is of wrong size");
        Key[i] = Utils::convert_to_num32(buf);
    }
    return true;
}

bool get_IV(const string &s, MODE m) {
    if (s == "") return true;
    FILE *iv_file = fopen(s.data(), "rb");
    if (iv_file == NULL) throw logic_error(s + " - cannot open file");
    if (m == CTR)
        get_IV32(iv_file);
    else
        get_IV64(iv_file);
    return true;
}

bool get_IV32(FILE *f) {
    uint8_t buf[4] = {0};
    size_t len = Utils::file_size(f);
    if (len != 4) throw logic_error("Wrong IV size");
    if (fread(buf, sizeof(uint8_t), 4, f) != 4)
        throw logic_error("Cannot read IV file");
    IV32 = Utils::convert_to_num32(buf);
    return true;
}

bool get_IV64(FILE *f) {
    IV.clear();
    uint8_t buf[8] = {0};
    size_t len = Utils::file_size(f), max_iter = len / 8;
    if (len % 8 != 0) throw logic_error("Wrong IV size");
    for (size_t i = 0; i < max_iter; i++) {
        if (fread(buf, sizeof(uint8_t), 8, f) != 8)
            throw logic_error("Cannot read IV file");
        IV.push_back(Utils::convert_to_num(buf));
    }
    return true;
}

bool get_I(const string &s) {
    FILE *i_file = fopen(s.data(), "rb");
    if (i_file == NULL) throw logic_error(s + " - cannot open file");
    i = i_file;
    return true;
}

bool get_O(const string &s) {
    FILE *o_file = fopen(s.data(), "wb");
    if (o_file == NULL) throw logic_error(s + " - cannot open file");
    o = o_file;
    return true;
}

void validate_params(bool p[14]) {
    if (p[MAC] && (p[E] || p[D]))
        throw logic_error("Cannot encrypt/decrypt in MAC mode");
    if (!p[K]) throw logic_error("Key file is mandatory");
    if ((p[ECB] || p[CTR] || p[OFB] || p[CBC] || p[CFB]) && !(p[E] || p[D]))
        throw logic_error("Decrypt/encrypt command arguement not given");
}

Modes::Mode *get_cipher(MODE m) {
    Modes::Mode *cipher;
    switch (m) {
        case ECB:
            cipher = new Modes::ECB(Key.data(), i, o);
            break;
        case CTR:
            cipher = new Modes::CTR(Key.data(), IV32, i, o);
            break;
        case OFB:
            cipher = new Modes::OFB(Key.data(), IV, i, o);
            break;
        case CBC:
            cipher = new Modes::CBC(Key.data(), IV, i, o);
            break;
        case CFB:
            cipher = new Modes::CFB(Key.data(), IV, i, o);
            break;
        case MAC:
            cipher = new Modes::MAC(Key.data(), i, o);
            break;
        case D:
        case E:
        case O:
        case V:
        case I:
        case K:
        case HELP:
        case H:
            break;
    }
    return cipher;
}