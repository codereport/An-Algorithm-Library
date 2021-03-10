### Getting Started & Building:
1. Prerequisites:
   * CMake version 3.17.0+ -> Ubuntu systems seem to have a problem getting the latest version, please build from sources or use another package such as snap. 
     * [Ubuntu CMake update instructions](https://apt.kitware.com/)
   * Ninja
2. Checkout the repository:
    ```sh
    git clone https://github.com/codereport/An-Algorithm-Library.git
    ```
3. Run cmake:
    ```sh
    cd An-Algorithm-Library
    mkdir build
    cmake -G "Ninja Multi-Config" -B build
    ```
4. Run tests:
    ```sh
    ninja -C build test
    ```
   * For more verbose output:

    ```sh
    ninja -C build && cd build && ctest -C debug --verbose && cd ..
    ```
   