#include <vector>
#include <unordered_map>
#include <string>
#include "InvertedIndex.hpp"

struct SearchResult {
    int doc_id;
    double score; 
};

namespace TFIDF {
std::vector<SearchResult> rank_documents(const std::unordered_map<std::string, std::vector<Posting>>& query_stats, int total_documents); }