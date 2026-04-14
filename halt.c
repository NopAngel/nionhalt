/*
* halt.c: halt/poweroff/reboot wrapper universal
*           -for OpenRC and Runit-
*
* @Copyright (C) 2026: MIT NopAngel
*      
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>

/* LOG PATH */
#define LOG_FILE "/var/log/nionhalt.log"


typedef enum {
    INIT_UNKNOWN,

    INIT_OPENRC,

    INIT_RUNIT
} InitSystem;



void log_event
(const char *action) 
{
    FILE *file = fopen(LOG_FILE, "a");
    if (file == NULL) 
    {
        /* If we can't write to /var/log (common in simulation), skip silently or log to stderr */
        fprintf(stderr, "Warning: Could not write to %s\n", LOG_FILE);
        return;
    }

    time_t now;
    time(&now);
    char *date = ctime(&now);
    date[strlen(date) - 1] = '\0'; /* Remove newline */

    struct passwd *pw = getpwuid(getuid());
    const char *username = pw ? pw->pw_name : "unknown";

    fprintf(file, "[%s] User: %s | Action: %s\n", date, username, action);
    fclose(file);
}

InitSystem detect_init
() 
{
    if (access("/sbin/openrc-run", F_OK) == 0 || access("/usr/bin/openrc", F_OK) == 0) 
    {
        return INIT_OPENRC;
    } 

    else if (access("/usr/bin/runit", F_OK) == 0 || access("/sbin/runit", F_OK) == 0) 
    {
        return INIT_RUNIT;
    }
    return INIT_UNKNOWN;
}

void execute_action
(const char *cmd, int simulate, const char *action_name) 
{
    log_event(action_name);
    if (simulate) {
        printf("\n  SIMULATION   Log entry would be: User tried to %s\n", action_name);
        printf("Target command: %s\n", cmd);
    } else {
        system(cmd);
    }
}

int main
(int argc, char *argv[]) 
{

    if (argc < 2) 
    {

        fprintf(stderr, "Usage: %s [shutdown|reboot]\n", argv[0]);
        return 1;

    }


    int simulate = (getuid() != 0);
    if (simulate) printf("Notice: Simulation mode (non-root user).\n");

    InitSystem init = detect_init();
    char *action = argv[1];

    if (init == INIT_OPENRC) 
    {
        if (strcmp(action, "shutdown") == 0) execute_action("openrc-shutdown --poweroff now", simulate, "shutdown");
        else if (strcmp(action, "reboot") == 0) execute_action("openrc-shutdown --reboot now", simulate, "reboot");
    } 
    else if (init == INIT_RUNIT) 
    {
        if (strcmp(action, "shutdown") == 0) execute_action("runit-init 0", simulate, "shutdown");
        else if (strcmp(action, "reboot") == 0) execute_action("runit-init 6", simulate, "reboot");
    } 
    else 
    {
        fprintf(stderr, "Error: Compatible init system not found.\n");
        return 1;
    }

    return 0;
}
