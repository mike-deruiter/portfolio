#include <iostream>
#include <string>

std::string int_to_binary(int);

int main()
{
    int i;

    while (std::cin >> i) {
        std::string b(int_to_binary(i));
        std::cout << b << std::endl;
    }

    return 0;
}

std::string int_to_binary(int i)
{
    int j = 1;  // length of the number
    int t = i;
        
    // determine length of number in advance
    while ((t /= 2) != 0) {
        ++j;
    }
    
    char* b = new char[j + 1];

    // start at the end of the string.
    char* p = b + j;
    *p = '\0';

    // work backwards through the string placing digits.
    while (i != 0) {
        *--p = '0' + (i % 2);
        i /= 2;
        --j;
    }
    
    // return a C++-style string.
    return std::string(b);
}
