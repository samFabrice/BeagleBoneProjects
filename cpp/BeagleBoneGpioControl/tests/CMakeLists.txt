add_executable(gpio_test gpio_test.cpp)
target_link_libraries(gpio_test gpio gtest gtest_main pthread)
target_include_directories(gpio_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(ultrasonic_test ultrasonic_test.cpp)
target_link_libraries(ultrasonic_test gpio ultrasonic gtest GTest::gmock gtest_main pthread)
target_include_directories(ultrasonic_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)