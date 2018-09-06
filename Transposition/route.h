#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

class Cipherer{
    int ** table;
    int rows;
    int size;
    int columns;
    ifstream text;

    int gettextsize(){
        int pos = text.tellg();
        text.seekg(0, ios_base::end);
        int textsize = text.tellg() - pos;
        text.seekg(pos);

        return textsize;
    }

    void fillEnc(){
        int buf[sizeof(int)*columns];
        while(!text.eof()){
            memset(buf, ' ', sizeof(buf));//TODO works??
            text.read(buf, sizeof(buf));
            if (rows == size){
                table = realloc(table, size += 10);//TODO check for error
            }
            table[++rows] = malloc(sizeof(buf));
            memcpy(table[rows], buf, sizeof(buf));
        }
    }

    void fillDec(){
        int textsize = gettextsize();
        rows += textsize/columns;
        if (rows > size){
            table = realloc(table, rows);//TODO check for error
        }
        
        for(size_t i = 1; i < rows; i++)
        {
            table[i] = malloc(sizeof(int)*columns);
        }

        int buf[rows-1];
        for(size_t i = 1; i <= columns; i++)
        {
            if(text.bad()) ; //TODO errorhandle 
            text.read(buf, sizeof(buf));
            setcolumn(i, buf);
        }  
    }
public:
    Cipherer(int * header, const int ncol, const ifstream& input): columns(ncols), text(input), rows(1), size(10){
        table = malloc(sizeof(int*)*10);
        table[0] = malloc(sizeof(int)*columns);
        memcpy(table[0], header, sizeof(int)*columns);//may just asign a ptr val, but its to dumb
    }

    ~Cipherer(){
        for(size_t i = 0; i < rows; i++)
        {
            free(table[i]);
        }
        free(table);
    }

    void encrypt(){
        fillEnc();
        
        for(size_t i = 1; i <= columns; i++)
        {
            cout<<getcolumn(i);
        }
    }

    void decrypt(){
        fillDec();

        for(size_t i = 1; i < rows; i++)
        {
            for(size_t j = 0; j < columns ; i++)
            {
                cout<<(char)table[i][j];
            }
        }
    }

    //TODO setcolumn getcolumn
}