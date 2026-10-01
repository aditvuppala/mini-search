#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <numeric>
#include "src/nlohmann/json.hpp"
#include "src/InvertedIndex.hpp"
#include "src/QueryProcessor.hpp"
#include "src/tf-idf.hpp"
#include "src/Tokenizer.hpp"

#ifdef __APPLE__
#include <mach/mach.h>
#elif defined(__linux__)
#include <sys/resource.h>
#endif

using json = nlohmann::json;
using namespace std::chrono;

double get_memory_usage_mb() {
#ifdef __APPLE__
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
        return static_cast<double>(info.resident_size) / (1024.0 * 1024.0);
    }
#elif defined(__linux__)
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        return static_cast<double>(usage.ru_maxrss) / 1024.0;
    }
#endif
    return 0.0;
}

int main() {
    std::cout << "\n====================================================\n";
    std::cout << "      SEARCH ENGINE PERFORMANCE BENCHMARK           \n";
    std::cout << "====================================================\n\n";

    std::ifstream file("scraped_pages.json");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open scraped_pages.json!\n";
        return 1;
    }

    json data;
    file >> data;

    InvertedIndex index;
    Tokenizer tokenizer;

    // --- Benchmark 1: Ingestion & Indexing ---
    std::cout << "[1] Benchmarking Ingestion & Indexing...\n";
    auto index_start = high_resolution_clock::now();

    size_t total_tokens_indexed = 0;
    for (const auto& doc : data) {
        int doc_id = doc["doc_id"].get<int>();
        std::string title = doc["title"].get<std::string>();
        std::string url = doc["url"].get<std::string>();
        std::string content = doc["content"].get<std::string>();

        std::vector<std::string> tokens = tokenizer.tokenize(content);
        total_tokens_indexed += tokens.size();
        index.add_document(doc_id, title, url, tokens);
    }

    auto index_end = high_resolution_clock::now();
    double index_time_ms = duration_cast<microseconds>(index_end - index_start).count() / 1000.0;
    double mem_used = get_memory_usage_mb();

    std::cout << "    - Documents Indexed: " << index.get_total_docs() << "\n";
    std::cout << "    - Tokens Ingested:   " << total_tokens_indexed << "\n";
    std::cout << "    - Ingestion Time:    " << index_time_ms << " ms\n";
    std::cout << "    - Throughput:        " << static_cast<int>(index.get_total_docs() / (index_time_ms / 1000.0)) << " docs/sec\n";
    if (mem_used > 0.0) {
        std::cout << "    - Resident Memory:   " << mem_used << " MB\n";
    }
    std::cout << "\n";

    // --- Benchmark 2: Query Processing & Ranking ---
    std::vector<std::string> test_queries = {
        "travel",
        "mystery historical",
        "fiction books art",
        "young adult adventure",
        "nonexistenttermxyz"
    };

    std::cout << "[2] Benchmarking Query Latency (1,000 runs per query)...\n";
    std::vector<double> all_latencies_us;

    for (const auto& q : test_queries) {
        std::vector<double> latencies;
        int trials = 1000;

        // Warmup: run a few untimed passes first so cold caches/branch
        // prediction don't skew the very first timed trials.
        for (int i = 0; i < 10; ++i) {
            auto stats = process(q, index);
            TFIDF::rank_documents(stats, index.get_total_docs());
        }

        for (int i = 0; i < trials; ++i) {
            auto q_start = high_resolution_clock::now();
            auto stats = process(q, index);
            auto results = TFIDF::rank_documents(stats, index.get_total_docs());
            auto q_end = high_resolution_clock::now();

            double duration_us = duration_cast<microseconds>(q_end - q_start).count();
            latencies.push_back(duration_us);
            all_latencies_us.push_back(duration_us);
        }

        std::sort(latencies.begin(), latencies.end());
        double avg = std::accumulate(latencies.begin(), latencies.end(), 0.0) / trials;
        double p99 = latencies[static_cast<size_t>(trials * 0.99)];

        std::cout << "    Query: \"" << q << "\" -> Avg: " << avg << " us | P99: " << p99 << " us\n";
    }

    std::sort(all_latencies_us.begin(), all_latencies_us.end());
    double total_avg = std::accumulate(all_latencies_us.begin(), all_latencies_us.end(), 0.0) / all_latencies_us.size();
    double overall_p99 = all_latencies_us[static_cast<size_t>(all_latencies_us.size() * 0.99)];

    std::cout << "\n----------------------------------------------------\n";
    std::cout << "OVERALL LATENCY SUMMARY:\n";
    std::cout << "  Average Query Latency: " << total_avg << " us (" << (total_avg / 1000.0) << " ms)\n";
    std::cout << "  P99 Latency:           " << overall_p99 << " us (" << (overall_p99 / 1000.0) << " ms)\n";
    std::cout << "====================================================\n\n";

    return 0;
}