#include "QueryProcessor.hpp"

using namespace std;

unordered_map<string, vector<Posting>> process(const string &query, const InvertedIndex &index) {
    //tokenize query into buzzwords
    Tokenizer myTokenizer;
    vector<string> query_terms = myTokenizer.tokenize(query);

    unordered_map<string, vector<Posting>> query_stats;

    for(const string &word : query_terms) {
        const vector<Posting>* postings_ptr = index.get_postings(word);
        if(postings_ptr != nullptr){
            query_stats[word] = *postings_ptr;
        }
    }

    //send words with their postings to tf-idf
    return query_stats;
}