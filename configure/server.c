#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "backup.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define QUEUE_SIZE 10
#define LOG_FILE "/home/ldeadlysinx/바탕화면/testlog/server.log"

// 로그 파일에 기록하는 함수
void write_log(const char *message) {
    FILE *log_file = fopen(LOG_FILE, "a");  // 로그 파일을 추가 모드로 열기
    if (log_file == NULL) {
        perror("로그 파일을 열 수 없습니다");
        
        return;
    }
    
    time_t now = time(NULL);
    char *time_str = ctime(&now);  // 현재 시간을 문자열로 변환

    time_str[strlen(time_str) - 1] = '\0';  // 마지막 개행 문자 제거

    fprintf(log_file, "[%s] %s\n", time_str, message);  // 로그 파일에 시간과 메시지 기록
    fclose(log_file);
}

typedef struct {
    char command[BUFFER_SIZE];   // 장비 제어 명령
    int client_socket;           // 클라이언트 소켓
} Task;

// 작업 큐
Task task_queue[QUEUE_SIZE];
int queue_count = 0;
pthread_mutex_t queue_lock;

// 장비 제어 함수 
void execute_command(const char* command) {
    printf("명령어 처리중: %s\n", command);
    char tempmsg[2048];
    snprintf(tempmsg, sizeof(tempmsg), "명령어 처리중: %s", command);
    write_log(tempmsg);
    sleep(2);  // 장비가 작업을 수행하는 시간

    
    // if (strcmp(command, "backup") == 0) {
    //     execute_backup();  // backup 실행
    // } else if (strcmp(command, "stop") == 0) {
    //     stop_backup();  // backup 중단
    // } else {
    //     printf("알 수 없는 명령: %s\n", command);
    // }
    
}

// SIGINT 핸들러 함수: 
void handle_sigint(int signo) {
    write_log("서버를 종료합니다.");
    printf("서버를 종료합니다.\n");
    exit(0);
}




// 클라이언트 요청 처리 스레드
void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    char buffer[BUFFER_SIZE];
    int read_size;

    while ((read_size = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[read_size] = '\0';

        pthread_mutex_lock(&queue_lock);  // 작업 큐에 추가
        if (queue_count < QUEUE_SIZE) {
            Task new_task;
            strcpy(new_task.command, buffer);
            new_task.client_socket = client_socket;
            task_queue[queue_count++] = new_task;
            printf("제어 명령: %s\n", buffer);
            char tempmsg[2048];
            snprintf(tempmsg, sizeof(tempmsg), "제어 명령: %s", buffer);
            write_log(tempmsg);
        }
        pthread_mutex_unlock(&queue_lock);
    }
    close(client_socket);
    return NULL;
}

// 작업 처리 스레드 (작업 큐 처리)
void* worker_thread(void* arg) {
    while (1) {
        pthread_mutex_lock(&queue_lock);
        if (queue_count > 0) {
            Task task = task_queue[--queue_count];
            pthread_mutex_unlock(&queue_lock);

            // 명령 실행
            execute_command(task.command);
            
            // 결과를 클라이언트로 전송
            char result[] = "명령 처리 완료\n";
            char tempmsg[2048];
            snprintf(tempmsg, sizeof(tempmsg), "명령 처리 완료: %s", task.command);
            write_log(tempmsg);
            send(task.client_socket, result, strlen(result), 0);
            close(task.client_socket);
        } else {
            pthread_mutex_unlock(&queue_lock);
        }
        sleep(1);  // 큐를 주기적으로 확인
    }
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t worker;

    write_log("명령어 서버 실행시작");

    // SIGINT 신호에 대해 handle_sigint 함수를 호출하도록 설정
    signal(SIGINT, handle_sigint);

    // 작업 큐를 위한 뮤텍스 초기화
    pthread_mutex_init(&queue_lock, NULL);

    // 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("소켓 생성 실패");
        write_log("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 바인딩
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("바인딩 실패");
        write_log("바인딩 실패");
        exit(EXIT_FAILURE);
    }

    // 리슨 상태로 전환
    if (listen(server_fd, 3) < 0) {
        perror("리스닝 실패");
        write_log("리스닝 실패");
        exit(EXIT_FAILURE);
    }

    // 작업 처리 스레드 시작
    pthread_create(&worker, NULL, worker_thread, NULL);

    printf("현재 서버 포트 %d...\n", PORT);

    //서버가 클라이언트의 연결 요청을 받아들이는것
    while ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) >= 0) {
        pthread_t client_thread;
        printf("새로운 처리 연결중\n");
        pthread_create(&client_thread, NULL, handle_client, &new_socket);
        pthread_detach(client_thread);
    }

    close(server_fd);
    return 0;
}
