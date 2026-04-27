#include <stdio.h>
#include <time.h>

int main()
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    int year = t-> tm_year + 1900;
    int month = t->tm_mon;
    int day = t->tm_mday;

    const char *months[] = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
    };

    printf("%s %d\n", months[month], year);
    
    return 0;
}