#include <iostream>

// -----------------------------------------------------------------------------
// ToyC Interop Example
// -----------------------------------------------------------------------------
//
// 1. Run the compiler: ./toyc
// 2. Type this code into the prompt:
//    def average(x y) (x + y) * 5
// 3. Press Ctrl+D to exit. This generates 'output.o'
// 4. Compile this runner: clang++ examples/runner.cpp output.o -o my_program
// 5. Run it: ./my_program
// -----------------------------------------------------------------------------

// Declare the function signature that exists in our object file.
// 'extern "C"' prevents C++ name mangling, allowing us to link to the symbol.
extern "C" {
    int average(int x, int y);
}

int main() {
    std::cout << "Running ToyC Interop Test..." << std::endl;

    int x = 10;
    int y = 20;

    // Call the function compiled by ToyC
    int result = average(x, y); 

    std::cout << "Calculating: (" << x << " + " << y << ") * 5" << std::endl;
    std::cout << "Result from ToyC binary: " << result << std::endl;
    
    // Expected output: 150
    if (result == 150) {
        std::cout << "SUCCESS: Integration working." << std::endl;
    } else {
        std::cout << "FAILURE: Incorrect result." << std::endl;
    }

    return 0;
}