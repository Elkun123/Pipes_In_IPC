#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main() {
    const char *fifoName = "myfile"; 

    int result[3][3] = {0};

    // Mở ống tên để đọc
    int fd = open(fifoName, O_RDONLY);
    if (fd == -1) {
        perror("Error opening FIFO");
        exit(1);
    }
    read(fd, result, sizeof(result));
    close(fd); // Đóng ống tên

    printf("Đã nhận dữ liệu từ producer.\n"); 

    // In kết quả ra console
    printf("Kết quả ma trận:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    // Tạo tên file theo định dạng thời gian (MMDDHHMM.out)
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char filename[50];
    strftime(filename, sizeof(filename) - 1, "%m%d%H%M.out", t);

    // Ghi kết quả vào file
    FILE *outputFile = fopen(filename, "w");
    if (outputFile == NULL) {
        perror("Error creating output file");
        exit(1);
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            fprintf(outputFile, "%d ", result[i][j]);
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile); // Đóng file

    return 0;
}
