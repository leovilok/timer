#include <sys/timerfd.h> /* timerfd_create() timerfd_settime() */
#include <unistd.h> /* read(), get/set(e)uid() execl()*/
#include <stdlib.h> /* atol() */
#include <stdint.h> /* uint64_t */
#include <stdio.h>  /* perror() */

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s time cmd\n", argv[0]);
        return 1;
    }
    
    int err = seteuid(0);
    
    if (err == -1) {
        perror("seteuid (get root rights)");
        return 1;
    }
    
    int fd = timerfd_create(CLOCK_BOOTTIME_ALARM, TFD_CLOEXEC);

    if (fd == -1) {
        perror("timerfd creation");
        return 1;
    }

    const struct itimerspec tspec = { .it_value.tv_sec = atol(argv[1])};

    err = timerfd_settime(fd, 0, &tspec, NULL);
    
    if (err == -1) {
        perror("timerfd arming");
        return 1;
    }

    err = seteuid(getuid());
    
    if (err == -1) {
        perror("seteuid (drop root right)");
        return 1;
    }
   
    uint64_t res = 0;
    ssize_t size = read(fd, &res, sizeof(res));

    if (size == -1) {
        perror("timerfd read");
        return 1;
    }

    //printf("dring\n");

    execl("/bin/sh", "sh", "-c", argv[2], (char*)NULL);

    perror("exec");
    return 1;
}

