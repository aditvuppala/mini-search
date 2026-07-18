//this file should take in a string and return a vector of lowercase, punctuation free words
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <unordered_set>
#include "Tokenizer.hpp"

using namespace std;

Tokenizer::Tokenizer() {
    stop_words = {
        "the", "is", "at", "which", "on", "and", "a", "to", "in", "of", 
        "it", "for", "with", "as", "an", "by", "are", "this", "that", "from"
    };
}

vector<string> Tokenizer::tokenize(string input){

    //output vector
    vector<string> output;
    stringstream text(input);
    string word;

    while (text >> word) {

        string clean_word = "";
        for(char &c : word) {
            if(isalpha(c) || isalnum(c)) {
                clean_word += tolower(c);
            }
        }
        if(clean_word.length() && stop_words.count(clean_word) == 0) {
            output.push_back(clean_word);
        }   
    }

    return output;
}