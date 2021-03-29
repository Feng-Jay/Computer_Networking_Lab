#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>
using namespace std;

int main()
{
    //创建Udp_socket,socket函数来自 socket.h, IPPOROTO_UDP来自in.h
    int Udp_socket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    struct sockaddr_in sever_addr;
    //     struct sockaddr_in{
    //     sa_family_t     sin_family;   //地址族（Address Family），也就是地址类型
    //     uint16_t        sin_port;     //16位的端口号
    //     struct in_addr  sin_addr;     //32位IP地址
    //     char            sin_zero[8];  //不使用，一般用0填充
    // };
    memset(&sever_addr,0,sizeof(sever_addr));
    sever_addr.sin_family=AF_INET;
    sever_addr.sin_port=htons(1234);
    sever_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//function from arpa/inet.h
    bind(Udp_socket,(struct sockaddr*)&sever_addr,sizeof(sever_addr));

    cout<<"Sever is ready to response!!!"<<endl;

    char recv_buf[50];
    char send_buf[50];
    int recv_num;
    int send_num;
    struct sockaddr_in client_addr;
    int len=sizeof(client_addr);
    while(true){
        cout<<"waiting for data"<<endl;
        recv_num=recvfrom(Udp_socket,recv_buf,sizeof(recv_buf)-1,0,(struct sockaddr*)&client_addr,(socklen_t *)&len);
        if(recv_num<0)
        {
            cout<<"receive ERROR"<<endl;
            exit(1);
        }
        recv_buf[recv_num]='\0';
        cout<<"Receive "<<recv_num<<"bytes: "<<recv_buf<<endl;
        for(int i=0;i<recv_num;i++){
            if(recv_buf[i]>='a'&&recv_buf[i]<='z')
            send_buf[i]=toupper(recv_buf[i]);
        }
        send_buf[recv_num]='\0';
        send_num=sendto(Udp_socket,send_buf,recv_num,0,(struct sockaddr*)&client_addr,len);
        if(send_num<0)
        {
            cout<<"send ERROR"<<endl;
            exit(1);
        }
    }
    close(Udp_socket);
    return 0;
}