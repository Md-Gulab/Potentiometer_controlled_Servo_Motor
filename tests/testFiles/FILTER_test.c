#include "unity.h"
#include "filter.h"

static filter_t f;

void setUp(void){
    filter_init(&f);
}

void tearDown(void) {}

void test_first_sample_primes_the_filter(void){
    //No ramp from zero: the first output equals the first input
    TEST_ASSERT_EQUAL_UINT16(1234, filter_update(&f, 1234));
}

void test_constant_input_gives_constant_output(void){
    for (int i = 0; i < 100; i++) {
        TEST_ASSERT_EQUAL_UINT16(2000, filter_update(&f, 2000));
    }
}

void test_step_response_ramps_then_settles(void){
    TEST_ASSERT_EQUAL_UINT16(0, filter_update(&f, 0));      // primes at 0 

    // After k samples of 4000 the window holds k * 4000 (rest zeros)
    for (uint32_t k = 1U; k <= FILTER_LEN; k++) {
        uint32_t expected = (4000U * k + FILTER_LEN / 2U) / FILTER_LEN;
        TEST_ASSERT_EQUAL_UINT16((uint16_t)expected, filter_update(&f, 4000));
    }

    // Window is now full of 4000, so the output has fully settled
    TEST_ASSERT_EQUAL_UINT16(4000, filter_update(&f, 4000));
}

void test_output_stays_within_input_bounds(void){
    uint32_t seed = 12345U;

    for (int i = 0; i < 1000; i++) {
        seed = seed * 1103515245U + 12345U;
        uint16_t sample = (uint16_t)(1000U + ((seed >> 16) % 101U));   // 1000..1100 
        uint16_t out = filter_update(&f, sample);

        TEST_ASSERT_TRUE(out >= 1000U);
        TEST_ASSERT_TRUE(out <= 1100U);
    }
}

void test_filter_reduces_noise(void){
    uint16_t out_min = 65535U;
    uint16_t out_max = 0U;

    for (int i = 0; i < 100; i++) {
        uint16_t sample = (i % 2 == 0) ? 2008U : 1992U;   // +-8 around 2000 
        uint16_t out = filter_update(&f, sample);

        if (i >= (int)(2U * FILTER_LEN)) {                // skip warm-up 
            if (out < out_min) { out_min = out; }
            if (out > out_max) { out_max = out; }
        }
    }

    // Input spread is 16 counts; the output must be calmer than that
    TEST_ASSERT_TRUE((out_max - out_min) < 16U);
}

void test_full_scale_values_do_not_overflow(void){
    for (int i = 0; i < 100; i++) {
        TEST_ASSERT_EQUAL_UINT16(4095, filter_update(&f, 4095));
    }

    for (int i = 0; i < 100; i++) {
        uint16_t out = filter_update(&f, (i % 2 == 0) ? 0U : 4095U);
        TEST_ASSERT_TRUE(out <= 4095U);
    }
}

void test_init_resets_the_state(void){
    for (int i = 0; i < 10; i++) {
        (void)filter_update(&f, 4000);
    }

    filter_init(&f);

    // After re-init the next sample primes the filter again
    TEST_ASSERT_EQUAL_UINT16(100, filter_update(&f, 100));
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_first_sample_primes_the_filter);
    RUN_TEST(test_constant_input_gives_constant_output);
    RUN_TEST(test_step_response_ramps_then_settles);
    RUN_TEST(test_output_stays_within_input_bounds);
    RUN_TEST(test_filter_reduces_noise);
    RUN_TEST(test_full_scale_values_do_not_overflow);
    RUN_TEST(test_init_resets_the_state);
    return UNITY_END();
}