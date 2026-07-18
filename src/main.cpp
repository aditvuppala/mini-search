#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "=========================================" << std::endl;
    std::cout << "  MiniSearch Linux Core Active via WSL   " << std::endl;
    std::cout << "=========================================" << std::endl;
    
    if (argc > 1) {
        std::cout << "Active Query Parameter: " << argv[1] << std::endl;
    } else {
        std::cout << "No runtime parameters provided." << std::endl;
    }
    
    return 0;
}