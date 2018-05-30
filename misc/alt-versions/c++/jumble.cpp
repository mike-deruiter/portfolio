/* jumble - takes jumbled words as arguments & compares them to
   /usr/share/dict/words, outputting all the words that the arguments
   unscramble to.                                                     */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

// constants
constexpr int SIZE = 20; // size of the array of nodes
constexpr const char* DICTIONARY = "/usr/share/dict/words"; /* location of
                                                               dictionary  */

// function prototypes
string jumble_word(string word);

// user-defined types
class Word_map {
    private:
        // definition for node
        struct map_node {
            unsigned long key;
            struct map_node *next;
        };

        struct map_node** map;
        int size;

        unsigned long convert_to_key(std::string k);
    public:
        // constructor
        Word_map(int s) : size{s}, map{new map_node*[s]} {
            for (int i = 0; i < size; ++i)
                map[i] = nullptr;
        }

        void insert(string k);
        bool contains(string k);
};

// main
int main(int argc, char** argv)
{
    vector<string> words_presort;
    Word_map word_map(SIZE);

    // get the words to unscramble from the arguments
    if (argc == 1) {
        cerr << argv[0] << ": 1 or more arguments expected." << endl;
        return 1;
    } else {
        for (int i = 1; argc-- > 1; ++i) {
            string word {argv[i]};
            words_presort.push_back(word);
        }
    }

    // sort the characters in the words alphabetically & add them to the map
    for (string word : words_presort) {
        string word_sort {jumble_word(word)};
        word_map.insert(word_sort);
    }

    ifstream dict(DICTIONARY, ifstream::in); // open the dictionary
    string dict_word;

    if (! dict) {
        cerr << argv[0] << ": cannot open dictionary." << endl;
        return 2;
    }
     
    /* sort the characters in each dictionary word alphabetically & compare 
       them to the map                                                      */ 
    while (dict >> dict_word) {
        string word_sort {jumble_word(dict_word)};
        if (word_map.contains(word_sort)) {
            cout << dict_word << endl;
        }
    }

    dict.close();

    return 0;
}

/* jumble_word - sort the characters in a word into alphabetic order. */
string jumble_word(string word) {
    char* jumble = (char*) word.c_str();
    sort(jumble, jumble + word.length());
    string word_sort {jumble};
    return word_sort;
}

/* convert_to_key - convert a string to a map key. */
unsigned long Word_map::convert_to_key(string s) {
    char* sk = (char*) s.c_str();
    unsigned long key = 0;
    char c;

    while ((c = *sk++) != 0)
        key = (key * 27) + (c - 'a') + 1;

    return key;
}

/* insert - insert a string into the map. */
void Word_map::insert(string s) {
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
bool Word_map::contains(string s) {
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
