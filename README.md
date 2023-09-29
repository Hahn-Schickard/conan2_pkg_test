# Conan2 Dependency repackage example

This repo showcases cpack failure when consuming conan v2 packages with transient dependencies.

All dependencies are built as shared libs.

`subproject` directory contains a custom conan v2 example package to reproduce the problem.

This project also allows testing this same problem on conan v1 systems, however `subproject` package can not be built because `test_package` is failing. For some reason, `libEXAMPLE_REST_Utility.so` can not be found, even thought it is linked in. You can see this behavior by running the following: 

```bash
docker build -t conan_v1_create_failure subproject/
docker run conan_v1_create_failure
```

## Failure reproduction

1. Create sub-project package with: 

```bash
conan create subproject/ --version=0.0.0 --build=missing
```

2. Create build directory and change into it with:

```bash
mkdir build && cd build
```

3. Call conan install command and generate CMake build scripts. (Cmake internally uses [conan_provider.cmake](cmake/conan_provider.cmake) to read [conanfile.txt](conanfile.txt), which tells conan what packages to download and install)

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DWITH_NANODBC=OFF -DWITH_CURL=OFF -DWITH_SUBPROJECT=ON ..
```

4. Build the code

```bash
cmake --build . --target all --config Debug --
```

5. Run the executable to make sure it works

```bash
./sources/conan2_pkg_test_Runner 
```

6. Try to create a simple package

```bash
cpack -G TGZ
```

On debian bookworm this fails to find `cassandra-cpp-driver` package from the **subproject**:

```bash
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: conan2_pkg_test
CPack: - Install project: conan2_pkg_test []
CMake Error at /conan2_pkg_test/build/sources/cmake_install.cmake:66 (file):
  file Could not resolve runtime dependencies:

    libcassandra.so.2
Call Stack (most recent call first):
  /conan2_pkg_test/build/cmake_install.cmake:47 (include)


CPack Error: Error when generating package: conan2_pkg_test
```

Same behavior can be seen when using `nanodbc` package. Clean the build directory with:

```bash
rm -rf build/* && cd build
```

and reconfigure the project to only use `nanodbc` package with:

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DWITH_NANODBC=ON -DWITH_CURL=OFF -DWITH_SUBPROJECT=OFF ..
```

Then re-run the steps 4-6 and get the following:

```bash
Pack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: conan2_pkg_test
CPack: - Install project: conan2_pkg_test []
CMake Error at /conan2_pkg_test/build/sources/cmake_install.cmake:66 (file):
  file Could not resolve runtime dependencies:

    libiconv.so.2
    libltdl.so.7
Call Stack (most recent call first):
  /conan2_pkg_test/build/cmake_install.cmake:47 (include)


CPack Error: Error when generating package: conan2_pkg_test
```

This is really similar to how **subproject** transient dependencies fail to be resolved by `cmake_install.cmake`. [`nanodbc`](https://conan.io/center/recipes/nanodbc?version=cci.20200807) package depends on [`odbc`](https://conan.io/center/recipes/odbc?version=2.3.11) package, which in turn depends on [`libtool`](https://conan.io/center/recipes/libtool?version=2.4.7) and [`libiconv`](https://conan.io/center/recipes/libiconv?version=1.17) packages. `libiconv.so` comes from [`libiconv`](https://conan.io/center/recipes/libiconv?version=1.17) package and `libltdl.so` comes from [`libtool`](https://conan.io/center/recipes/libtool?version=2.4.7) package.

Even stranger things happen when we try to package projects with `libcurl` dependency. Similar to as before  Clean the build directory with:

```bash
rm -rf build/* cd build
```

and reconfigure the project to only use `libcurl` package with:

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DWITH_NANODBC=OFF -DWITH_CURL=ON -DWITH_SUBPROJECT=OFF ..
```

Then re-run the steps 4-6 and get the following:

```bash
CPack: Create package using TGZ
CPack: Install projects
CPack: - Run preinstall target for: conan2_pkg_test
CPack: - Install project: conan2_pkg_test []
CMake Error at /conan2_pkg_test/build/sources/cmake_install.cmake:66 (file):
  file Multiple conflicting paths found for libcrypto.so.3:

    /lib/x86_64-linux-gnu/libcrypto.so.3
    /root/.conan2/p/b/opens6b2fbd1f2b289/p/lib/libcrypto.so.3
Call Stack (most recent call first):
  /conan2_pkg_test/build/cmake_install.cmake:47 (include)


CPack Error: Error when generating package: conan2_pkg_test
```

For some reason, cmake sees both the `libcrypto.so.3` that comes from [`openssl`](https://conan.io/center/recipes/openssl?version=3.1.2) package and the one that comes from the system.

## Reproducing with Docker

Build docker images with 

```bash
docker build -t conan_example --build-arg CONAN_VERSION=2.0.10 --build-arg USE_NANODBC=ON --build-arg USE_CURL=ON --build-arg USE_SUBPROJECT=ON .
```

or to use provided defaults (conan v2.0.10, nanodbc off, curl off, subproject ON)

```bash
docker build -t conan_example .
```

And run it with: 

```bash
docker run conan_example 
```

or run it in interactive mode with: 

```bash
docker run -it conan_example bash 
```
