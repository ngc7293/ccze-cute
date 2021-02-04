#include <ccze.h>

#include <stdio.h>
#include <string.h>

#include <pcre.h>

static const char* REGEX = "^\\[(?<date>[0-9\\- \\:]+)\\] \\((?<level>[a-z]+)\\)(?: <thread (?<thread>[0-9a-f]+)>)? \\[(?<component>[^\\]]+)\\] (?<message>.+)$";
static pcre* re;
static pcre_extra* re_extra;

struct level_color {
    ccze_color_t color;
    const char* level;
};

static struct level_color levels[] = {
    { CCZE_COLOR_STATIC_BLACK, "debug" },
    { CCZE_COLOR_STATIC_WHITE, "info" },
    { CCZE_COLOR_STATIC_YELLOW, "warning" },
    { CCZE_COLOR_STATIC_RED, "error" },
    { 0, NULL }
};


static void addstr_pad(ccze_color_t color, const char* str, int pad)
{
    int len = strlen(str);
    while (len++ < pad) { ccze_space(); }
    ccze_addstr(color, str);
}

static void ccze_cute_setup(void)
{
    int error = 0;
    int offset = 0;
    const char* errstr;
    re = pcre_compile2(REGEX, 0, &error, &errstr, &offset, NULL);
    re_extra = pcre_study(re, 0, &errstr);
}

static void ccze_cute_shutdown(void)
{
    pcre_free_study(re_extra);
    free(re);
}

static int ccze_cute_handle(const char *str, size_t length, char **rest)
{
    int vector[32];
    int ret = pcre_exec(re, re_extra, str, length, 0, 0, vector, 32);
    const char* line;

    if (ret < 0) {
        *rest = str;
        return 0;
    }

    if (pcre_get_named_substring(re, str, vector, ret, "date", &line)) {
        ccze_addstr(CCZE_COLOR_STATIC_WHITE, "[");
        ccze_addstr(CCZE_COLOR_STATIC_YELLOW, line);
        ccze_addstr(CCZE_COLOR_STATIC_WHITE, "] ");
    }

    if (pcre_get_named_substring(re, str, vector, ret, "level", &line)) {
        ccze_color_t color = CCZE_COLOR_STATIC_WHITE;
        for (int i = 0; levels[i].level != NULL; i++) {
            if (strcmp(line, levels[i].level) == 0) {
                color = levels[i].color;
                break;
            }
        }

        ccze_addstr(CCZE_COLOR_STATIC_WHITE, "(");
        addstr_pad(color, line, 7);
        ccze_addstr(CCZE_COLOR_STATIC_WHITE, ") ");
    }

    if (pcre_get_named_substring(re, str, vector, ret, "thread", &line)) {
        ccze_addstr(CCZE_COLOR_STATIC_WHITE, "<thread ");
        ccze_addstr(CCZE_COLOR_STATIC_BLUE, line + strlen(line) - 5);
        ccze_addstr(CCZE_COLOR_STATIC_WHITE, "> ");
    }

    if (pcre_get_named_substring(re, str, vector, ret, "component", &line)) {
        ccze_addstr(CCZE_COLOR_STATIC_WHITE, "[");
        addstr_pad(CCZE_COLOR_STATIC_GREEN, line, 12);
        ccze_addstr(CCZE_COLOR_STATIC_WHITE, "] ");
    }

    if (pcre_get_named_substring(re, str, vector, ret, "message", &line)) {
        ccze_addstr(CCZE_COLOR_STATIC_WHITE, line);
    }

    ccze_newline();
    *rest = NULL;
    return 1;
}

CCZE_DEFINE_PLUGIN(cute, FULL, "Plugin for cute::log format");