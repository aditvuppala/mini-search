#include <iostream>
#include <vector>
#include <string>
#include "Tokenizer.hpp"
#include "InvertedIndex.hpp"
#include "tf-idf.hpp"
#include "QueryProcessor.hpp"

using namespace std;

int main() {
    InvertedIndex index;
    Tokenizer tokenizer;

    // Add 3 documents with distinct terms
    index.add_document(0, tokenizer.tokenize("c++ vector tutorial"));
    index.add_document(1, tokenizer.tokenize("python web crawler scraping"));
    index.add_document(2, tokenizer.tokenize("fast c++ search engine inverted index"));

    // Query for terms that appear in SOME but NOT ALL documents
    std::string query = "python crawler"; 
    auto query_stats = process(query, index);

    // Rank results
    auto results = rank_documents(query_stats, index.get_total_docs());

    std::cout << "--- Search Results for: '" << query << "' ---\n";
    for (const auto& res : results) {
        std::cout << "Doc ID: " << res.doc_id << " | Score: " << res.score << "\n";
    }

    return 0;
}