#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#define ITE 100000000
int count =0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER; 
void *in_thong_bao(void *thread_id) {
    long tid;
    tid = (long)thread_id; 
    printf(" %ld: Dang chay cong viec cua toi ---\n", tid);
    pthread_exit(NULL); 
}
int pthread_create(
    pthread_t *thread,
    const pthread_attr_t *attr,
    void *(*start_routine)(void *),
    void *arg
);
int pthread_join(//
    pthread_t thread,       
    void **retval          
);
void *thread1(void *arg){
    long tid = (long)arg;
    printf("luong 1 bat dau  \n");
    for(int i=0;i< ITE;i++){
        //lock khoa
        pthread_mutex_lock(&count_mutex);
        count ++;
        //mo khoa
        pthread_mutex_unlock(&count_mutex);
    }
    printf("luong 1 ket thuc \n" );
    pthread_exit(NULL);
}
void *thread2(void *arg){
    long tid = (long)arg;
    printf("luong 2 bat dau  \n");
    for(int i=0;i< ITE;i++){
        //lock khoa
        pthread_mutex_lock(&count_mutex);
        count ++;
        //mo khoa
        pthread_mutex_unlock(&count_mutex);    }
    printf("luong 2 ket thuc \n");
    pthread_exit(NULL);

}
int main(){
    pthread_t threadA,threadB;
    int ret;
    long id1=1,id2=2;
    printf("bat dau chuong trinh \n");
    //tao luong a va b
    ret = pthread_create(&threadA,NULL,thread1,(void *) id1);
    if(ret != 0){
        perror("loi tao luong 1 \n");

        return EXIT_FAILURE;
    }

    ret = pthread_create(&threadB,NULL,thread2,(void *) id2);
    if(ret != 0){
        perror("loi tao luong 2 \n");

        return EXIT_FAILURE;
    }

    printf("dang thuc hien luong a va b \n");
    pthread_join(threadB,NULL);
    pthread_join(threadA,NULL);
    pthread_mutex_destroy(&count_mutex);

    printf("kq dung la %d \n",ITE*2);
    printf("kq hien tai la %d \n",count);

    return EXIT_SUCCESS;

}