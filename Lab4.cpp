#include <iostream>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

using namespace std;

const int SEM_CNT = 2, BUFFER_COUNT = 8;

pid_t read_process_pid, write_process_pid;
int sem_id, shm_id;

namespace my_sem {
    void P(int sem_id, int index) {
        struct sembuf sem{};
        sem.sem_num = index;
        sem.sem_op = -1;
        sem.sem_flg = 0;
        semop(sem_id, &sem, 1);
    }

    void V(int sem_id, int index, short incr = 1) {
        struct sembuf sem{};
        sem.sem_num = index;
        sem.sem_op = incr;
        sem.sem_flg = 0;
        semop(sem_id, &sem, 1);
    }
}

struct Buff {
    bool end;
    char buff[1024];
};

Buff *get_shm() {
    return (Buff *) shmat(shm_id, nullptr, 0);
}

void parent() {
    cout << "waiting pid " << read_process_pid << ", " << write_process_pid << endl;

    int status_read = 0, status_write = 0;
    waitpid(read_process_pid, &status_read, 0);
    waitpid(write_process_pid, &status_write, 0);

    for (int i = 0; i < SEM_CNT; i++) {
        semctl(sem_id, i, IPC_RMID);
    }

    shmctl(shm_id, IPC_RMID, nullptr);
    cout << "exit with status" << endl;
    cout << status_read << ", " << status_write << endl;
}

void read_process() {
    Buff *base_buf = get_shm();
    cout << "read process start " << endl;
    int current = -1;
    while (true) {
        my_sem::P(sem_id, 1);

        current = (current + 1) % BUFFER_COUNT;
        Buff *buff = &base_buf[current];
        cout << "read: " << buff->buff << endl;

        my_sem::V(sem_id, 0);
        if (buff->end) break;
    }
    cout << "read process exit" << endl;
}

void write_process() {
    Buff *base_buf = get_shm();

    cout << "write process start " << endl;
    int current = -1;
    for (int i = 0; i < 100; i++) {
        current = (current + 1) % BUFFER_COUNT;
        my_sem::P(sem_id, 0);

        Buff *buff = &base_buf[current];
        memset(buff->buff, 0, sizeof(Buff));
        if (i == 99) buff->end = true;
        sprintf(buff->buff, "hzytql_%d", i);
        cout << "written: " << buff->buff << endl;

        my_sem::V(sem_id, 1);
    }
    cout << "write process exit" << endl;
}

int main() {
    sem_id = semget(IPC_PRIVATE, SEM_CNT, IPC_CREAT | 0666);
    shm_id = shmget(IPC_PRIVATE, sizeof(Buff) * BUFFER_COUNT, IPC_CREAT | 0666);

    my_sem::V(sem_id, 0, BUFFER_COUNT);

    pid_t t1 = fork();
    if (t1 < 0) {
        cerr << "fork error";
        exit(0);
    }
    if (t1) { // parent
        pid_t t2 = fork();
        if (t2 < 0) {
            cerr << "fork error";
            exit(0);
        }

        if (t2) { // parent
            read_process_pid = t1, write_process_pid = t2;
            parent();
        } else {
            write_process();
        }
    } else { // read
        read_process();
    }

    return 0;
}