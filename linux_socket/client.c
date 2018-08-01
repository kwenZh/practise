#include<stdio.h>

#include"Tools.h"

void dealReceiveMessage(int server_sock_fd, char *recv_msg);
void sendMessage(int server_sock_fd, char *input_msg);
void BeginConnectServer(struct sockaddr_in server_addr, int server_sock_fd, MESSAGEMODEL message);


void BeginConnectServer(struct sockaddr_in server_addr, int server_sock_fd, MESSAGEMODEL message) {
    if(connect(server_sock_fd, (struct sockaddr *)&server_addr,  sizeof(struct sockaddr_in)) == 0)
    {
        showNowTime();
        printf("已连接到服务器[%s: %d]\n",
        inet_ntoa(server_addr.sin_addr),
        ntohs(server_addr.sin_port));
        printf("[show]:查看当前在线客户端编号\n[quit]:客户端下线\n[send+clientId]:私发给某个客户端消息\n");
        fd_set client_fd_set;
        struct timeval tv;
        while(1)
        {
          initTime(&tv);
          FD_ZERO(&client_fd_set);
          FD_SET(STDIN_FILENO, &client_fd_set);
          FD_SET(server_sock_fd, &client_fd_set);

          select(server_sock_fd + 1, &client_fd_set, NULL, NULL, &tv);
          if(FD_ISSET(STDIN_FILENO, &client_fd_set))
          {
            sendMessage(server_sock_fd, message.input_msg);
          }

          if(FD_ISSET(server_sock_fd, &client_fd_set))
          {
            // 接受服务端传递的消息
            dealReceiveMessage(server_sock_fd,message.recv_msg);
          }
        }
     }
}
void sendMessage(int server_sock_fd, char *input_msg) {
  bzero(input_msg, MAX_SIZE);
  fgets(input_msg, MAX_SIZE, stdin);

  if(strcmp(input_msg, CLIENT_QUIT) == 0) {  //由于fget会捕捉到回车符\n 所以这里判断的时候 需要加上\n
    showNowTime();
    printf("自己下线...\n");
    exit(0);
  }

  if(send(server_sock_fd, input_msg, MAX_SIZE, 0) == -1)
  {
    showNowTime();
    perror("客户端发送消息出错! \n");
  }
}
void dealReceiveMessage(int server_sock_fd, char *recv_msg) {
  // 接受服务端传递的消息
  bzero(recv_msg, MAX_SIZE);
  long byte_num = recv(server_sock_fd, recv_msg, MAX_SIZE, 0);
  if(byte_num > 0)
  {
    if(byte_num > MAX_SIZE)
    {
      byte_num = MAX_SIZE;
      recv_msg[byte_num] = '\0';
    }
      showNowTime();
      printf("%s", recv_msg);
  }
  else if (byte_num < 0)
  {
    showNowTime();
    printf("接受消息出错！\n");
  }
  else
  {
    showNowTime();
    printf("与服务器失去连接\n");
    exit(0);
  }
}

int main(int argc, char const *argv[]) {
  if(argc != 3) {
    // 这三个参数分别是 执行者 比如a.out  和输入的两个参数ip和端口号
    usage(argv[0]);
    exit(1);
  }
  struct sockaddr_in server_addr;
  initLocalAddress(&server_addr, argv);
  MESSAGEMODEL message;         //将收发消息包装在一起

  int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(server_sock_fd == -1) {
    showNowTime();
    perror("socket error");
    return 1;
  }
  BeginConnectServer(server_addr, server_sock_fd, message);
  close(server_sock_fd);

  return 0;
}
