[![progress-banner](https://backend.codecrafters.io/progress/grep/816c87b4-3a74-4958-bbc2-90d9e3f0c353)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

# Regex Matcher in C

My implementation of the core regex functionality.

See my Python and Java versions for extensions.

Based on the excellent [Rob Pike code](https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html).

Here is that code which I used to build upon not only this C version, but the Zig, Python and Java versions (see the other repos).

```c
  /* match: search for regexp anywhere in text */
    int match(char *regexp, char *text)
    {
        if (regexp[0] == '^')
            return matchhere(regexp+1, text);
        do {    /* must look even if string is empty */
            if (matchhere(regexp, text))
                return 1;
        } while (*text++ != '\0');
        return 0;
    }

    /* matchhere: search for regexp at beginning of text */
    int matchhere(char *regexp, char *text)
    {
        if (regexp[0] == '\0')
            return 1;
        if (regexp[1] == '*')
            return matchstar(regexp[0], regexp+2, text);
        if (regexp[0] == '$' && regexp[1] == '\0')
            return *text == '\0';
        if (*text!='\0' && (regexp[0]=='.' || regexp[0]==*text))
            return matchhere(regexp+1, text+1);
        return 0;
    }

    /* matchstar: search for c*regexp at beginning of text */
    int matchstar(int c, char *regexp, char *text)
    {
        do {    /* a * matches zero or more instances */
            if (matchhere(regexp, text))
                return 1;
        } while (*text != '\0' && (*text++ == c || c == '.'));
        return 0;
    }
```

## Build

```bash
cmake -B build && cmake --build build
```

## Test

```bash
ctest --test-dir build
```
