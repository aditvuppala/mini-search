#include "InvertedIndex.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Updated add_document to store docInfo metadata
void InvertedIndex::add_document(int doc_id, const std::string& title, const std::string& url, const std::vector<std::string>& tokens) {
    ++total_docs;
    doc_metadata[doc_id] = docInfo{title, url};

    for(const std::string &word : tokens) { 
        auto it = index.find(word);

        if(it == index.end()) {
            std::vector<Posting> add_to_index;
            add_to_index.push_back(Posting{doc_id, 1});
            index.insert({word, add_to_index});
        }
        else {
            std::vector<Posting> &current_vector = it->second;

            if(!current_vector.empty() && current_vector.back().doc_id == doc_id) {
                current_vector.back().term_frequency++;
            }
            else {
                current_vector.push_back(Posting{doc_id, 1});
            }
        }
    }
}

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

// Safe look-up to prevent segfaults on missing words
const std::vector<Posting>* InvertedIndex::get_postings(const std::string& word) const {
    auto it = index.find(word);
    if (it != index.end()) {
        return &it->second;
    }
    return nullptr; // Returns nullptr safely if word not found
}

int InvertedIndex::get_total_docs() const {
    return total_docs;
}

docInfo InvertedIndex::get_doc_info(int doc_id) const {
    auto it = doc_metadata.find(doc_id);
    if (it != doc_metadata.end()) {
        return it->second;
    }
    return {"Unknown Document", "Unknown URL"};
}