#include <sys/time.h>
#include <stdio.h>

unsigned long get_time();

int main(int nargs, char **args) {
        fprintf (stdout, "The current epoch time / ms: %ld\n", get_time());
        long start_time = get_time();
        int i;
        for (i = 0; i < 1e8; ++i);
        fprintf (stdout, "It took %ld ms to count to 10^8.\n", \
                        get_time() - start_time);
        return 0;
}

unsigned long get_time() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        unsigned long ret = tv.tv_usec;
        ret /= 1000;
        ret += (tv.tv_sec * 1000);
        return ret;
}