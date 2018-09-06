#include <route.h>

int main(int argc, char const *argv[])
{
    ifstream input(argv[1]);
    //TODO error check
    string mode, numbers;
    input>>mode;
    input>>numbers;

    int* nums = parseNums(numbers);

    if (nums == 0){
        //TODO error!!!!
    }
    Cipherer c(nums, numbers.size()/2, input);
    if (mode == "encrypt"){
        c.encrypt();
    }else if(mode == "decrypt"){
        c.decrypt();
    }else{
        //TODO error
    }

    return 0;
}
