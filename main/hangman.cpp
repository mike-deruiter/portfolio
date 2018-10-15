/* hangman.cpp: Will eventually be a program that solves Hangman. */

#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#define DICT_NAME "/usr/share/dict/words"

void hash_chars(std::string);
void print_common_char();

std::map<char, int> letters;

int main(int argc, char *argv[]) {
    std::ifstream dict {DICT_NAME, std::ifstream::in};
    std::string w;
    
    while (dict >> w) {
        hash_chars(w);
    }
    
    print_common_char();
}

void hash_chars(std::string s) {
    int len = s.length();
    
    for (int i = 0; i < len; i++) {
        char c = s[i];
    
        if (! isupper(c) && ! islower(c))
            continue;
    
        if (isupper(c))
            c = tolower(c); 
    
        letters[c]++;
    }
}

void print_common_char() {
    for (auto const& c : letters) {
        std::cout << c.first << ": " << c.second << std::endl;
    }
}
