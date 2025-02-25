#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char buf[512];
    char *args[MAXARG];
    int i, len = 0;

    // Sao chép đối số lệnh
    for (i = 0; i < argc - 1; i++) {
        args[i] = argv[i + 1];
    }

    // Đọc từng dòng từ stdin
    while (read(0, buf + len, 1) == 1) {
        if (buf[len] == '\n') {
            buf[len] = 0;  // Kết thúc chuỗi
            args[i] = buf; // Gán đối số cuối cùng
            args[i + 1] = 0; // Đặt NULL cho exec

            if (fork() == 0) { // Tạo tiến trình con
                exec(args[0], args);
                exit(0);
            } else {
                wait(0); // Đợi tiến trình con kết thúc
            }
            len = 0;
        } else {
            len++;
        }
    }
    exit(0);
}
