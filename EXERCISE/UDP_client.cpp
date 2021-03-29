#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

using namespace std;

int main()
{
    int Udp_socket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

    struct sockaddr_in sever_addr;
    memset(&sever_addr,0,sizeof(sever_addr));
    sever_addr.sin_family=AF_INET;
    sever_addr.sin_port=htons(1234);
    sever_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    int len=sizeof(sever_addr);
    char send_buf[50];
    char recv_buf[50];
    int send_num,recv_num;
    cout<<"please input string!"<<endl;
    cin>>send_buf;

    send_num=sendto(Udp_socket,send_buf,strlen(send_buf),0,(struct sockaddr*)&sever_addr,len);
    if(send_num<0){
        cout<<"send ERROR"<<endl;
        exit(1);
    }
    recv_num=recvfrom(Udp_socket,recv_buf,sizeof(recv_buf),0,(struct sockaddr*)&sever_addr,(socklen_t*)&len);
    if(recv_num<0){
        cout<<"receive ERROR"<<endl;
        exit(1);
    }
    recv_buf[recv_num]='\0';
    cout<<"Sever give back: "<<recv_buf<<endl;
    
    close(Udp_socket);
    return 0;
}