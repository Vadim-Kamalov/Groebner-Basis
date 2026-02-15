add_library(groebner_basis_lib STATIC)

target_sources(groebner_basis_lib PRIVATE
    rational.cpp
    monomial.cpp
    term.cpp
    orderings.cpp
    algo_utils.cpp
)

target_include_directories(groebner_basis_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
