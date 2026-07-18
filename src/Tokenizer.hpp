#ifndef TOKENIZER
#define TOKENIZER

#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <unordered_set>

using namespace std;

class Tokenizer {
public:
    // Constructor: This will initialize our ignore list automatically
    Tokenizer();

    std::vector<std::string> tokenize(std::string input);

private:
    std::unordered_set<std::string> stop_words;
};

#endif // TOKENIZER