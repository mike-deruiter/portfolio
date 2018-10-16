/* hangman.cpp (Prototype): 
   Theoretically solves Hangman, but does so very stupidly; the computer 
   always guesses letters in descending order of their frequency in the 
   dictionary. I don't know a word that it can correctly solve.          */

#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

#define DICT_NAME "/usr/share/dict/words"
#define MAX_WRONG   6

void hash_chars(const std::string);
void print_common_char(); // DEBUG
void play_hangman(const std::string);
char guess(std::vector<char>& guessed);

std::map<char, int> letters;

int main(int argc, char *argv[]) {
    std::ifstream dict {DICT_NAME, std::ifstream::in};
    std::string word;
    
    while (dict >> word)
        hash_chars(word);
    
    while (std::cin >> word)
        play_hangman(word);
}

void hash_chars(const std::string s) {
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

void play_hangman(const std::string w) {
    char g;
    int wrong_guesses = 0;
    std::vector<char> guessed;
    
    std::string so_far (w.length(), '_');
    
    while (wrong_guesses < MAX_WRONG && so_far != w) {
        if (g = guess(guessed)) {
            if (w.find(g) != std::string::npos) { // match
                for (int i = 0; i < w.length(); ++i) {
                    if (w[i] == g)
                        so_far[i] = g;
                }
            } else
                wrong_guesses++;
        } else
            return; // all letters guessed, we must have won
            
        std::cout << so_far << std::endl;
    }
}

char guess(std::vector<char>& guessed) {
    int max = 0;
    char guess = 0;
    
    for (auto const& c : letters) {
        if (c.second > max && std::find(guessed.begin(), guessed.end(), c.first) 
                              == guessed.end()) 
            {
                guess = c.first;
                max = c.second;
            }    
    }

    std::cerr << "Guess: " << guess << std::endl;
    guessed.push_back(guess);

    return guess;
}
