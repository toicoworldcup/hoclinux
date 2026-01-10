#include<stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[],char *envp[]){
    char *user = getenv("USER");
    if(user!=NULL){
        printf("xin chao nguoi dung %s \n",user);

    }
    else{
        printf("khong tim thay nguoi dung \n");
    }
    printf("danh sach cac bien moi truong \n");
    for(int i=0;envp[i]!=NULL;i++){
        printf("%s\n",envp[i]);
    }
    return 0;

}