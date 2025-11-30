#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_RES 3
#define NUM_THREAD 5
//khai bao semaphore
sem_t res_semaphore;

void *work(void *arg){
    long tid = (long)arg;
    printf(" luong %ld dang cho \n",tid);
    //yeu cau slot,bo dem <=0 thi bi block
    sem_wait(&res_semaphore);
    printf("luong %ld dang dung tai nguyen \n",tid);
    sleep(5);
    printf("luong %ld hoan thanh su dung \n",tid);
    //giai phong slot
    sem_post(&res_semaphore);
    pthread_exit(NULL);
}

int main(){
    //mang thread
    pthread_t threads[NUM_THREAD];
    int ret;
    //khoi tao semaphore
    ret = sem_init(&res_semaphore,0,MAX_RES);
    if(ret != 0){
        perror("loi khoi tao /n");
        return EXIT_FAILURE;

    }
    printf("%d luong co the truy cap \n",NUM_THREAD);
    // Tạo 5 luong
    for (long i = 0; i < NUM_THREAD; i++) {
        pthread_create(&threads[i], NULL, work,(void *)(i + 1));
    }

    // ket thuc 5 luong
    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_join(threads[i], NULL);
    }
    //huy semaphore
    sem_destroy(&res_semaphore);
    printf("tat ca luong hoan thanh");
    return EXIT_FAILURE;
}
