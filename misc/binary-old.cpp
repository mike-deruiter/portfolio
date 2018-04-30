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
    int t = i;  // scratch variable for determining j
    
    // determine length of number in advance
    for (;(t /= 2) != 0; ++j) {}

    /* number will be padded w/ zeroes & spaces will separate every 4 digits,
       so take those into account                                             */
    for (;j % 4 != 0; ++j) {}
    int spaces = (j / 4) - 1;

    // allocate memory
    char* b = new char[j + spaces + 1];

    // start at the end of the string.
    char* p = b + j + spaces;
    *p = '\0';

    // work backwards through the string placing digits.
    for (;i != 0; i /= 2) {
        *--p = '0' + (i % 2);
        // place spaces every 4 digits
        if (--j % 4 == 0) { 
            *--p = ' ';
        }
    }

    // pad the number w/ leading zeroes
    for (;j > 0; --j) {
        *--p = '0';
    }

    // return a C++-style string.
    return std::string(b);
}
