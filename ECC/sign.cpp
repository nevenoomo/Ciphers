#include "ECC.h"
#include "common.h"
#include "key_generation.h"
#include "rand.h"

#ifdef __SIGN__
char usage_str[] =
    "USAGE: sign [-s] priv-key file [crt]\n"
    "-s         использовать набор параметров:\n"
    "           id-tc26-gost-3410-2012-256-paramSetA (МР 26.2.002-2018 Б.1).\n"
    "           По умолчанию используется набор:\n"
    "           id-tc26-gost-3410-2012-512-paramSetС (МР 26.2.002-2018 Б.2).\n"
    "           priv-key – название файла (путь к файлу) с приватным ключом.\n"
    "pub-key    название файла (путь к файлу) с открытым ключом.\n"
    "file       название файла (путь к файлу), для которого вычисляется ЭЦП.\n"
    "crt        название файла (путь к файлу) c подписью. По умолчанию "
    "file.crt.\n";
#else
char usage_str[] =
    "USAGE: verify [-s] pub-key file [crt]\n"
    "-s         использовать набор параметров:\n"
    "           id-tc26-gost-3410-2012-256-paramSetA (МР 26.2.002-2018 Б.1).\n"
    "           По умолчанию используется набор:\n"
    "           id-tc26-gost-3410-2012-512-paramSetС (МР 26.2.002-2018 Б.2).\n"
    "           priv-key – название файла (путь к файлу) с приватным ключом.\n"
    "pub-key    название файла (путь к файлу) с открытым ключом.\n"
    "file       название файла (путь к файлу), для которого вычисляется ЭЦП.\n"
    "crt        название файла (путь к файлу) c подписью. По умолчанию "
    "file.crt.\n";
#endif

void inline print_usage() { cout << usage_str; }

bool inline is_flag_on(char const **argv) { return string("-s") == argv[1]; }

void prologue1(int argc, char const *argv[], ifstream &key, ifstream &in,
               ofstream &out) {
    bool is_512 = is_flag_on(argv);
    int min_argc = is_512 ? 2 : 1;

    if (argc < min_argc + 2) {
        cerr << "Too few arguements\n";
        print_usage();
        exit(1);
    }

    key.open(argv[min_argc], ios_base::binary);
    if (!key.is_open()) {
        cerr << "Cannot open file with key\n";
        exit(1);
    }

    in.open(argv[min_argc + 1], ios_base::binary);
    if (!in.is_open()) {
        cerr << "Cannot open message file\n";
        exit(1);
    }

    if (argc > min_argc + 2) {
        out.open(argv[min_argc + 2], ios_base::binary);
    } else {
        out.open(string(argv[min_argc + 1]) + ".crt", ios_base::binary);
    }

    if (!out.is_open()) {
        cerr << "Cannot open output file\n";
        exit(1);
    }
}

void prologue2(int argc, char const *argv[], ifstream &key, ifstream &in,
               ifstream &crt) {
    bool is_512 = is_flag_on(argv);
    int min_argc = is_512 ? 2 : 1;

    if (argc < min_argc + 2) {
        cerr << "Too few arguements\n";
        print_usage();
        exit(1);
    }

    key.open(argv[min_argc], ios_base::binary);
    if (!key.is_open()) {
        cerr << "Cannot open file with key\n";
        exit(1);
    }

    in.open(argv[min_argc + 1], ios_base::binary);
    if (!in.is_open()) {
        cerr << "Cannot open message file\n";
        exit(1);
    }

    if (argc > min_argc + 2) {
        crt.open(argv[min_argc + 2], ios_base::binary);
    } else {
        crt.open(string(argv[min_argc + 1]) + ".crt", ios_base::binary);
    }

    if (!crt.is_open()) {
        cerr << "Cannot open crt file\n";
        exit(1);
    }
}

void sign(int argc, char const *argv[]) {
    ifstream key;
    ifstream in;
    ofstream out;
    int size = is_flag_on(argv) ? 32 : 64;

    prologue1(argc, argv, key, in, out);
    BigInteger d(size);

    key.read((char *)d.get_data(), size);
    if (key.gcount() != size) {
        cerr << "Bad key\n";
        exit(1);
    }
    reverse(d.get_data(), d.get_data() + size);
    ECC signer(rand_bytes, size == 64 ? ECP::setC : ECP::setA);

    uint8_t buf[128];
    size_t bytes_read;
    while (!in.eof()) { // REVIEW
        in.read((char *)buf, sizeof(buf));
        bytes_read = in.gcount();
        if (bytes_read)
            signer.update(buf, bytes_read);
    }
    signer.sign(buf, d);

    out.write((char *)buf, size * 2);
}

void verify(int argc, char const *argv[]) {
    ifstream key;
    ifstream in;
    ifstream crt;
    int size = is_flag_on(argv) ? 32 : 64;

    prologue2(argc, argv, key, in, crt);
    ECP::Point Q(BigInteger::ZERO, BigInteger::ONE, size == 64 ? ECP::setC : ECP::setA);

    key.read((char *)Q.get_first_data(), size);
    if (key.gcount() != size) {
        cerr << "Bad key\n";
        exit(1);
    }

    reverse(Q.get_first_data(), Q.get_first_data() + size);

    key.read((char *)Q.get_second_data(), size);
    if (key.gcount() != size) {
        cerr << "Bad key\n";
        exit(1);
    }

    reverse(Q.get_second_data(), Q.get_second_data() + size);

    ECC signer(rand_bytes, size == 64 ? ECP::setC : ECP::setA);

    uint8_t buf[128];
    size_t bytes_read;
    while (!in.eof()) { // REVIEW
        in.read((char *)buf, sizeof(buf));
        bytes_read = in.gcount();
        if (bytes_read)
            signer.update(buf, bytes_read);
    }

    crt.read((char *)buf, size * 2);

    if (signer.verify(buf, Q)) {
        cout << "Valid!\n";
        return;
    }
    cout << "Invalid\n";
    exit(1);
}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        print_usage();
        exit(1);
    }

#ifdef __SIGN__
    sign(argc, argv);
#else
    verify(argc, argv);
#endif

    return 0;
}
