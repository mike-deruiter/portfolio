/* jumble - takes jumbled words as arguments & compares them to
 * /usr/share/dict/words, outputting all the words that the arguments
 * unscramble to.
 *  
 * Copyright (c) 2017, Mike DeRuiter. All rights reserved.
 * Send bugs to: aynrandjuggalo@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following condition is met:
 *   1) Redistributions of source code (if you're distributing a binary you
 *      don't have to bother. This source is potentially interesting for 
 *      educational purposes but the program itself is hardly the GNU 
 *      compiler) must retain the above copyright notice (please remove the 
 *      bit about sending bug reports to me), this list of one whole 
 *      condition (including the funny parts) & the following disclaimer:
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ''AS IS'' AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY 
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE.                                                               
 *
 * THIS DISCLAIMER HAS SUPER COW POWERS. (You have to include this part too
 * b/c it's funny to me)                                                      */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// constants
constexpr int SIZE = 20; // size of the array of nodes
constexpr const char* DICTIONARY = "/usr/share/dict/words"; /* location of
                                                               dictionary  */

// function prototypes
std::string jumble_word(std::string word);

// user-defined types
class Word_map {
    private:
        // definition for node
        struct map_node {
            unsigned long key;
            struct map_node *next;
        };

        map_node** map;
        int size;

        unsigned long convert_to_key(std::string k);
    public:
        // constructor
        Word_map(int s) : size{s}, map{new map_node*[s]} {
            for (int i = 0; i < size; ++i)
                map[i] = nullptr;
        }

        void insert(std::string k);
        bool contains(std::string k);
};

// main
int main(int argc, char** argv)
{
    std::vector<std::string> words_presort;
    Word_map word_map(SIZE);

    // get the words to unscramble from the arguments
    if (argc == 1) {
        std::cerr << argv[0] << ": 1 or more arguments expected." << std::endl;
        return 1;
    } else {
        for (int i = 1; argc-- > 1; ++i) {
            std::string word {argv[i]};
            words_presort.push_back(word);
        }
    }

    // sort the characters in the words alphabetically & add them to the map
    for (std::string word : words_presort) {
        std::string word_sort {jumble_word(word)};
        word_map.insert(word_sort);
    }

    std::ifstream dict(DICTIONARY, std::ifstream::in); // open the dictionary
    std::string dict_word;

    if (! dict) {
        std::cerr << argv[0] << ": cannot open dictionary." << std::endl;
        return 2;
    }
     
    /* sort the characters in each dictionary word alphabetically & compare 
       them to the map                                                      */ 
    while (dict >> dict_word) {
        std::string word_sort {jumble_word(dict_word)};
        if (word_map.contains(word_sort)) {
            std::cout << dict_word << std::endl;
        }
    }

    dict.close();

    return 0;
}

/* jumble_word - sort the characters in a word into alphabetic order. */
std::string jumble_word(std::string word) {
    char* jumble = (char*) word.c_str();
    std::sort(jumble, jumble + word.length());
    std::string word_sort {jumble};
    return word_sort;
}

/* convert_to_key - convert a string to a map key. */
unsigned long Word_map::convert_to_key(std::string s) {
    char* sk = (char*) s.c_str();
    unsigned long key = 0;
    char c;

    while ((c = *sk++) != 0)
        key = (key * 27) + (c - 'a') + 1;

    return key;
}

/* insert - insert a string into the map. */
void Word_map::insert(std::string s) {
    unsigned long key = convert_to_key(s);

    struct map_node* i = new struct map_node;
    i->key = key;
    i->next = nullptr;

    int code = key % size;

    struct map_node* n = map[code];

    if (n == nullptr) {
        map[code] = i;
    } else {
        while (n->next != nullptr) {
            n = n->next;
        }
        n->next = i;
    }
}

/* contains - check whether the map contains a string. */
bool Word_map::contains(std::string s) {
    unsigned long key = convert_to_key(s);
    int code = key % size;

    struct map_node* n = map[code];

    while (n != nullptr) {
        if (n->key == key) {
            return true;
        }
        n = n->next;
    }

    return false;
}
