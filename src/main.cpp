#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Tokenizer.hpp"
#include "InvertedIndex.hpp"
#include "tf-idf.hpp"
#include "QueryProcessor.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

bool load_documents_from_json(const std::string& filepath, InvertedIndex& index) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filepath << "\n";
        return false;
    }

    json data;
    try {
        file >> data;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON Parse error: " << e.what() << "\n";
        return false;
    }

    Tokenizer tokenizer;

    std::cout << "Indexing documents from " << filepath << "...\n";
    for (const auto& doc : data) {
        int doc_id = doc["doc_id"].get<int>();
        std::string title = doc["title"].get<std::string>();
        std::string url = doc["url"].get<std::string>();
        std::string content = doc["content"].get<std::string>();

        // Tokenize content and add to index
        std::vector<std::string> tokens = tokenizer.tokenize(content);
        index.add_document(doc_id, title, url, tokens);
    }

    std::cout << "Successfully indexed " << index.get_total_docs() << " documents!\n\n";
    return true;
}

int main() {
    InvertedIndex index;

    // Load JSON scraped by Python crawler
    if (!load_documents_from_json("scraped_pages.json", index)) {
        return 1;
    }

    std::cout << "======================================\n";
    std::cout << "   Mini Search Engine Ready! (C++)    \n";
    std::cout << "======================================\n";

    std::string query;
    while (true) {
        std::cout << "\nEnter search query (or type ':q' to exit): ";
        if (!std::getline(std::cin, query) || query == ":q") {
            break;
        }

        if (query.empty()) continue;

        // 1. Process query
        auto query_stats = process(query, index);

        // 2. Rank documents with TF-IDF
        auto results = TFIDF::rank_documents(query_stats, index.get_total_docs());

        if (results.empty()) {
            std::cout << "No matching documents found.\n";
            continue;
        }

        // 3. Print results with titles and URLs
        std::cout << "\nTop Results:\n";
        int rank = 1;
        for (const auto& res : results) {
            docInfo info = index.get_doc_info(res.doc_id);
            std::cout << "[" << rank++ << "] " << info.title << "\n";
            std::cout << "    Score: " << res.score << "\n";
            std::cout << "    URL:   " << info.url << "\n\n";
        }
    }

    std::cout << "\nGoodbye!\n";
    return 0;
}