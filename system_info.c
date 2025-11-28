#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define NAME "/proc/meminfo"
#define NAME1 "/proc/cpuinfo"
#define NAME2 "hethong.txt"
#define SIZE 100

int main(){
    int fd;
    char buffer[SIZE];
    ssize_t bytes_read,bytes_write;

    // mo file (/proc/meminfo)
    fd = open(NAME,O_RDONLY);
    if(fd == -1){
        perror("Loi: Khong mo duoc file nguon /proc/meminfo");
        return EXIT_FAILURE;
    }
    
    // mo file (hethong.txt) 
    int fd2;
    fd2= open(NAME2,O_WRONLY|O_CREAT|O_TRUNC,0644); 
    if(fd2 == -1){
        perror("Loi: Khong mo duoc file dich hethong.txt");
        close(fd);
        return EXIT_FAILURE;
    }

    // doc va ghi
    while((bytes_read=read(fd,buffer,SIZE-1))>0){
        // ghi
        bytes_write = write(fd2,buffer,bytes_read);
        if(bytes_write == -1){
            perror("Loi: Khong ghi duoc du lieu vao file dich");
            close(fd);
            close(fd2);
            return EXIT_FAILURE;
        }

    }  
    
    close(fd);
    close(fd2);
    printf("Ghi thong tin memory vao '%s' thanh cong.\n", NAME2); 
    return EXIT_SUCCESS;
}