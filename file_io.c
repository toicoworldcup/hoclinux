#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#define SIZE 100
int main(){
    int fd;
    char data[]=" bai 1";
    char buffer[SIZE];
    __ssize_t bytes_read;
    printf("ghi du lieu vao file \n");
    //mo file
    fd = open("hello.txt",O_WRONLY|O_CREAT,0644);
    if(fd== -1){
        perror("loi khong mo duoc file \n");
        return EXIT_FAILURE;
    }

    //ghi du lieu vao file
    if(write(fd,data,strlen(data))== -1){
        perror("loi khong ghi duoc file \n");
        close(fd);
        return EXIT_FAILURE;
    }
     //dong file
    close(fd);
    printf("ghi du lieu vao file hello.txt thanh cong \n" );

    //doc du lieu tu file
    fd = open("hello.txt",O_RDONLY);
    if(fd == -1){
        perror("loi khong doc duoc file \n");
        return EXIT_FAILURE;
    }

    printf("noi dung file \n");
    bytes_read = read(fd,buffer,SIZE-1);
    if (bytes_read == -1)
    {
        perror("loi khong doc duoc file");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("%s",buffer);
    close(fd);
    printf("da doc file thanh cong \n");
    return EXIT_SUCCESS;
    



}