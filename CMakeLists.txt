cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project("StdBot")

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

add_subdirectory("libs/json")
add_subdirectory("libs/spdlog")

add_executable(StdBot "src/StdBot.cpp")
add_executable(TestStdBot "test/Test.cpp")

target_include_directories(StdBot PRIVATE
	SYSTEM "libs/json/include"
	SYSTEM "libs/spdlog/include"
	SYSTEM "libs/httplib"
)

target_include_directories(TestStdBot PRIVATE
	SYSTEM "libs/json/include"
	SYSTEM "libs/spdlog/include"
	SYSTEM "libs/httplib"
	SYSTEM "libs/catch2"
)

target_link_libraries(StdBot PUBLIC
	spdlog::spdlog
	OpenSSL::SSL
	OpenSSL::Crypto
	${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(TestStdBot PUBLIC
	spdlog::spdlog
	OpenSSL::SSL
	OpenSSL::Crypto
	${CMAKE_THREAD_LIBS_INIT}
)

set(MSVC_WARNINGS
	/W4 # Baseline reasonable warnings
	/w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss
			# of data
	/w14254 # 'operator': conversion from 'type1:field_bits' to
			# 'type2:field_bits', possible loss of data
	/w14263 # 'function': member function does not override any base class
			# virtual member function
	/w14265 # 'classname': class has virtual functions, but destructor is not
			# virtual instances of this class may not be destructed correctly
	/w14287 # 'operator': unsigned/negative constant mismatch
	/we4289 # nonstandard extension used: 'variable': loop control variable
			# declared in the for-loop is used outside the for-loop scope
	/w14296 # 'operator': expression is always 'boolean_value'
	/w14311 # 'variable': pointer truncation from 'type1' to 'type2'
	/w14545 # expression before comma evaluates to a function which is missing
			# an argument list
	/w14546 # function call before comma missing argument list
	/w14547 # 'operator': operator before comma has no effect; expected
			# operator with side-effect
	/w14549 # 'operator': operator before comma has no effect; did you intend
			# 'operator'?
	/w14555 # expression has no effect; expected expression with side- effect
	/w14619 # pragma warning: there is no warning number 'number'
	/w14640 # Enable warning on thread un-safe static member initialization
	/w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may
			# cause unexpected runtime behavior.
	/w14905 # wide string literal cast to 'LPSTR'
	/w14906 # string literal cast to 'LPWSTR'
	/w14928 # illegal copy-initialization; more than one user-defined
			# conversion has been implicitly applied
	/permissive- # standards conformance mode for MSVC compiler.
	/DCPPHTTPLIB_OPENSSL_SUPPORT

	/O0
	/g3
	/ggdb
	/MP

)

if(WIN32)
	target_compile_options(StdBot PUBLIC ${MSVC_WARNINGS})
	target_compile_options(TestStdBot PUBLIC ${MSVC_WARNINGS})
else()
	target_compile_options(StdBot PUBLIC
		-O3
		-DCPPHTTPLIB_OPENSSL_SUPPORT
	)

	target_compile_options(TestStdBot PUBLIC
		-DCPPHTTPLIB_OPENSSL_SUPPORT
	)
endif()

target_include_directories(StdBot PUBLIC
	${CMAKE_SOURCE_DIR}/libs
	${CMAKE_SOURCE_DIR} 
)

target_include_directories(TestStdBot PUBLIC
	${CMAKE_SOURCE_DIR}/libs
	${CMAKE_SOURCE_DIR} 
)