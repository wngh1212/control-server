#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>
#include <mysql/mysql.h>

#define OVECCOUNT 30  // Maximum number of matched patterns

// Regular expression pattern
const char *pattern = R"(^(\d{4}-\d{2}-\d{2})T(\d{2}:\d{2}:\d{2})(?:\.\d+)?(?:[+-]\d{2}:\d{2})? ([a-zA-Z0-9_]+) ([^\[\]]+)\[(\d+)\]: (.+)$)";

// Function to remove single quotes from a string
void remove_single_quotes(char *str) {
    int len = strlen(str);
    int i, j = 0;
    for (i = 0; i < len; i++) {
        if (str[i] != '\'') {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';  // Null-terminate the string
}

int main() {
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // File path
    const char *file_path = "/var/log/syslog";

    // Open file
    file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Compile regular expression
    pcre *re;
    const char *error;
    int erroffset;
    re = pcre_compile(pattern, 0, &error, &erroffset, NULL);
    if (re == NULL) {
        printf("Failed to compile regex: %s\n", error);
        exit(EXIT_FAILURE);
    }

    // Initialize MySQL connection
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }

    // Connect to MySQL database
    if (mysql_real_connect(conn, "localhost", "root", "1234", "syslog_db", 0, NULL, 0) == NULL) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    // Iterate over each line
    while ((read = getline(&line, &len, file)) != -1) {
        // Match regular expression
        int ovector[OVECCOUNT];
        int rc = pcre_exec(re, NULL, line, strlen(line), 0, 0, ovector, OVECCOUNT);
        if (rc >= 0) {
            // If match is successful
            char *date = strndup(line + ovector[2], ovector[3] - ovector[2]);
            char *time = strndup(line + ovector[4], ovector[5] - ovector[4]);
            char *host_name = strndup(line + ovector[6], ovector[7] - ovector[6]);
            char *program_name = strndup(line + ovector[8], ovector[9] - ovector[8]);
            char *pid = strndup(line + ovector[10], ovector[11] - ovector[10]);
            char *message = strndup(line + ovector[12], ovector[13] - ovector[12]);

            // Remove single quotes from each string
            remove_single_quotes(date);
            remove_single_quotes(time);
            remove_single_quotes(host_name);
            remove_single_quotes(program_name);
            remove_single_quotes(pid);
            remove_single_quotes(message);

            char sql[1024];
            snprintf(sql, sizeof(sql), "INSERT INTO syslog (log_date, log_time, hostname, program_name, pid, message) VALUES ('%s', '%s', '%s', '%s', '%s', '%s')",
                     date, time, host_name, program_name, pid, message);
            //printf("SQL Query: %s\n", sql); // Print the SQL query

            // Execute SQL statement
            if (mysql_query(conn, sql) != 0) {
                fprintf(stderr, "Error: %s\n", mysql_error(conn));
            }

            // Free memory
            free(date);
            free(time);
            free(host_name);
            free(program_name);
            free(pid);
            free(message);
    }
    }

    // Clean up
    free(line);
    fclose(file);
    pcre_free(re);
    mysql_close(conn);

    return 0;
}
