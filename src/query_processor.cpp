#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <unordered_set>
#include "Tokenizer.hpp"
#include "InvertedIndex.hpp"
#include <map>

using namespace std;

vector<string> process(const string &query, const InvertedIndex &index) {
    //tokenize query into buzzwords
    Tokenizer myTokenizer;
    vector<string> query_terms = myTokenizer.tokenize(query);

    map<string, vector<Posting>> query_stats;

    for(const string &word : query_terms) {
        const vector<Posting>* postings_ptr = index.get_postings(word);
        if(postings_ptr != nullptr){
            query_stats[word] = *postings_ptr;
        }
    }

    //send words with their postings to tf-idf

}