//generates list of tf-idf
#include "tf-idf.hpp"
#include <cmath>
#include <algorithm>

namespace TFIDF {

std::vector<SearchResult> rank_documents(const std::unordered_map<std::string, std::vector<Posting>>& query_stats, int total_documents) {
    //keeping track of doc_id --> doc_score
    std::unordered_map<int, double> doc_scores;
    
    //loop through every (word, vector_of_postings) pair in the map. 
    for (const auto& [term, postings] : query_stats) {
        //skip if search didn't match any documents
        if (postings.empty()) continue;

        //calculate idf score 
        double idf = std::log(1.0 + (static_cast<double>(total_documents) / postings.size()));

        //loop through every posting for this word 
        for (const auto& posting : postings) {
            //calculate tf
            double tf = static_cast<double>(posting.term_frequency);

            //add score into running total
            doc_scores[posting.doc_id] += (tf * idf);
        }
    }


    //create a vector in order to sort by relevance
    std::vector<SearchResult> result;
    for (const auto& [doc_id, score] : doc_scores) {
        result.push_back({doc_id, score});
    }

    //sort postings descending order
    std::sort(result.begin(), result.end(), [](const SearchResult &a, const SearchResult &b) 
    { return a.score > b.score; });


    return result;
}
}

