#include <iostream>
#include <cstdlib>
using namespace std;

/* the maximum int is 10 characters long, + 1 for the optional sign */
#define MAX_LEN 11

// function prototypes
bool parse_fraction(const string frac_str, int& num, int& denom);
bool is_digit(const char d);
bool is_separator(const char s);

// user-defined types
class fraction {
    public:
        fraction(int n, int d) {
            num = n;
            denom = d;
        }

        void simplify() {
            int g = gcd(num, denom);
            num /= g;
            denom /= g;
        }

        friend ostream& operator<<(ostream& os, const fraction& f) {
            return os << f.num << "/" << f.denom;
        }
    private:
        int num, denom;
        int gcd(const int num, const int denom);
};

// main
int main() 
{
    string frac_str;
    int num, denom;

    while (cin >> frac_str) {
        if (!parse_fraction(frac_str, num, denom)) {
            cerr << frac_str << ": invalid" << endl;
            continue;
        }

        fraction frac(num, denom);
        frac.simplify();

        cout << frac << endl;
    }
   
    return 0;
}

// function definitions
bool parse_fraction(const string frac_str, int& num, int& denom) 
{
    char n[MAX_LEN + 1], d[MAX_LEN + 1];
    int i, j;
    
    j = 0;
    
    // read numerator
    i = 0;
    while (is_digit(frac_str[j])) {
        n[i++] = frac_str[j++];     // copy characters
        if (i > MAX_LEN)
            return false;
    }
    n[i] = '\0';
    num = atoi(n);                  // convert to number

    // skip over whitespace & slash
    while (is_separator(frac_str[j]))
        j++;
        
    /* abort if we've reached the end of the string or we're not at the 
       denominator now                                                  */
    if (j > frac_str.length() || !is_digit(frac_str[j]))
        return false;

    // read denominator
    i = 0;
    while (is_digit(frac_str[j])) {
        d[i++] = frac_str[j++];     // copy characters
        if (i > MAX_LEN)
            return false;
    }
    d[i] = '\0';
    denom = atoi(d);                // convert to number

    if (denom == 0)
        return false;

    return true;
}

bool is_digit(const char d)
{
    if (d >= '0' && d <= '9')
        return true;
    return false;
}

bool is_separator(const char s)
{
    if (s == ' ' || s == '\t' || s == '/')
        return true;
    return false;
}

int fraction::gcd(const int num, const int denom) {
    // Euclid's algorithm
    int temp;
    int n = num, d = denom;

    while (n > 0) {
        if (n < d) {
            temp = n;
            n = d;
            d = temp;
        }
        n %= d;
    }

    return d;                                    
};
