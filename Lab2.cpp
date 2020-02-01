#include <iostream>
#include <sys/sem.h>
#include <pthread.h>

using namespace std;

int sem_id;

namespace my_sem {
    void P(int sem_id, int index) {
        struct sembuf sem{};
        sem.sem_num = index;
        sem.sem_op = -1;
        sem.sem_flg = 0;
        semop(sem_id, &sem, 1);
    }
    void V(int sem_id, int index) {
        struct sembuf sem{};
        sem.sem_num = index;
        sem.sem_op = 1;
        sem.sem_flg = 0;
        semop(sem_id, &sem, 1);
    }
};

int result = 0;

void* accumulator(void* _) {
    for (int i = 0; i < 100; i++) {
        my_sem::P(sem_id, 0);
        result++;
        my_sem::V(sem_id, 1);
    }
    cout << "accumulator finished" << endl;

    return nullptr;
}

void* print(void* _) {
    for (int i = 0; i <= 100; i++) {
        my_sem::P(sem_id, 1);
        cout << result << endl;
        my_sem::V(sem_id, 0);
    }
    cout << "print finished" << endl;

    return nullptr;
}

int main() {
    sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);

    my_sem::V(sem_id, 1);

    pthread_t p1, p2;
    pthread_create(&p2, nullptr, print, nullptr);
    pthread_create(&p1, nullptr, accumulator, nullptr);

    pthread_join(p1, nullptr);
    pthread_join(p2, nullptr);

    semctl(sem_id, 0, IPC_RMID);
    semctl(sem_id, 1, IPC_RMID);
    return 0;
}