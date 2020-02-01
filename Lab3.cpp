#include <iostream>
#include <sys/sem.h>
#include <unistd.h>

using namespace std;

int sem_no, remain_tickets = 100;

namespace my_sem {
    void P(int sem_no, int index, short dec = -1) {
        struct sembuf sem{};
        sem.sem_num = index;
        sem.sem_op = dec;
        sem.sem_flg = 0;
        semop(sem_no, &sem, 1);
    }

    void V(int sem_no, int index, short inc = 1) {
        struct sembuf sem{};
        sem.sem_num = index;
        sem.sem_op = inc;
        sem.sem_flg = 0;
        semop(sem_no, &sem, 1);
    }
}

bool sell(int id) {
    bool result = false;
    my_sem::P(sem_no, 0);
    if (remain_tickets > 0) {
        remain_tickets--;
        result = true;
        cout << id << ", purchased one ticket" << endl;
    }
    my_sem::V(sem_no, 0);
    return result;
}

void *purchase(void *_id) {
    int id = *(int *) _id;
    while (sell(id)) {
        sleep(1);
    }
    return nullptr;
}

int main() {
    sem_no = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);

    my_sem::V(sem_no, 0);
    pthread_t arr[50];
    for (int i = 0; i < 50; i++) {
        int no = i;
        pthread_create(&arr[i], nullptr, purchase, (void *) &no);
    }

    for (int i = 0; i < 50; i++) {
        pthread_join(arr[i], nullptr);
    }

    semctl(sem_no, 0, IPC_RMID);
    return 0;
}