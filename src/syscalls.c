#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"

int _write(int file, char *ptr, int len) {
    (void)file;
    for (int i = 0; i < len; i++) {
        UARTCharPut(UART0_BASE, ptr[i]);   // change UART0_BASE if you use another port
    }
    return len;
}

int _close(int file) {
    (void)file;
    errno = EBADF;
    return -1;
}

int _fstat(int file, struct stat *st) {
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file) {
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    (void)file; (void)ptr; (void)dir;
    return 0;
}

int _read(int file, char *ptr, int len) {
    (void)file; (void)ptr; (void)len;
    return 0;
}

int _open(const char *name, int flags, int mode) {
    (void)name; (void)flags; (void)mode;
    errno = ENOSYS;
    return -1;
}

int _getpid(void) {
    return 1;
}

int _kill(int pid, int sig) {
    (void)pid; (void)sig;
    errno = EINVAL;
    return -1;
}