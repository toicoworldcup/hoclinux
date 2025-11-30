#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
void *in_thong_bao(void *thread_id) {
    long tid;
    tid = (long)thread_id; 
    printf("--- Luong %ld: Dang chay cong viec cua toi ---\n", tid);
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
    sleep(1);
    printf("luong 1 ket thuc \n" );
    pthread_exit(NULL);
}
void *thread2(void *arg){
    long tid = (long)arg;
    printf("luong 2 bat dau  \n");
    sleep(3);
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

    return EXIT_SUCCESS;

}