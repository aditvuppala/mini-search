#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <unordered_set>
#include "Tokenizer.hpp"
#include "InvertedIndex.hpp"

using namespace std;

vector<string> process(string query) {
    //tokenize query into buzzwords
    Tokenizer myTokenizer;
    vector<string> tokenized_words = myTokenizer.tokenize(query);

    

}