# 네트워크 기반의 작업 제어 시스템

## 소개
클라이언트들이 작업 서버에 작업 요청을 보내고, 서버가 해당 작업을 처리 후 그 결과를 클라이언트에게 반환하는 시스템입니다. 
여러 개의 클라이언트가 서버에 작업을 요청 할 수 있고, 서버는 요청을 큐에 넣어 순차적으로 처리 또는 병렬 처리 할 수 있습니다.
연산 작업이나 데이터 처리 및 프로그램을 통합적으로 제어 및 처리 할 수 있습니다.


## 🛠 주요 기능

### 🙋‍♀️ 작업기능
- TCP/IP 소켓 통신: 클라이언트 서버 간의 통신
https://www.notion.so/TCP-IP-1036676582de80f2bf02f1f45d15d9d3

- 작업 큐 처리
```c
typedef struct {
    char command[BUFFER_SIZE];   // 장비 제어 명령
    int client_socket;           // 클라이언트 소켓
} Task;

// 작업 큐
Task task_queue[QUEUE_SIZE];
```

- 큐 동기화



- 자식 프로세스
https://www.notion.so/f501b039ab6143608eae690e317af4ca
- 시그널 종료
https://www.notion.so/50179384893b47dd93a70f77a842b520 
- o3 최적화
```
gcc -o3 server server.c backup.c -lpthread
```
cf.) 백업 파일은 -> https://github.com/ldeadlysinx/MultiThreadBackup

## 🔬 주요 과제
