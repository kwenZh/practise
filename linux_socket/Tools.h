#ifndef _TOOLS_H_
#define _TOOLS_H_

#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<time.h>
#include <sys/types.h>

#define MAX_SIZE          1024
#define BACKLOG           5
#define MAX_CLIENT_COUNT  100
#define MAX_SIZE          1024  //500多个汉字

#define SAPCE_ASCLL       32

#define SERVER_QUIT       "shutdown\n"
#define SHOW_COMMAND      "show\n";
#define CLIENT_QUIT       "quit\n"


#define ERROR_FLAG        -1

typedef struct MESSAGEMODEL {
  char input_msg[MAX_SIZE];
  char recv_msg[MAX_SIZE];
}MESSAGEMODEL;

void initLocalAddress(struct sockaddr_in *server_addr, const char* argv[]);
void initTime(struct timeval *tv);
void showNowTime();
int getClientId(char *recv_msg);

// 返回的是十进制数字的长度
int kwenItoa(int val, char *result, int ar_len);

int kwenItoa(int val, char *result, int ar_len) {
  if(val == 0) {
    result[ar_len - 1] = '0';

    return ar_len - 2;
  }
  int count = ar_len - 1;

  while(val) {
    result[count--] = val % 10 + 48;
    val = val / 10;
  }
  result[ar_len]= '\0';

  return count;
}

int getClientId(char *recv_msg) {
  int spaceIndex = -1;

  for(int i = 0; recv_msg[i] && i < MAX_SIZE; i++) {
    if(recv_msg[i] == SAPCE_ASCLL) {
      spaceIndex = i;
      break;
    }
  }

  return spaceIndex;
}

void showNowTime() {
  struct timespec time;
  struct tm nowTime;

  clock_gettime(CLOCK_REALTIME, &time); //获取相对于1970到现在的秒数
  localtime_r(&time.tv_sec, &nowTime);
  printf("%04d-%02d-%02d %02d:%02d:%02d: ",
            nowTime.tm_year + 1900,
            nowTime.tm_mon + 1,
            nowTime.tm_mday,
            nowTime.tm_hour,
            nowTime.tm_min,
            nowTime.tm_sec);
}

void initTime(struct timeval *tv) {
  tv->tv_sec = 20;
  tv->tv_usec = 0;  //这一步都可以写成一个函数 在tool里面
}
void initLocalAddress(struct sockaddr_in *server_addr, const char* argv[]) {
  server_addr->sin_family = AF_INET;
  server_addr->sin_port = htons(atoi(argv[2])); //端口
  server_addr->sin_addr.s_addr = inet_addr(argv[1]); //地址
  bzero(&(server_addr->sin_zero), 8);
  // inet_ntoa
}
static void usage(const char* proc) {
  showNowTime();
	printf("%s [ip] [port]\n", proc);
}

#endif
