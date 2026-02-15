add_executable(run_tests)

target_sources(run_tests PRIVATE
    test_rational.cpp
    test_monomial.cpp
    test_term.cpp
    test_orderings.cpp
    test_polynomial.cpp
    test_algo.cpp
)

target_include_directories(run_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(run_tests PRIVATE
    GTest::gtest_main
    groebner_basis_lib
)