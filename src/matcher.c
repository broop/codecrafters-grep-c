#include <ctype.h>
#include <string.h>
#include "matcher.h"

bool match_next(const char *pattern, const char *input_line);

bool match_character_group(const char *pattern, const char *input_line);

bool match_plus(const char *pattern, const char *input_line);

bool match_question(const char *pattern, const char *input_line);

bool match_alternation(const char *pattern, const char *input_line);

/**
 *
 * @param pattern
 * @param input_line
 * @return
 */
bool match_pattern(const char *pattern, const char *input_line) {
    do {
        if (pattern[0] == '^')
            return match_next(pattern + 1, input_line);
        if (match_next(pattern, input_line)) return true;
    } while (*input_line++ != '\0');

    return false;
}

/**
 *
 * @param pattern
 * @param input_line
 * @return
 */
bool match_next(const char *pattern, const char *input_line) {
    if (pattern[0] == '\0') return true;

    //plus quantifier : one or more
    if ((pattern[1] != '\0' && pattern[1] == '+') ||
        (pattern[0] == '\\' && pattern[2] == '+')) {
        return match_plus(pattern, input_line);
    }
    //question quantifier: zero or one
    if ((pattern[1] != '\0' && pattern[1] == '?') ||
        (pattern[0] == '\\' && pattern[2] == '?')) {
        return match_question(pattern, input_line);
    }

    //digits
    if (pattern[0] == '\\' && pattern[1] == 'd') {
        if (isdigit(*input_line)) {
            return match_next(pattern + 2, input_line + 1);
        }
    }
    //words
    if (pattern[0] == '\\' && pattern[1] == 'w') {
        if (input_line[0] == '_' || isalnum(*input_line)) {
            return match_next(pattern + 2, input_line + 1);
        }
    }
    //character groups
    if (pattern[0] == '[') {
        return match_character_group(pattern, input_line);
    }

    //alternation
    if (pattern[0] == '(') {
        return match_alternation(pattern, input_line);
    }

    //end of string anchor
    if (pattern[0] == '$' && pattern[1] == '\0') {
        return *input_line == '\0';
    }
    if (*input_line != '\0' && (pattern[0] == '.' || pattern[0] == *input_line))
        return match_next(pattern + 1, input_line + 1);

    return false;
}

bool match_plus(const char *pattern, const char *input_line) {
    //get the previous character - check if it's a \d
    bool is_digit = strncmp(pattern, "\\d", 2) == 0;
    const char *found = nullptr;

    //greedy: try to match c as many times as possible - but at least once
    while (input_line[0] != '\0' &&
           (is_digit ? isdigit(input_line[0]) : (pattern[0] == '.' || input_line[0] == pattern[0]))) {
        if (found == nullptr) {
            found = input_line; //found one
        }
        input_line++;
    }

    if (found == nullptr) {
        return false; //bail here
    }
    //backtrack after being greedy until match_next succeeds or 1 match is left
    while (input_line > found) {
        if (match_next(pattern + (is_digit ? 3 : 2), input_line)) {
            return true;
        }
        input_line--;
    }
    return false;
}

bool match_question(const char *pattern, const char *input_line) {
    bool is_digit = strncmp(pattern, "\\d", 2) == 0;

    //try matching one
    if (input_line[0] != '\0') {
        bool matches = is_digit ? isdigit(input_line[0]) : (pattern[0] == '.' || input_line[0] == pattern[0]);
        if (matches && match_next(pattern + (is_digit ? 3 : 2), input_line + 1)) {
            return true;
        }
    }

    //try matching zero
    return match_next(pattern + (is_digit ? 3 : 2), input_line);
}

/**
 *
 * @param pattern
 * @param input_line
 * @return
 */
bool match_alternation(const char *pattern, const char *input_line) {
    const char *start = pattern + 1; //skip the '('
    const char *end = strchr(start, ')'); //find the closing ')'
    if (end == nullptr) return false;
    const char *alt_start = start; //for keeping track of each alternative
    const char *current = start; //pointer

    while (current <= end) {
        if (current == end || *current == '|') {
            //we have ourselves an alternative to check
            size_t alt_len = current - alt_start; //its length
            //check for a match
            if (strncmp(input_line, alt_start, alt_len) == 0) {
                //there's a match so we check if the rest matches, otherwise loop continues with other alternatives
                if (match_next(end + 1, input_line + alt_len)) {
                    return true;
                }
            }
            alt_start = current + 1;
        }
        current++;
    }
    return false;
}

/**
 *
 * @param pattern
 * @param input_line
 * @return
 */
bool match_character_group(const char *pattern, const char *input_line) {
    bool negate = (pattern[1] == '^');
    const char *start = pattern + 1 + negate; //negate converts to 0, 1 here
    const char *end = strchr(start, ']');
    if (end == nullptr) return false;
    // Check for input not empty, char found in string, and found position is before the ending ']'
    bool matched = *input_line != '\0' && (strchr(start, input_line[0]) != nullptr) && (
                       strchr(start, input_line[0]) < end);
    //Change the value of matched if we are to negate
    if (negate) {
        matched = !matched && (input_line[0] != '\0');
    }
    //if it is a match after all that...
    if (matched) {
        return match_next(end + 1, input_line + 1);
    }

    return false;
}
