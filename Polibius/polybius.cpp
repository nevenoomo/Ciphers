#include "polybius.h"

void envoke_error(const string &message, int code);
bool validate_params(string &mode, string &lang, int num);

int main(int argc, char const *argv[])
{
    if (argc != 2)
        envoke_error("Filename expected", COMMAND_SYNTAX_ERROR);

    ifstream input(argv[1]);
    if (!input.is_open())
        envoke_error("File is not accessible", ACCESS_ERROR);

    string mode, lang_and_number;
    getline(input, mode);
    getline(input, lang_and_number);
    if (!input.good())
        envoke_error("File structure is incorect", FILE_SYNTAX_ERROR);
    string lang;
    int num;
    stringstream s(lang_and_number);
    s >> lang >> num;
    if (!s.good())
        ; // TODO envoke error
    validate_params(mode, lang, num);
    Polybius p(mode, lang, num, input);
    p.process();
    getchar();
    return 0;
}

void envoke_error(const string &message, int code)
{
    cerr << "ERROR: " << message << endl;
    getchar();
    exit(code);
}

bool validate_params(string &mode, string &lang, int num)
{ //num must be odd and only english is supported
    if (mode != "encrypt" && mode != "decrypt")
    {
        envoke_error("Mode is not recognosed", FILE_SYNTAX_ERROR);
    }
    else if (lang != "english")
    {
        envoke_error("Language is not supported", FILE_SYNTAX_ERROR); //TODO try + string
    }
    else if (num % 2 == 0)
    {
        envoke_error("Odd number expected", FILE_SYNTAX_ERROR);
    }
    else if (num < 0){
        envoke_error("Positive number expected", FILE_SYNTAX_ERROR);
    }
    return true;
}