#include <iostream>
#include <unistd.h>
#include <sys/sem.h>
#include <csignal>

using namespace std;

int pipe_fd[2];
pid_t children1_pid, children2_pid;

void signal_handler(int sig_no) {
    if (sig_no == SIGINT) {
        cout << "sigint received" << endl;
        kill(children1_pid, SIGKILL);
        kill(children2_pid, SIGKILL);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
    }
}

void parent() {
    signal(SIGINT, signal_handler);

    int status1 = 0, status2 = 0;
    waitpid(children1_pid, &status1, 0);
    waitpid(children2_pid, &status2, 0);
    cout << status1 << ", " << status2 << endl;
}

void children1() {
    for (int i = 0; i <= 100000008; i++) {
        string str = "hzytql_" + to_string(i);
        write(pipe_fd[1], str.c_str(), str.size());
        sleep(1);
    }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void children2() {
    while (true) {
        char buffer[100];
        int n = read(pipe_fd[0], buffer, 100);
        buffer[n] = '\0';
        cout << buffer << endl;
    }
}
#pragma clang diagnostic pop

int main() {
    pipe(pipe_fd);

    pid_t t1 = fork();
    if (t1 == 0) {
        children1();
    } else {
        children1_pid = t1;
        pid_t t2 = fork();
        if (t2 == 0) {
            children2();
        } else {
            children2_pid = t2;
            parent();
        }
    }
    return 0;
}