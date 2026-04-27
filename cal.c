#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

const char *months[] = {
    "January", "February", "March", "April",
    "May", "June", "July", "August",
    "September", "October", "November", "December"};

int days_in_month(int year, int month)
{
    int num_of_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 1)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {
            return 29;
        }
    }
    return num_of_days[month];
}

int first_weekday(int year, int month)
{
    int y = year;
    int m = month + 1;
    if (m <= 2) {
        m += 12;
        y -= 1;
    }
    int K = y % 100;
    int J = y / 100;
    int q = 1;
    int h = (q + 13*(m+1)/5 + K + K/4 + J/4 + 5*J) % 7;

    return (h + 5) % 7 + 1; // 1 - monday, 2 - tuesday... 7 - sunday
}

void print_month(int year, int month, int highlight_day)
{
    int days = days_in_month(year, month);
    int start = first_weekday(year, month);

    printf("     %s %d\n", months[month], year);
    printf("Mo Tu We Th Fr Sa Su\n");

    int column; // 0 - monday 6 - sunday

    for (column = 1; column < start; column++)
    {
        printf("   ");
    }

    for (int day = 1; day <= days; day++)
    {
        if (day == highlight_day)
        {
            printf("\033[7m%2d\033[0m ", day);
        }
        else
        {
            printf("%2d ", day);
        }
        column++;

        if (column > 7)
        {
            printf("\n");
            column = 1;
        }
    }

    if (column != 1)
    {
        printf("\n");
    }
    printf("\n");
}

void print_month_week(int year, int month, int week, int highlight_day)
{
    int num_of_days = days_in_month(year, month);
    int first_weekday_in_month = first_weekday(year, month);

    for (int x = 0; x < 7; x++)
    {
        int day = week * 7 + x + 2 - first_weekday_in_month;
        if (day >= 1 && day <= num_of_days)
        {
            if (day == highlight_day)
            {
                printf("\033[7m%2d\033[0m ", day);
            }
            else
            {
                printf("%2d ", day);
            }
        }
        else
        {
            printf("   ");
        }
    }
}

void print_year(int year, int today_year, int today_month, int today_day)
{
    char buf[64];
    sprintf(buf, "%d", year);
    int year_len = strlen(buf);
    int pad = (64 - year_len) / 2;
    printf("%*s%d\n", pad, "", year);

    for (int quarter = 0; quarter < 4; quarter++)
    {
        int m1 = quarter * 3;
        int m2 = quarter * 3 + 1;
        int m3 = quarter * 3 + 2;

        int pad1 = (20 - strlen(months[m1])) / 2;
        int pad2 = (20 - strlen(months[m2])) / 2;
        int pad3 = (20 - strlen(months[m3])) / 2;

        printf("%*s%s%*s  %*s%s%*s  %*s%s%*s\n",
               pad1, "", months[m1], 20 - pad1 - (int)strlen(months[m1]), "",
               pad2, "", months[m2], 20 - pad2 - (int)strlen(months[m2]), "",
               pad3, "", months[m3], 20 - pad3 - (int)strlen(months[m3]), "");

        printf("Mo Tu We Th Fr Sa Su  Mo Tu We Th Fr Sa Su  Mo Tu We Th Fr Sa Su\n");
        for (int y = 0; y <= 5; y++)
        {
            int h1 = (year == today_year && m1 == today_month) ? today_day : 0;
            print_month_week(year, m1, y, h1);
            printf(" ");

            int h2 = (year == today_year && m2 == today_month) ? today_day : 0;
            print_month_week(year, m2, y, h2);
            printf(" ");

            int h3 = (year == today_year && m3 == today_month) ? today_day : 0;
            print_month_week(year, m3, y, h3);
            printf("\n");
        }
    }
}

int parse_number(const char *str, long *value)
{
    char *end;
    *value = strtol(str, &end, 10);
    if (end == str || *end != '\0')
    {
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    int year = t->tm_year + 1900;
    int month = t->tm_mon;
    int today_day = t->tm_mday;

    if (argc == 1)
    {
        print_month(year, month, today_day);
    }
    else if (argc == 2)
    {
        long y; // year
        if (!parse_number(argv[1], &y) || y < 1 || y > 9999)
        {
            fprintf(stderr, "Incorrect argument provided.\nUse values like: cal [[1..12]] 1..9999\n");
            return 1;
        }
        print_year((int)y, year, month, today_day);
    }
    else if (argc == 3)
    {
        long m, y; // m - month y - year
        if (!parse_number(argv[1], &m) || m < 1 || m > 12)
        {
            fprintf(stderr, "Incorrect argument provided.\n Use values like: cal [[1..12]] 1..9999\n");
            return 1;
        }
        if (!parse_number(argv[2], &y) || y < 1 || y > 9999)
        {
            fprintf(stderr, "Incorrect argument provided.\nUse values like: cal [[1..12]] 1..9999\n");
            return 1;
        }
        int highlight_day = ((int)y == year && (int)m - 1 == month) ? today_day : 0;
        print_month((int)y, ((int)m) - 1, highlight_day);
    }
    else
    {
        fprintf(stderr, "Incorrect argument provided.\nUse values like: cal [[1..12]] 1..9999\n");
        return 1;
    }

    return 0;
}