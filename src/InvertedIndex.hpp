#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct Posting {
    int doc_id;
    int term_frequency;
};

struct docInfo {
    std::string title;
    std::string url;
};

class InvertedIndex {
public:
    void add_document(int doc_id, const std::string& title, const std::string& url, const std::vector<std::string>& tokens);
    void print_index() const;
    const std::vector<Posting>* get_postings(const std::string& word) const;
    int get_total_docs() const;
    docInfo get_doc_info(int doc_id) const;

private:
    int total_docs = 0;
    std::unordered_map<std::string, std::vector<Posting>> index;
    std::unordered_map<int, docInfo> doc_metadata;
};