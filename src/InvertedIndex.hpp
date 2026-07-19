#ifndef INVERTED_INDEX_HPP
#define INVERTED_INDEX_HPP

#include <string>
#include <vector>
#include <unordered_map>

// Represents a word's presence inside a specific document
struct Posting {
    int doc_id;
    int term_frequency; 
};

class InvertedIndex {
public:
    // Ingests a tokenized document into the index database
    void add_document(int doc_id, const std::vector<std::string>& tokens);

    // Helper to print out the database layout to the terminal (great for testing)
    void print_index() const;

    // Getter to look up a word's postings directly
    const std::vector<Posting>* get_postings(const std::string& word) const;

private:
    std::unordered_map<std::string, std::vector<Posting>> index;
    int total_docs = 0; 
};

#endif // INVERTED_INDEX_HPP