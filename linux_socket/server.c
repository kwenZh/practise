#include<stdio.h>

#include"Tools.h"

// 建立socket之后开始监听客户端的连接 等待
void BeginListenClient(int server_sock_fd, MESSAGEMODEL *IRmessage);
// 发送消息给客户端
void SendMessageToClient(int *client_fds, char *input_msg);
// 处理新的客户端连接
void dealNewConnect(int *client_fds, int server_sock_fd, char *input_msg);
// 处理客户端发送过来的消息 判断 私发还是群发 还是show
void dealClientSendMessage(int *client_fds,MESSAGEMODEL *IRmessage, fd_set *server_fd_set);
// 处理客户端发出的群聊信息 将其发给除了发出的那个客户端其余之外的所有客户端
void dealGroupChat(int *client_fds, char *recv_msg, int i);
// 处理客户端发出的show命令
void dealShowCommand(int *client_fds, int i);
// 处理客户端的私聊消息　消息格式send+客户端编号　这里仅处理的是单个编号
void dealPrivateChat(int i, int *client_fds, char *recv_msg);


void dealGroupChat(int *client_fds, char *recv_msg, int i) {
  showNowTime();
  printf("客户端[%d]的群聊消息：%s", i, recv_msg);
  char message[MAX_SIZE];
  message[0] = '(';
  message[1] = 48 + i;
  strcpy(message + 2, "):");
  char new_message[MAX_SIZE];
  strcpy(new_message, "客户端的群消息");
  strcat(new_message, message);
  strcat(new_message, recv_msg);
  // 发送给在线的客户端 但是不包括自己
  for(int j = 0; j < MAX_CLIENT_COUNT; j++) {
    if(client_fds[j] != 0 && j != i) {
      send(client_fds[j], new_message, MAX_SIZE, 0);
    }
  }
}
void dealShowCommand(int *client_fds, int i) {
  int all = 0;
  for(int k = 0; k < MAX_CLIENT_COUNT; k++)
  {
    if(client_fds[k] != 0)
    {
      all++;
    }
  }
  showNowTime();
  printf("客户端[%d]查看当前在线人数\n", i);
  char new_message[MAX_SIZE];
  strcpy(new_message, "当前在线的客户端数量为：");
  char number_all[10];
  number_all[0] = 48 + all;  //ascll码转化为数字字符
  number_all[1] = '\0';
  strcat(new_message, number_all);
  strcat(new_message, "个， 分别是客户端(");
  for(int j = 0; j < MAX_CLIENT_COUNT; j++)
  {
    if(client_fds[j] != 0) {
      number_all[0] = 48 + j;
      strcat(new_message, number_all);
      strcat(new_message, ","); //这里如果可以用c++连接字符串就很方便了
    }
  }
  strcat(new_message, ")\n");
  send(client_fds[i], new_message, MAX_SIZE, 0);
}

void dealPrivateChat(int i, int *client_fds, char *recv_msg) {
  int spaceIndex = getClientId(recv_msg);
  char resultId[20] = {'\0'};
  memcpy(resultId, recv_msg + 4, spaceIndex - 4);
  int toSendId =  atoi(resultId);
  showNowTime();
  if(client_fds[toSendId] == 0 || toSendId >= MAX_CLIENT_COUNT || toSendId < 0) {
    send(client_fds[i], "你要私聊的对象是个null，不存在,请检查格式[send+客户端编号] show查看当前在线客户端\n", MAX_SIZE, 0);
    printf("客户端[%d]向不存在的客户端[%d]私发一条消息：%s这个客户端一定是疯了....\n", i, toSendId, recv_msg + 6);
    return;
  }else {
    printf("客户端[%d]向客户端[%d]私发一条消息：%s", i, toSendId, recv_msg);
    if(toSendId == i) {
      send(client_fds[i], "不得私发给自己\n", MAX_SIZE, 0);
      return;
    }
    char new_message[MAX_SIZE];
    char fromId[50] = {'\0'};
    int fromId_len = kwenItoa(i, fromId ,50);
    strcpy(new_message, "客户端[");
    strcat(new_message, fromId + fromId_len + 1);  //这里是from
    strcat(new_message, "]发来消息：");
    strcat(new_message, recv_msg + spaceIndex + 1);
    send(client_fds[toSendId], new_message, MAX_SIZE, 0);
  }
}

void receiveMessage(int i, int *client_fds, char *recv_msg) {

  char dest[10] = {""};
  char new_recv_msg[MAX_SIZE];
  strncpy(dest, recv_msg, 4);
  if(strcmp(dest,"send")== 0)
  {
    //这是一条私发给某一个客户端的消息
    dealPrivateChat(i, client_fds, recv_msg);
  }
  // 如果是查看当前客户端数量
  else if(strcmp(dest, "show") == 0)
  {
    dealShowCommand(client_fds, i);
  }
  else
  {
    dealGroupChat(client_fds, recv_msg, i);
  }
}

void dealClientSendMessage(int *client_fds,MESSAGEMODEL *IRmessage, fd_set *server_fd_set) {
  for(int i = 0; i < MAX_CLIENT_COUNT; i++) {
    if(client_fds[i] != 0) {
      if(FD_ISSET(client_fds[i], server_fd_set))
      {
        // 处理某一个客户端发送过来的消息
        bzero(IRmessage->recv_msg, MAX_SIZE);
        long byte_num = recv(client_fds[i], IRmessage->recv_msg, MAX_SIZE, 0);
        if(byte_num > 0) {
          IRmessage->recv_msg[byte_num > MAX_SIZE ? MAX_SIZE : byte_num] = '\0';
          receiveMessage(i, client_fds, IRmessage->recv_msg);
        } else if(byte_num < 0) {
          showNowTime();
          printf("从客户端[%d]接受消息出错\n", i);
        } else {
          // 客户端的下线
            FD_CLR(client_fds[i], server_fd_set);
            client_fds[i] = 0;
            showNowTime();
            printf("客户端[%d]下线\n", i);
        }
      }
    }
  }
}

void dealNewConnect(int *client_fds, int server_sock_fd, char *input_msg) {
  struct sockaddr_in client_address;
  socklen_t address_len;
  int client_sock_fd = accept(server_sock_fd,
                              (struct sockaddr *)&client_address,
                              &address_len);
  showNowTime();
  printf("新的客户端加入client_sock_fd = %d\n", client_sock_fd);
  if(client_sock_fd > 0)
  {
    int index = -1;
    for(int i = 0; i < MAX_CLIENT_COUNT; i++) {
      if(client_fds[i] == 0) {
        //将加入的客户端放到闲置的数组中去
        index = i;
        client_fds[i] = client_sock_fd;
        break;
      }
    }
    if(index >= 0)
    {
      showNowTime();
      printf("新的客户端[%d]加入成功\n", index);
    }
    else
    {
        bzero(input_msg, MAX_SIZE);
        strcpy(input_msg, "服务器加入的客户端达到最大值，无法加入!\n");
        send(client_sock_fd, input_msg, MAX_SIZE, 0);
        showNowTime();
        printf("已连接客户端数达到最大值，新客户端加入失败 %s : %d\n",
        inet_ntoa(client_address.sin_addr),
        ntohs(client_address.sin_port));
    }
  }
  else {
    showNowTime();
    perror("连接错误\n");
  }
}
void SendMessageToClient(int *client_fds, char *input_msg) {

  bzero(input_msg, MAX_SIZE);
  fgets(input_msg, MAX_SIZE, stdin);
  // 判断服务端输入的内容 若输入shutdown则退出服务器 注意fgets连回车也会捕捉到　所以宏里面应加上\n
  if(strcmp(input_msg, SERVER_QUIT) == 0)
  {
    showNowTime();
    printf("服务器下线....\n");
    exit(0);
  }
  // 服务器查看当前在线客户端
  if(strcmp(input_msg, "allclient\n") == 0) {
    showNowTime();
    printf("服务器查看当前在线客户端\n");
    int all = 0;
    for(int k = 0; k < MAX_CLIENT_COUNT; k++)
    {
      if(client_fds[k] != 0)
      {
        all++;
      }
    }
    char new_message[MAX_SIZE];
    strcpy(new_message, "当前在线的客户端数量为：");
    char number_all[10];
    number_all[0] = 48 + all;  //ascll码转化为数字字符
    number_all[1] = '\0';
    strcat(new_message, number_all);
    strcat(new_message, "个， 分别是客户端(");
    for(int j = 0; j < MAX_CLIENT_COUNT; j++)
    {
      if(client_fds[j] != 0) {
        number_all[0] = 48 + j;
        strcat(new_message, number_all);
        strcat(new_message, ","); //这里如果可以用c++连接字符串就很方便了
      }
    }
    strcat(new_message, ")");
    puts(new_message);
  }else {
    char input_msg_new[MAX_SIZE];
    strcpy(input_msg_new, "服务端：");
    strcat(input_msg_new, input_msg);
    showNowTime();
    printf("发送消息给以下客户端：\n");
    for(int i = 0; i < MAX_CLIENT_COUNT; i++)
    {
      if(client_fds[i] != 0)
      {
        // 发送消息给所有的客户端
        printf("client_fds[%d]=%d\n",i, client_fds[i]);
        send(client_fds[i], input_msg_new, MAX_SIZE, 0);
      }
    }
  }
}

void  BeginListenClient(int server_sock_fd, MESSAGEMODEL *IRmessage) {
    if(listen(server_sock_fd, BACKLOG) == ERROR_FLAG) {
      showNowTime();
      perror("listen error\n");

      exit(1);
    }
    showNowTime();
    printf("server_sock_fd = %d\n", server_sock_fd); //这里可以不用再打印出来
    int client_fds[MAX_CLIENT_COUNT] = {0};  //记录连接的客户端
    fd_set server_fd_set;
    int max_fd = -1;
    struct timeval tv;

// 所谓的监听就是一直等待
    while(1) {
      initTime(&tv);
      FD_ZERO(&server_fd_set);
      FD_SET(STDIN_FILENO, &server_fd_set);

      if(max_fd < STDIN_FILENO) {
        max_fd = STDIN_FILENO;
      }

      //服务端socket　　忘记写上代码 客户端上服务器不显示的问题在这里
      FD_SET(server_sock_fd, &server_fd_set);
      if(max_fd < server_sock_fd) {
        max_fd = server_sock_fd;
      }

      // 客户端连接
      for(int i = 0; i < MAX_CLIENT_COUNT; i++) {
        if(client_fds[i] != 0) {
          FD_SET(client_fds[i], &server_fd_set);
          if(max_fd < client_fds[i]) {
            max_fd = client_fds[i];
          }
        }
      }
      int ret = select(max_fd + 1, &server_fd_set, NULL, NULL, &tv);

          //这里就不对进行不到这里
      if(ret <= 0) {
        continue;
      }
      else {
        if(FD_ISSET(STDIN_FILENO, &server_fd_set)) {
          SendMessageToClient(client_fds, IRmessage->input_msg);
        }
        if(FD_ISSET(server_sock_fd, &server_fd_set)) {
          // 有新的连接请求
          dealNewConnect(client_fds, server_sock_fd, IRmessage->input_msg);
        }
        dealClientSendMessage(client_fds, IRmessage, &server_fd_set);
      }
    }

}

int main(int argc, char const *argv[]) {
  if(argc != 3) {
    usage(argv[0]);

    return ERROR_FLAG;
  }
  MESSAGEMODEL IRmessage;
  int on = 1;

// 本地地址
  struct sockaddr_in server_addr; //本地的地址
// 创建socket
  int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(server_sock_fd == ERROR_FLAG) {
    showNowTime();
    perror("socket error\n");

    return 1;
  }
  int bind_result = setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
  initLocalAddress(&server_addr, argv);

// 绑定socket
  bind_result = bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  // bind 普遍遭遇的问题是试图绑定一个已经在使用的端口。该陷阱是也许没有活动的套接字存在，
  // 但仍然禁止绑定端口（bind 返回 EADDRINUSE），它由 TCP 套接字状态 TIME_WAIT 引起。
  // 该状态在套接字关闭后约保留 2 到 4 分钟。在 TIME_WAIT 状态退出之后，套接字被删除，
  // 该地址才能被重新绑定而不出问题。 改setsockopt就好了
  // 参考文章：

  if(bind_result == -1) {
    showNowTime();
    perror("Bind eror!\n");
    return 1;
  }
  showNowTime();
  printf("服务端已建立成功！\nIp:[%s]\nPort: [%s]\n等待客户端加入.....\n",  argv[1], argv[2]);
// 开始监听等待客户端
  BeginListenClient(server_sock_fd, &IRmessage);
  close(server_sock_fd);  //这一步的关闭是有必要的

  return 0;
}
