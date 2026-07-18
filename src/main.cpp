#include <iostream>
#include <vector>
#include <string>
#include "Tokenizer.hpp"

int main(/*int argc, char* argv[]*/) {
    // 1. Instantiate our Tokenizer object
    Tokenizer my_tokenizer;

    // 2. Define a messy test sentence containing capitalization, punctuation, and stop-words
    std::string test_input = "The compilation succeeded! Pointers are fast, and C++ rules.";
    
    std::cout << "Original Text: \"" << test_input << "\"\n\n";

    // 3. Run the tokenizer assembly line
    std::vector<std::string> clean_tokens = my_tokenizer.tokenize(test_input);

    // 4. Print out the resulting vector elements
    std::cout << "--- Tokenizer Output Vector ---\n";
    for (size_t i = 0; i < clean_tokens.size(); ++i) {
        std::cout << "Token [" << i << "]: " << clean_tokens[i] << "\n";
    }

    return 0;
}