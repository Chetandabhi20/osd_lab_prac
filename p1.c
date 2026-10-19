#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <string.h>

void read_proc_info() {
    FILE *fp;
    char buffer[256];

    printf("\n================ VALIDATION USING /proc ================\n");

    // 1. Validate PID & PPID from /proc/self/status
    fp = fopen("/proc/self/status", "r");
    if (fp != NULL) {
        printf("[/proc/self/status]\n");
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strncmp(buffer, "Pid:", 4) == 0 || strncmp(buffer, "PPid:", 5) == 0) {
                printf("  %s", buffer);
            }
        }
        fclose(fp);
    } else {
        perror("Error opening /proc/self/status");
    }

    // 2. Validate Kernel Version from /proc/version
    fp = fopen("/proc/version", "r");
    if (fp != NULL) {
        printf("\n[/proc/version]\n");
        if (fgets(buffer, sizeof(buffer), fp)) {
            printf("  %s", buffer);
        }
        fclose(fp);
    } else {
        perror("Error opening /proc/version");
    }

    // 3. Validate Uptime from /proc/uptime
    fp = fopen("/proc/uptime", "r");
    if (fp != NULL) {
        double uptime_sec;
        if (fscanf(fp, "%lf", &uptime_sec) == 1) {
            printf("\n[/proc/uptime]\n");
            printf("  Uptime: %.2f seconds\n", uptime_sec);
        }
        fclose(fp);
    } else {
        perror("Error opening /proc/uptime");
    }

    // 4. Validate Memory from /proc/meminfo
    fp = fopen("/proc/meminfo", "r");
    if (fp != NULL) {
        printf("\n[/proc/meminfo]\n");
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strncmp(buffer, "MemTotal:", 9) == 0 ||
                strncmp(buffer, "MemFree:", 8) == 0 ||
                strncmp(buffer, "SwapTotal:", 10) == 0 ||
                strncmp(buffer, "SwapFree:", 9) == 0) {
                printf("  %s", buffer);
            }
        }
        fclose(fp);
    } else {
        perror("Error opening /proc/meminfo");
    }
}

int main() {
    struct utsname uts;
    struct sysinfo info;

    printf("================ SYSTEM CALLS OUTPUT ================\n");

    // 1. Process IDs
    printf("Process ID (PID)        : %d\n", getpid());
    printf("Parent Process ID (PPID): %d\n", getppid());

    // 2. Kernel and Host Information using uname()
    if (uname(&uts) == -1) {
        perror("uname() failed");
        return EXIT_FAILURE;
    }
    printf("OS Name                 : %s\n", uts.sysname);
    printf("Hostname                : %s\n", uts.nodename);
    printf("Kernel Release          : %s\n", uts.release);
    printf("Architecture            : %s\n", uts.machine);

    // 3. Memory and Uptime Information using sysinfo()
    if (sysinfo(&info) == -1) {
        perror("sysinfo() failed");
        return EXIT_FAILURE;
    }
    unsigned long unit = info.mem_unit ? info.mem_unit : 1;
    printf("System Uptime           : %ld seconds\n", info.uptime);
    printf("Total RAM               : %lu MB\n", (info.totalram * unit) / (1024 * 1024));
    printf("Free RAM                : %lu MB\n", (info.freeram * unit) / (1024 * 1024));
    printf("Total Swap              : %lu MB\n", (info.totalswap * unit) / (1024 * 1024));
    printf("Free Swap               : %lu MB\n", (info.freeswap * unit) / (1024 * 1024));
    printf("Active Processes        : %u\n", info.procs);

    // Validate with /proc
    read_proc_info();

    printf("\n24DCE026_Chetan Dabhi\n");
    return EXIT_SUCCESS;
}