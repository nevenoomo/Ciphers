#include "key_generation.h"
#include "common.h"
#include "params.h"
#include "rand.h"

#ifdef _GEN_PRIV_
char usage_str[] =
    "USAGE: genpkey [-s] file\n"
    "-s      использовать набор параметров: \n"
    "        id-tc26-gost-3410-2012-256-paramSetA (МР 26.2.002-2018 Б.1). \n"
    "        По умолчанию используется набор: \n"
    "        id-tc26-gost-3410-2012-512-paramSetС (МР 26.2.002-2018 Б.2). \n"
    "file    название файла (путь к файлу) c приватным ключем\n";
#else
char usage_str[] =
    "USAGE: convpkey [-s] file\n"
    "-s      использовать набор параметров: \n"
    "        id-tc26-gost-3410-2012-256-paramSetA (МР 26.2.002-2018 Б.1). \n"
    "        По умолчанию используется набор: \n"
    "        id-tc26-gost-3410-2012-512-paramSetС (МР 26.2.002-2018 Б.2). \n"
    "file    название файла (путь к файлу) c приватным ключем\n"
    "file2   название файла (путь к файлу), в который будет записан открытй "
    "ключ. По\n"
    "        умолчанию file.pub.\n";
#endif

void print_usage();
bool is_flag_on(char const **argv);
#ifdef _GEN_PRIV_
void gen_priv(const KeyGenerator &k, char const **argv, int argc);
#else
void gen_pub(const KeyGenerator &k, char const **argv, int argc);
#endif

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        print_usage();
        exit(0);
    }
    bool is_A = is_flag_on(argv);
    KeyGenerator k(rand_bytes, ECP::Params(is_A));

#ifdef _GEN_PRIV_
    gen_priv(k, argv, argc);
#else
    gen_pub(k, argv, argc);
#endif

    return 0;
}

void inline print_usage() { cout << usage_str; }

bool inline is_flag_on(char const **argv) { return string("-s") == argv[1]; }

void gen_priv(const KeyGenerator &k, char const **argv, int argc) {
    ofstream out_file;
    if (!is_flag_on(argv)) {
        if (argc == 2) {
            cerr << "No filename given\n";
        }
        out_file.open(argv[2], ios_base::binary);
    } else {
        out_file.open(argv[1], ios_base::binary);
    }

    if (!out_file.is_open()) {
        cerr << "ERROR: cannot open file specified\n";
        exit(1);
    }

    BigInteger d;
    k.gen_priv_key(d);
    out_file << d.get_data();
    out_file.close();
}

void gen_pub(const KeyGenerator &k, char const **argv, int argc) {
    ifstream in_file;
    ofstream out_file;

    if (!is_flag_on(argv)) {
        if (argc == 2) {
            cerr << "No filename given\n";
        }
        in_file.open(argv[2], ios_base::binary);
        if (argc > 3) {
            out_file.open(argv[3], ios_base::binary);
            if (!out_file.is_open()) {
                cerr << "ERROR: cannot open file specified\n";
                exit(1);
            }
        }else{
            out_file.open(string(argv[2])+".pub", ios_base::binary);
        }
    } else {
        in_file.open(argv[1], ios_base::binary);
        if (argc > 2) {
            out_file.open(argv[2], ios_base::binary);
            if (!out_file.is_open()) {
                cerr << "ERROR: cannot open file specified\n";
                exit(1);
            }
        }else{
            out_file.open(string(argv[1])+".pub", ios_base::binary);
        }
    }

    if (!in_file.is_open()) {
        cerr << "ERROR: cannot open file(s) specified\n";
        exit(1);
    }

    BigInteger d(ECP::Params(is_flag_on(argv)).q.size());
    in_file.read((char *)d.get_data(), d.size());
    ECP::Point Q;
    k.gen_pub_key(Q, d);
    out_file << Q.first() << " " << Q.second();

    in_file.close();
    out_file.close();
}
