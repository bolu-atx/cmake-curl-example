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
#set(BUILD_OPENSSL_WORKING_DIR ${CMAKE_BINARY_DIR}/_external/build-openssl)
#set(BUILD_OPENSSL_SRC_DIR ${CMAKE_SOURCE_DIR}/build-openssl)
#set(BUILD_OPENSSL_INSTALL_DIR "${BUILD_OPENSSL_WORKING_DIR}/install/OpenSSL")
#file(MAKE_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR})
#if(NOT EXISTS ${BUILD_OPENSSL_INSTALL_DIR})
#
#	message(STATUS "Building OpenSSL.. at ${BUILD_OPENSSL_WORKING_DIR}, Install DIR ${BUILD_OPENSSL_INSTALL_DIR}")
#	execute_process(
#			COMMAND ${CMAKE_COMMAND} ${BUILD_OPENSSL_SRC_DIR} -DINSTALL_DIR=${BUILD_OPENSSL_INSTALL_DIR}
#			WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
#	)
#	execute_process(
#			COMMAND ${CMAKE_COMMAND} --build .
#			WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
#	)
#else()
#	message(STATUS "${BUILD_OPENSSL_INSTALL_DIR} already exists, skips rebuilding..")
#endif()
#
#set(OPENSSL_ROOT_DIR "${BUILD_OPENSSL_ROOT_DIR}/OpenSSL")

if (MSVC)
	set(CURL_PLATFORM_OPTIONS "CMAKE_USE_WINSSL ON;CMAKE_USE_OPENSSL OFF")
elseif(APPLE)
	set(CURL_PLATFORM_OPTIONS "CMAKE_USE_SECTRANSP ON;CMAKE_USE_OPENSSL OFF")
else()
	set(CURL_PLATFORM_OPTIONS "CMAKE_USE_OPENSSL ON")
endif()

CPMAddPackage(
		NAME curl
		VERSION 7.67.0
		URL https://github.com/curl/curl/releases/download/curl-7_67_0/curl-7.67.0.tar.gz
		OPTIONS
		"CURL_STATICLIB ON"
		"BUILD_CURL_EXE OFF"
		"BUILD_SHARED_LIBS OFF"
		"BUILD_TESTING OFF"
		"WITH_STATIC_RT ON"
		"CMAKE_USE_LIBSSH2 OFF"
        ${CURL_PLATFORM_OPTIONS}
)

add_executable(test-curl main.cpp)
target_link_libraries(test-curl libcurl)
