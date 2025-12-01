#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    
    printf("DOI SO DONG LENH\n");
    printf("So luong doi so (argc): %d\n", argc);
    
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    
    printf("\n");

    printf("--- BIEN MOI TRUONG ---\n");
    char *home_dir = getenv("HOME");
    char *path = getenv("PATH");

    if (home_dir != NULL) {
        printf("Gia tri cua bien $HOME: %s\n", home_dir);
    } else {
        printf("Bien $HOME khong ton tai.\n");
    }

    if (path != NULL) {
        printf("Gia tri cua bien $PATH : %.50s...\n", path);
    }

    return 0;
}