#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "InvertedIndex.hpp"


// Ingests a tokenized document into the index database
void InvertedIndex::add_document(int doc_id, const std::vector<std::string>& tokens) {
    ++total_docs;
    for(const std::string &word : tokens) { 

        auto it = index.find(word);

        //if word doesnt already exist in index
        if(it == index.end()) {
            std::vector<Posting> add_to_index;
            add_to_index.push_back(Posting{doc_id, 1});
            index.insert({word, add_to_index});
        }
        //if word already exists in index
        else {
            //current word posting vector
            std::vector<Posting> &current_vector = it->second;

            //if this document already exists in the index for this word:
            if(!current_vector.empty() && current_vector.back().doc_id==doc_id) {
                current_vector.back().term_frequency++;
            }
            //word exists in index but nto for this specific document
            else {
                current_vector.push_back(Posting{doc_id, 1});
            }

        }

    }

}

// Helper to print out the database layout to the terminal (great for testing)
void InvertedIndex::print_index() const {
    for(auto it = index.begin(); it != index.end(); ++it) {
        const std::vector<Posting> &posting_vector = it->second;
        std::cout << it->first << ": ";
        for(const auto &posting : posting_vector) {
            std::cout << "doc id: " << posting.doc_id << " term freq: " << posting.term_frequency << "||";
        }
        std::cout << "\n";
    }
}   

// Getter to look up a word's postings directly
const std::vector<Posting>* InvertedIndex::get_postings(const std::string& word) const {
    auto it = index.find(word);

    return &it->second;
}