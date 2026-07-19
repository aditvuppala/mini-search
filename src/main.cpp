#include <iostream>
#include <vector>
#include <string>
#include "Tokenizer.hpp"
#include "InvertedIndex.hpp"

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

    // Step 1: Instantiate exactly ONE master index
    InvertedIndex master_index;

    // Step 2: Create mock data representing pre-tokenized documents
    // This allows us to test the index without worrying about tokenizer bugs!
    std::vector<std::string> doc0_tokens = {"mini", "search", "engine", "search"};
    std::vector<std::string> doc1_tokens = {"cpp", "search", "data"};

    std::cout << "--- Ingesting Mock Documents ---\n";
    
    // Step 3: Add documents sequentially
    master_index.add_document(0, doc0_tokens);
    master_index.add_document(1, doc1_tokens);
    
    std::cout << "Ingestion complete!\n";

    // Step 4: Call your print function to check the internal state
    master_index.print_index();

    return 0;
}