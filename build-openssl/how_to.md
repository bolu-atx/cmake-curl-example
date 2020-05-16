# build-openssl

This Cmake project is a shell that allows you to build OpenSSL at configure time

## How to use

Insert the following snippet in the project that you wish to have the OpenSSL dependency build
which can be turned off via `-DBUILD_OPENSSL=OFF` in case we want to prefer the system OpenSSL library

```cmake
# Configure time OpenSSL
OPTION(BUILD_OPENSSL "Build OpenSSL from source" ON)
message(STATUS "Building OPENSSL FROM SOURCE ${BUILD_OPENSSL}")
if (BUILD_OPENSSL)
    set(BUILD_OPENSSL_WORKING_DIR ${CMAKE_BINARY_DIR}/_external/build-openssl)
    set(BUILD_OPENSSL_SRC_DIR ${CMAKE_SOURCE_DIR}/external/build-openssl)
    set(BUILD_OPENSSL_INSTALL_DIR "${BUILD_OPENSSL_WORKING_DIR}/install-openssl")
    file(MAKE_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR})
    if(NOT EXISTS ${BUILD_OPENSSL_INSTALL_DIR})
        message(STATUS "Building OpenSSL.. at ${BUILD_OPENSSL_WORKING_DIR}, Install DIR ${BUILD_OPENSSL_INSTALL_DIR}")
        execute_process(
                COMMAND ${CMAKE_COMMAND} ${BUILD_OPENSSL_SRC_DIR} -DINSTALL_DIR=${BUILD_OPENSSL_INSTALL_DIR}
                WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
        )
        execute_process(
                COMMAND ${CMAKE_COMMAND} --build .
                WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
        )
    else()
        message(STATUS "Found pre-built openSSL at ${BUILD_OPENSSL_INSTALL_DIR}, skipping rebuild")
    endif()

    set(OPENSSL_INCLUDE_DIR ${BUILD_OPENSSL_INSTALL_DIR}/include)
    add_library(openssl INTERFACE IMPORTED)
    target_include_directories(openssl INTERFACE ${OPENSSL_INCLUDE_DIR})
    target_link_libraries(openssl INTERFACE ${BUILD_OPENSSL_INSTALL_DIR}/lib/libssl.a ${BUILD_OPENSSL_INSTALL_DIR}/lib/libcrypto.a)
else()
    find_package(OpenSSL REQUIRED)
    SET(curl_PLATFORM_OPT "CMAKE_USE_OPENSSL ON")
endif()
```
