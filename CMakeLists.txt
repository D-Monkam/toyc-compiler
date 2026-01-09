cmake_minimum_required(VERSION 3.10)
project(toyc)

# 1. Find the LLVM package
find_package(LLVM REQUIRED CONFIG)

# 2. Add LLVM definitions/includes
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# 3. Define your source files
set(SOURCES src/main.cpp)

# 4. Create the executable
add_executable(toyc ${SOURCES})

# 5. Link against specific LLVM libraries
# core: Basic IR generation
# native: For targeting the host machine
llvm_map_components_to_libnames(llvm_libs core support native)
target_link_libraries(toyc ${llvm_libs})