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
    KeyGenerator k(rand_bytes, (is_A) ? ECP::setA : ECP::setC);

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
    size_t size = is_flag_on(argv) ? 32 : 64;

    if (is_flag_on(argv)) {
        if (argc == 2) {
            cerr << "No filename given\n";
            exit(1);
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
    d.fit_to_size(size);
    reverse(d.get_data(), d.get_data() + d.size());
    out_file.write((char *)d.get_data(), size);
    out_file.close();
}

void gen_pub(const KeyGenerator &k, char const **argv, int argc) {
    ifstream in_file;
    ofstream out_file;
    size_t size = is_flag_on(argv) ? 32 : 64;

    if (is_flag_on(argv)) {
        if (argc == 2) {
            cerr << "No filename given\n";
            exit(1);
        }
        in_file.open(argv[2], ios_base::binary);
        if (argc > 3) {
            out_file.open(argv[3], ios_base::binary);
        } else {
            out_file.open(string(argv[2]) + ".pub", ios_base::binary);
        }
    } else {
        in_file.open(argv[1], ios_base::binary);
        if (argc > 2) {
            out_file.open(argv[2], ios_base::binary);
        } else {
            out_file.open(string(argv[1]) + ".pub", ios_base::binary);
        }
    }

    if (!in_file.is_open()) {
        cerr << "ERROR: cannot open private key file\n";
        exit(1);
    } else if (!out_file.is_open()) {
        cerr << "ERROR: cannot open output file\n";
        in_file.close();
        exit(1);
    }

    BigInteger d(size);

    in_file.read((char *)d.get_data(), d.size());
    if (in_file.gcount() != (int)d.size()) {
        cerr << "Unexpected private key size. Check -s option\n";
        in_file.close();
        out_file.close();
        exit(1);
    }

    reverse(d.get_data(), d.get_data() + d.size());
    ECP::Point Q(BigInteger::ZERO, BigInteger::ZERO,
                 (size == 32) ? ECP::setA : ECP::setA);
    k.gen_pub_key(Q, d);

    reverse(Q.get_first_data(), Q.get_first_data() + size);
    reverse(Q.get_second_data(), Q.get_second_data() + size);

    out_file.write((char *)Q.get_first_data(), size);
    out_file.write((char *)Q.get_second_data(), size);

    in_file.close();
    out_file.close();
}
