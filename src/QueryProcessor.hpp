#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <unordered_set>
#include "Tokenizer.hpp"
#include "InvertedIndex.hpp"
#include <unordered_map>
#include <map>

std::unordered_map<std::string, std::vector<Posting>> process(
    const std::string &query, 
    const InvertedIndex &index
);