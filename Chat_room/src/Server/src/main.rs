use std::net::TcpListener;
use std::sync::mpsc;
use std::thread;
use std::io::{ErrorKind,Read,Write};

const SERVER_ADDR:&str = "127.0.0.1:1234";
const MESSAGE_SIZE:usize=1024;

fn main()
{
    let listener=TcpListener::bind(SERVER_ADDR).expect("Fail to bind socket!");
    listener.set_nonblocking(true).expect("Fail to set no blocking!");
    let mut clients=vec![];// 存储接入的用户
    let (sender,receiver)=mpsc::channel::<String>();//使用RUST的内置管道
    loop{
        if let Ok((mut socket,address))=listener.accept(){
            clients.push(socket.try_clone().expect("Fail to insert socket to vector!"));
            println!("Usr {} log in",address);
            let sender=sender.clone();

            thread::spawn(move || loop{
            
                let mut buf=vec![0;MESSAGE_SIZE];//缓冲区
                match socket.read_exact(&mut buf){//从TCP流中读数据
                    Ok(_)=>{
                        let message=buf.into_iter().take_while(|&x| x!=0).collect::<Vec<_>>();
                        //使用迭代器读取流数据
                        let message=String::from_utf8(message).expect("Fail to give utf8 message!");
                        println!("Accept {}'s message: {}",address,message);
                        sender.send(message).expect("Fail to send message to receiver!");
                    }
                    Err(ref err) if err.kind()==ErrorKind::WouldBlock=>(),//若阻塞则match到Err结果
                    Err(_)=>{//处理错误
                        println!("{}'s connection down",address);
                        break;
                    }
                }
                thread::sleep(::std::time::Duration::from_millis(100));
            });

        }

        if let Ok(message)=receiver.try_recv(){//从消息队列中得到信息
            clients=clients.into_iter().filter_map(|mut client|
            {
                let mut buf=message.clone().into_bytes();
                buf.resize(MESSAGE_SIZE,0);
                client.write_all(&buf).map(|_| client).ok()
            }).collect::<Vec<_>>();
        }

         thread::sleep(::std::time::Duration::from_millis(100));
    }


}