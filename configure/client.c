#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define LOG_FILE "/home/ldeadlysinx/바탕화면/testlog/client.log"

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


int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char command[BUFFER_SIZE];

    // 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("소켓 생성 오류\n");
        write_log("소켓 생성 오류");
        return -1;
    }

    // 서버 주소 설정
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 서버 연결
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("유효하지 않는 주소\n");
        write_log("유효하지 않는 주소");
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("연결 실패n");
        write_log("포트 연결 실패");
        return -1;
    }

    // 제어 명령 입력
    printf("제어 명령 입력");
    fgets(command, BUFFER_SIZE, stdin);
    command[strcspn(command, "\n")] = 0;  // 줄바꿈 제거
    char tempmsg[2048];
    snprintf(tempmsg, sizeof(tempmsg), "제어 명령 입력: %s", command);
    write_log(tempmsg);

    // 서버에 명령 전송
    send(sock, command, strlen(command), 0);

    // 서버로부터 결과 수신
    read(sock, buffer, BUFFER_SIZE);
    printf("서버 반응: %s\n", buffer);
    memset(tempmsg, 0, sizeof(tempmsg)); 
    snprintf(tempmsg, sizeof(tempmsg), "서버 반응: %s", buffer);
    write_log(tempmsg);

    close(sock);
    return 0;
}

