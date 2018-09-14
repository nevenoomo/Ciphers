#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

#define COMMAND_SYNTAX_ERROR 1
#define ACCESS_ERROR 2
#define FILE_SYNTAX_ERROR 3
#define UNVALID_TEXT 4

void envoke_error(const string &message, int code);

class Polybius
{
    bool enc;//true => encrypt, false => decrypt
    enum lang
    {
        ENGLISH
    } lang;
    ifstream & text;
    int rotation;
    vector<int> cipher;

    void fill(){
        int c = ' ', coordinate = 0, offset = 0;
        while((c = toupper(text.get()))!= EOF){
            if (!isalpha(c)) envoke_error("Text is not valid", UNVALID_TEXT);
            if (c == 'Z') c = 'Y';
            offset = c - 'A';
            coordinate = (offset/5+1)*10 + (offset%5+1);
            cipher.push_back(coordinate);
        }
    }
public:
    Polybius(string& mode, string& lang, int num, ifstream& input): text(input), rotation(num){
        if (lang == "english"){
            this->lang = ENGLISH;
        }
        this->enc = mode == "encrypt";
    }

    void process(){
        fill();
        
    }
};
