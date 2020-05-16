CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
PROJECT(test-curl
		LANGUAGES CXX)

#========== Add CPM package manager ================= 
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
set(CPM_VERSION 0.18)

if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
	message(STATUS "Downloading CPM.cmake")
	file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
include(${CPM_DOWNLOAD_LOCATION})
include(ExternalProject)

##========== Add dependency =================
#OPTION(BUILD_OPENSSL "Build OpenSSL from source" OFF)
#message(STATUS "Building OPENSSL FROM SOURCE ${BUILD_OPENSSL}")
#if (BUILD_OPENSSL)
#	set(BUILD_OPENSSL_WORKING_DIR ${CMAKE_BINARY_DIR}/_external/build-openssl)
#	set(BUILD_OPENSSL_SRC_DIR ${CMAKE_SOURCE_DIR}/build-openssl)
#	set(BUILD_OPENSSL_INSTALL_DIR "${BUILD_OPENSSL_WORKING_DIR}/install/OpenSSL")
#	file(MAKE_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR})
#	if(NOT EXISTS ${BUILD_OPENSSL_INSTALL_DIR})
#
#		message(STATUS "Building OpenSSL.. at ${BUILD_OPENSSL_WORKING_DIR}, Install DIR ${BUILD_OPENSSL_INSTALL_DIR}")
#		execute_process(
#				COMMAND ${CMAKE_COMMAND} ${BUILD_OPENSSL_SRC_DIR} -DINSTALL_DIR=${BUILD_OPENSSL_INSTALL_DIR}
#				WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
#		)
#		execute_process(
#				COMMAND ${CMAKE_COMMAND} --build .
#				WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
#		)
#	else()
#		message(STATUS "Found pre-built openSSL at ${BUILD_OPENSSL_INSTALL_DIR}, skipping rebuild")
#	endif()
#
#	set(ENV{OPENSSL_ROOT_DIR} ${BUILD_OPENSSL_INSTALL_DIR})
#	set(OPENSSL_ROOT_DIR ${BUILD_OPENSSL_INSTALL_DIR} CACHE PATH "path to OpenSSL ROOT" FORCE)
#	message(STATUS "openSSL root (env) is $ENV{OPENSSL_ROOT_DIR}")
#	message(STATUS "openSSL root (sys) is ${OPENSSL_ROOT_DIR}")
#    set(OPENSSL_CRYPTO_LIBRARY ${BUILD_OPENSSL_INSTALL_DIR}/lib/libcrypto-1_1_x64.lib CACHE STRING "CRYPTO LIB" FORCE)
#	set(OPENSSL_SSL_LIBRARY ${BUILD_OPENSSL_INSTALL_DIR}/lib/libssl_1_1-x64.lib CACHE STRING "SSL LIB" FORCE)
#	list(APPEND CMAKE_MODULE_PATH "C:/OpenSSL/cmake/")
#else()
#	CPMAddPackage(
#			NAME OpenSSL
#			GITHUB_REPOSITORY janbar/openssl-cmake
#			GIT_TAG master
#			OPTIONS
#			"WITH_APPS OFF"
#	)
#	if (openssl-cmake_ADDED)
#		set(OPENSSL_INCLUDE_DIR "${openssl-cmake_BINARY_DIR}/include")
#		target_include_directories(ssl INTERFACE ${OPENSSL_INCLUDE_DIR})
#		target_include_directories(crypto INTERFACE ${OPENSSL_INCLUDE_DIR})
#        find_package(OpenSSL REQUIRED)
#	endif()
#endif()

set(OPENSSL_ROOT_DIR "C:/Users/justb/Downloads/openssl-1.1.0f-vs2017")
find_package(OpenSSL REQUIRED
		PATHS
		${OPENSSL_ROOT_DIR})
if(OpenSSL_FOUND)
	message(WARNING ${OPENSSL_INCLUDE_DIR})
endif()


CPMAddPackage(
		NAME curl
		VERSION 7.67.0
		URL https://github.com/curl/curl/releases/download/curl-7_67_0/curl-7.67.0.tar.gz
		OPTIONS
		"CMAKE_USE_LIBSSH2 OFF"
		"CURL_STATICLIB ON"
		"BUILD_CURL_EXE OFF"
		"BUILD_SHARED_LIBS OFF"
		"BUILD_TESTING OFF"
		"WITH_STATIC_RT ON"
		"OPENSSL_MSVC_STATIC_RT ON"
		"OPENSSL_ROOT_DIR ${OPENSSL_ROOT_DIR}"
)

#cmake .. -G "Visual Studio 14 2015 Win64" -DBUILD_SHARED_LIBS=OFF -DOPENSSL_USE_STATIC_LIBS=ON -DBUILD_CPR_TESTS=OFF -DCURL_STATICLIB=ON -DOPENSSL_MSVC_STATIC_RT=ON -DBUILD_SHARED_LIBS=OFF -DWITH_STATIC_RT=ON -DOPENSSL_ROOT_DIR=D:\Stacks\openssl-1.0.2n\OpenSSL-X64

add_executable(test-curl main.cpp)
target_link_libraries(test-curl libcurl)
