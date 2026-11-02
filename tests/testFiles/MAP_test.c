#include "unity.h"
#include "map.h"

//Same numbers as the real project: ADC window -> servo pulse in microseconds
#define IN_MIN   10
#define IN_MAX   4085
#define OUT_MIN  1000
#define OUT_MAX  2000

void setUp(void) {}
void tearDown(void) {}

void test_low_end_maps_to_out_min(void){
    TEST_ASSERT_EQUAL_INT32(OUT_MIN, map_range(IN_MIN, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX));
}

void test_high_end_maps_to_out_max(void){
    TEST_ASSERT_EQUAL_INT32(OUT_MAX, map_range(IN_MAX, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX));
}

void test_below_range_is_clamped_to_out_min(void){
    TEST_ASSERT_EQUAL_INT32(OUT_MIN, map_range(0, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX));
}

void test_above_range_is_clamped_to_out_max(void){
    TEST_ASSERT_EQUAL_INT32(OUT_MAX, map_range(4095, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX));
}

void test_middle_maps_to_centre(void){
    TEST_ASSERT_EQUAL_INT32(1500, map_range(2048, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX));
}

void test_reversed_output_range(void){
    TEST_ASSERT_EQUAL_INT32(2000, map_range(IN_MIN, IN_MIN, IN_MAX, 2000, 1000));
    TEST_ASSERT_EQUAL_INT32(1000, map_range(IN_MAX, IN_MIN, IN_MAX, 2000, 1000));
}

void test_reversed_input_range(void){
    TEST_ASSERT_EQUAL_INT32(0,   map_range(100, 100, 0, 0, 100));
    TEST_ASSERT_EQUAL_INT32(100, map_range(0,   100, 0, 0, 100));
}

void test_degenerate_input_range_returns_out_min(void){
    TEST_ASSERT_EQUAL_INT32(OUT_MIN, map_range(5, 7, 7, OUT_MIN, OUT_MAX));
}

void test_output_is_monotonic_and_in_range(void){
    int32_t prev = map_range(0, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
    for (int32_t x = 1; x <= 4095; x++) {
        int32_t y = map_range(x, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX);
        TEST_ASSERT_TRUE(y >= prev);
        TEST_ASSERT_TRUE(y >= OUT_MIN && y <= OUT_MAX);
        prev = y;
    }
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_low_end_maps_to_out_min);
    RUN_TEST(test_high_end_maps_to_out_max);
    RUN_TEST(test_below_range_is_clamped_to_out_min);
    RUN_TEST(test_above_range_is_clamped_to_out_max);
    RUN_TEST(test_middle_maps_to_centre);
    RUN_TEST(test_reversed_output_range);
    RUN_TEST(test_reversed_input_range);
    RUN_TEST(test_degenerate_input_range_returns_out_min);
    RUN_TEST(test_output_is_monotonic_and_in_range);
    return UNITY_END();
}