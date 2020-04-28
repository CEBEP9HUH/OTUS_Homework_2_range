cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
    project(ip_filter VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(ip_filter VERSION 0.0.42)
endif()

include(${CMAKE_BINARY_DIR}/conan_files/conanbuildinfo.cmake)
conan_basic_setup()

configure_file(description.json.in description.json)

add_executable(ip_filter ip_filter.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(ip_filter PRIVATE
    ${CONAN_LIBS}
)

if(MSVC)
    target_compile_options(ip_filter PRIVATE
        /W4
    )
else()
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -Werror -pedantic
    )
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)