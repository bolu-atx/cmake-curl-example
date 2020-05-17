CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
set(CMAKE_VERBOSE_MAKEFILE ON)

PROJECT(test-curl
		LANGUAGES CXX)

# Prefer static libs over shared lib
LIST(REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
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
# Get all propreties that cmake supports
execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)

# Convert command output into a CMake list
STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
# Fix https://stackoverflow.com/questions/32197663/how-can-i-remove-the-the-location-property-may-not-be-read-from-target-error-i
list(FILTER CMAKE_PROPERTY_LIST EXCLUDE REGEX "^LOCATION$|^LOCATION_|_LOCATION$")
# For some reason, "TYPE" shows up twice - others might too?
list(REMOVE_DUPLICATES CMAKE_PROPERTY_LIST)

OPTION(ENABLE_OPENSSL_FROM_SOURCE "BUILD OPENSSL FROM SOURCE AT CONFIGURE TIME" ON)

if(ENABLE_OPENSSL_FROM_SOURCE)
	set(BUILD_OPENSSL_WORKING_DIR ${CMAKE_BINARY_DIR}/_external/build-openssl)
	set(BUILD_OPENSSL_SRC_DIR ${CMAKE_SOURCE_DIR}/build-openssl)
	set(BUILD_OPENSSL_INSTALL_DIR "${BUILD_OPENSSL_WORKING_DIR}/install/OpenSSL")
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
		message(STATUS "${BUILD_OPENSSL_INSTALL_DIR} already exists, skips rebuilding..")
	endif()

	set(OPENSSL_ROOT_DIR "${BUILD_OPENSSL_INSTALL_DIR}" CACHE INTERNAL "OPENSSL ROOT DIR")
	message(MESSAGE "Setting OPENSSL ROOT DIR to newly built OpenSSL ${OPENSSL_ROOT_DIR}")
else()
	# set(OPENSSL_LIBRARIES "/usr/local/opt/openssl/lib" CACHE INTERNAL "OPENSSL lib dir")
endif()

if (MSVC)
	set(CURL_PLATFORM_OPTIONS "CMAKE_USE_WINSSL ON;CMAKE_USE_OPENSSL OFF")
elseif(APPLE)
	# set(OPENSSL_LIBRARIES "/usr/local/opt/openssl/lib" CACHE INTERNAL "OPENSSL lib dir")
	# set(ENV{OPENSSL_ROOT_DIR} "/usr/local/opt/openssl")
	# find_library(OPENSSL_CRYPTO_LIB
	# 	NAME libcrypto.a crypto.a
	# 	HINTS ${OPENSSL_ROOT_DIR}
	# 	PATH_SUFFIXES
	# 	lib
	# 	NO_DEFAULT_PATH)
	# find_library(OPENSSL_SSL_LIB
	# 	NAME ssl.a libssl.a
	# 	HINTS ${OPENSSL_ROOT_DIR}
	# 	PATH_SUFFIXES
	# 	lib
	# 	NO_DEFAULT_PATH)
	# set(OPENSSL_LIBS ${OPENSSL_SSL_LIB} ${OPENSSL_CRYPTO_LIB})
	# message(STATUS "OPENSSL LIB DIR ${OPENSSL_LIBRARIES}, INCLUDE DIR ${OPENSSL_INCLUDE_DIR}, LIBs ${OPENSSL_LIBS}")
	# add_library(libssl INTERFACE)
	# add_library(libcrypto INTERFACE)
	# target_link_libraries(libcrypto INTERFACE ${OPENSSL_CRYPTO_LIB})
	# target_link_libraries(libssl INTERFACE ${OPENSSL_SSL_LIB})
	# target_include_directories(libssl INTERFACE ${OPENSSL_INCLUDE_DIR})
	# target_include_directories(libcrypto INTERFACE ${OPENSSL_INCLUDE_DIR})
	set(CURL_PLATFORM_OPTIONS "CMAKE_USE_SECTRANSP OFF;CMAKE_USE_OPENSSL ON")
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
