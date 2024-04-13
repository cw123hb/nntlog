#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void submit_task(GtkWidget *widget, gpointer data) {
    GtkEntry *entry = GTK_ENTRY(data);
    const gchar *task_description = gtk_entry_get_text(entry);
    if (task_description == NULL || strlen(task_description) == 0) {
        printf("No task description provided.\n");
        return;
    }

    time_t current_time;
    struct tm *time_info;

    current_time = time(NULL);
    time_info = localtime(&current_time);

    char *home_dir = getenv("HOME");
    if (home_dir == NULL) {
        perror("HOME environment variable not set");
        exit(EXIT_FAILURE);
    }
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

    // Clear the entry field after submitting the task
    gtk_entry_set_text(entry, "");
}

void exit_application(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *entry;
    GtkWidget *submit_button;
    GtkWidget *exit_button;
    GtkWidget *vbox;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "No Nonsense Task Logger");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    entry = gtk_entry_new();
    // Set the maximum length of the entry widget to a reasonable value
    // For example, 1000 characters
    gtk_entry_set_max_length(GTK_ENTRY(entry), 1000);

    submit_button = gtk_button_new_with_label("Submit");
    exit_button = gtk_button_new_with_label("Exit");

    // Create a vertical box to hold the widgets
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), submit_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), exit_button, TRUE, TRUE, 0);

    g_signal_connect(submit_button, "clicked", G_CALLBACK(submit_task), entry);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(exit_application), NULL);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
// gcc `pkg-config --cflags gtk+-3.0` -o nntlog nntlog.c `pkg-config --libs gtk+-3.0`



