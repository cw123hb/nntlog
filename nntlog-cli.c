#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void add_task() {
    char task_description[100];
    time_t current_time;
    struct tm *time_info;

    printf("Enter task description: ");
    fgets(task_description, sizeof(task_description), stdin);
    task_description[strcspn(task_description, "\n")] = '\0';

    current_time = time(NULL);
    time_info = localtime(&current_time);

    char *home_dir = getenv("HOME");
    size_t len = strlen(home_dir) + strlen("/worklog.csv") + 1;
    char *worklog_path = malloc(len);
    strcpy(worklog_path, home_dir);
    strcat(worklog_path, "/worklog.csv");

    FILE *worklog = fopen(worklog_path, "a");
    if (worklog == NULL) {
        perror("Error opening worklog file");
        exit(EXIT_FAILURE);
    }

    fprintf(worklog, "%04d-%02d-%02d %02d:%02d:%02d,%s\n",
            time_info->tm_year + 1900,
            time_info->tm_mon + 1,
            time_info->tm_mday,
            time_info->tm_hour,
            time_info->tm_min,
            time_info->tm_sec,
            task_description);

    fclose(worklog);
    printf("Task added to worklog at %04d-%02d-%02d %02d:%02d:%02d\n",
           time_info->tm_year + 1900,
           time_info->tm_mon + 1,
           time_info->tm_mday,
           time_info->tm_hour,
           time_info->tm_min,
           time_info->tm_sec);

    free(worklog_path);
}

int main() {
    add_task();
    return 0;
}
