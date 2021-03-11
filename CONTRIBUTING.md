### Getting Started & Building:

1. Prerequisites:
    * CMake version 3.18.0+ -> Ubuntu systems seem to have a problem getting the latest version, please build from
      sources or use another package such as snap.
        * [Ubuntu CMake update instructions](https://apt.kitware.com/)
    * Ninja
2. Checkout the repository:
    ```sh
    git clone https://github.com/codereport/An-Algorithm-Library.git
    ```

#### Linux or Windows w/ Clang

1. Run cmake:
    ```sh
    cd An-Algorithm-Library
    mkdir build
    cmake -G "Ninja Multi-Config" -B build
    ```
2. Run tests:
    ```sh
    ninja -C build test
    ```
    * For more verbose output:

    ```sh
    # On linux you can combine these lines with &&
    ninja -C build
    cd build 
    ctest -C debug --verbose 
    cd ..
    ```

#### Windows MSVC

1. Run cmake:
    ```sh
    cd An-Algorithm-Library
    mkdir build
    cmake -G "Visual Studio 16 2019" -B build
    ```
2. Run tests:
    * For more verbose output:
    ```sh
    MSBuild build\ALL_BUILD.vcxproj 
    cd build 
    ctest -C debug --verbose 
    cd ..
    ```