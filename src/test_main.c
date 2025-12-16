#include <assert.h>
#include <stdio.h>
#include "matcher.h"

static int test_count = 0;
#define TEST(expr) do { assert(expr); test_count++; } while(0)

int main() {
    // Stage: Match a literal character
    TEST(match_pattern("d", "dog") == true);
    TEST(match_pattern("f", "dog") == false);

    // Stage: Match digits
    TEST(match_pattern("\\d", "123") == true);
    TEST(match_pattern("\\d", "apple") == false);
    TEST(match_pattern("\\d", "abc_0_xyz") == true);

    // Stage: Match word characters
    TEST(match_pattern("\\w", "banana") == true);
    TEST(match_pattern("\\w", "RASPBERRY") == true);
    TEST(match_pattern("\\w", "414") == true);

    // Stage: Positive Character Groups
    TEST(match_pattern("[raspberry]", "r") == true);
    TEST(match_pattern("[raspberry]", "rcd") == true);
    TEST(match_pattern("[acdfghijk]", "blueberry") == false);
    TEST(match_pattern("[pear]", "[]") == false);

    // Stage: Negative Character Groups
    TEST(match_pattern("[^xyz]", "apple") == true);
    TEST(match_pattern("[^abc]", "apple") == true);
    TEST(match_pattern("[^anb]", "banana") == false);
    TEST(match_pattern("[^opq]", "orange") == true);

    // Stage: Combining Character Classes
    TEST(match_pattern("\\d apple", "sally has 3 apples") == true);
    TEST(match_pattern("\\d apple", "sally has 1 orange") == false);
    TEST(match_pattern("\\d\\d\\d apples", "sally has 124 apples") == true);
    TEST(match_pattern("\\d \\w\\w\\ws", "sally has 3 dogs") == true);
    TEST(match_pattern("\\d \\w\\w\\ws", "sally has 4 dogs") == true);
    TEST(match_pattern("\\d \\w\\w\\ws", "sally has 1 dog") == false);

    // Stage: Start of string anchor
    TEST(match_pattern("^grape", "grape_raspberry") == true);
    TEST(match_pattern("^grape", "raspberry_grape") == false);

    // Stage: End of string anchor
    TEST(match_pattern("blueberry$", "banana_blueberry") == true);
    TEST(match_pattern("blueberry$", "blueberry_banana") == false);
    TEST(match_pattern("^orange$", "orange") == true);
    TEST(match_pattern("^orange$", "orange_orange") == false);

    // Stage: Match one or more times
    TEST(match_pattern("ca+t", "cat") == true);
    TEST(match_pattern("ca+at", "caaats") == true);
    TEST(match_pattern("ca+t", "act") == false);
    TEST(match_pattern("ca+t", "ca") == false);
    TEST(match_pattern("^abc_\\d+_xyz$", "abc_123_xyz") == true);
    TEST(match_pattern("^abc_\\d+_xyz$", "abc_rst_xyz") == false);

    // Stage: Match zero or one times
    TEST(match_pattern("ca?t", "cat") == true);
    TEST(match_pattern("ca?t", "act") == true);
    TEST(match_pattern("ca?a?t", "cat") == true);
    TEST(match_pattern("ca?t", "caat") == false);
    TEST(match_pattern("ca?t", "cag") == false);

    // Stage: Wildcard
    TEST(match_pattern("c.t", "cat") == true);
    TEST(match_pattern("c.t", "car") == false);
    TEST(match_pattern("g.+gol", "gol") == false);

    // Stage: Alternation
    TEST(match_pattern("a (cat|dog)", "a cat") == true);
    TEST(match_pattern("a (cat|dog)", "a cog") == false);
    TEST(match_pattern("^I see \\d+ (cat|dog)s?$", "I see 1 cat") == true);
    TEST(match_pattern("^I see \\d+ (cat|dog)s?$", "I see 42 dogs") == true);
    TEST(match_pattern("^I see \\d+ (cat|dog)s?$", "I see a cat") == false);
    TEST(match_pattern("^I see \\d+ (cat|dog)s?$", "I see 2 dog3") == false);

    printf("All %d tests passed!\n", test_count);
    return 0;
}
