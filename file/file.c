#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
int main(){
    int fd;
    char *content = "hahjahjahjarqrwq121211221";
    char buffer[20];
    fd = open("content.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
    if(fd <0){
        perror("loi mo file /n");
        return 1;
    }
    write(fd,content,strlen(content));
    lseek(fd,12,SEEK_SET);
    int n = read(fd,buffer,sizeof(buffer)-1);
    if(n>0){
        buffer[n] = '\0';
        printf("ki tu so 12 la %c \n",buffer[0]);
        printf("noi dung tu ki tu so 12 la %s \n",buffer);
        
    }
    close(fd);
    return 0;
    
}