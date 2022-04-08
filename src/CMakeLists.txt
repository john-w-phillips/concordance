target_sources(code_challenge PRIVATE Tokenizer.cpp)
add_executable(concordance concordance.cpp)
target_link_libraries(concordance code_challenge)
