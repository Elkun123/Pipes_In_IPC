#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define MATRIX_SIZE 3

// Hàm đọc ma trận từ file
void readMatrixFromFile(const char* filename, int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
    fclose(file);
}

// Hàm cộng ma trận
void addMatrices(int result[MATRIX_SIZE][MATRIX_SIZE], int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i][j] += matrix[i][j];
        }
    }
}

int main(int argc, char *argv[]) {
    const char *fifoName = "myfile"; // Tên ống tên
    mkfifo(fifoName, 0666); // Tạo ống tên

    // Kiểm tra xem người dùng đã cung cấp ít nhất 1 file input hay chưa
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <matrix1.txt> [<matrix2.txt> ... <matrixN.txt>]\n", argv[0]);
        exit(1);
    }

    int result[MATRIX_SIZE][MATRIX_SIZE] = {0}; // Khởi tạo ma trận kết quả ban đầu với các phần tử bằng 0
    int matrix[MATRIX_SIZE][MATRIX_SIZE] = {0}; // Ma trận tạm thời để đọc từ file

    // Đọc và cộng từng ma trận từ các file được truyền vào
    for (int fileIndex = 1; fileIndex < argc; fileIndex++) {
        memset(matrix, 0, sizeof(matrix)); // Đặt lại giá trị của ma trận tạm thời về 0
        readMatrixFromFile(argv[fileIndex], matrix); // Đọc ma trận từ file
        addMatrices(result, matrix); // Cộng ma trận vào kết quả
    }

    // Ghi kết quả vào ống tên
    int fd = open(fifoName, O_WRONLY);
    if (fd == -1) {
        perror("Error opening FIFO");
        exit(1);
    }
    write(fd, result, sizeof(result));
    close(fd); // Đóng ống tên

    printf("Dữ liệu đã được gửi đến Consumer.\n");

    return 0;
}
