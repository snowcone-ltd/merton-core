#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int clock_t;

struct tm;

char *asctime(const struct tm *timeptr);
clock_t clock(void);
char *ctime(const time_t *clock);
double difftime(time_t time1, time_t time0);
struct tm *gmtime(const time_t *timer);
struct tm *localtime(const time_t *timer);
time_t mktime(struct tm *timeptr);
size_t strftime(char *s, size_t maxsize, const char *format, const struct tm *timeptr);
time_t time(time_t *tloc);

#ifdef __cplusplus
}
#endif
