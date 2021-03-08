### Getting Started & Building:
1. Prerequisites:
   * CMake version 3.17.0+ -> Ubuntu systems seem to have a problem getting the latest version, please build from sources or use another package such as snap. 
     * [Ubuntu CMake update instructions](https://apt.kitware.com/)
   * Ninja
2. Checkout the repository:
    ```sh
    git clone https://github.com/codereport/An-Algorithm-Library.git
    ```
3. Build An Algorithm Library:    
   * Run cmake:
    ```sh
    cd An-Algorithm-Library
    mkdir build
    cmake -G "Ninja Multi-Config" -B build
    ```
   * To build debug:
    ```sh
    ninja -C build 
    ```
   * To build release:
    ```sh
    ninja -C build -f build-Release.ninja
    ```
4. Run tests:
    ```sh
    ninja -C build test
    ```