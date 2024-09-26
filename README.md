# 네트워크 기반의 작업 제어 시스템

## 소개
클라이언트들이 작업 서버에 작업 요청을 보내고, 서버가 해당 작업을 처리 후 그 결과를 클라이언트에게 반환하는 시스템입니다. 
여러 개의 클라이언트가 서버에 작업을 요청 할 수 있고, 서버는 요청을 큐에 넣어 순차적으로 처리 또는 병렬 처리 할 수 있습니다.
연산 작업이나 데이터 처리 및 프로그램을 통합적으로 제어 및 처리 할 수 있습니다.


## 🛠 주요 기능

### 🙋‍♀️ 작업기능
- TCP/IP 소켓 통신: 클라이언트 서버 간의 통신
[소켓통신][https://www.notion.so/TCP-IP-1036676582de80f2bf02f1f45d15d9d3](https://holy-taste-f34.notion.site/TCP-IP-1036676582de80f2bf02f1f45d15d9d3)

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
```c
pthread_mutex_lock(&queue_lock);
```
멀티스레드 환경에서는 여러 스레드가 동시에 같은 자원(예: 변수, 데이터 구조)에 접근할 수 있습니다. # 
pthread_mutex_lock() 함수는 특정 자원에 대한 접근을 한 스레드만 허용하도록 하여 이러한 문제를 방지합니다.

- 자식 프로세스
[https://www.notion.so/f501b039ab6143608eae690e317af4ca](https://holy-taste-f34.notion.site/f501b039ab6143608eae690e317af4ca)
- 시그널 종료
[https://www.notion.so/50179384893b47dd93a70f77a842b520 ](https://holy-taste-f34.notion.site/50179384893b47dd93a70f77a842b520)
- o3 최적화
```
gcc -o3 server server.c backup.c -lpthread
```
cf.) 백업 파일은 -> https://github.com/ldeadlysinx/MultiThreadBackup

## 🔬 주요 과제

- 작업큐 관리: 작업큐에 동시에 접근시 명령어 간의 경쟁 조건 발생 이를 통해 큐의 동기화 조건 필요
- 동기화 기능: 뮤테스락 은 바쁜 대기를 통해 확인하기 때문에 자원을 많이 잡아먹음 다른 동기화 기법 사용 필요 (ex. 모니터?)

## 🔬 추가했으면 하는 기능
-  보안 강화: 클라이언트와 서버 간의 통신을 암호화하고, 인증 민 권한 관리 추가 필요
-  성능 모니터링: 시스템 성능을 모니터링 하여 최적화를 위한 cpu 메모리 사용량 등을 기록 필요
  
