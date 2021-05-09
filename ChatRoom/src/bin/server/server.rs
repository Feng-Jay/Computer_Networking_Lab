use std::net::TcpListener;
use std::net::SocketAddr;
use std::sync::mpsc;
use std::io::{ErrorKind,Read,Write};
use std::thread;

use crate::head::*;
use chat_room::{Protocol,MESSAGE_SIZE,LOCAL_HOST};

pub struct Server{
    pub addr:&'static str
}

impl Server{
    pub fn new(add:&'static str)->Self{
        Self{
            addr:add
        }
    }

    pub fn listen(&self)->TcpListener{
        let listener=TcpListener::bind(self.addr).expect("Fail to bind Server addr!");
        listener.set_nonblocking(true).expect("Fail to set noblocking!");
        listener
    }

    fn sleep(){
        thread::sleep(::std::time::Duration::from_millis(100));
    }

    fn build_message(buf:Vec<u8>,addr:SocketAddr)->(String,Protocol){
        let message=buf.into_iter().take_while(|&x| x!=0).collect::<Vec<_>>();
        let mut message=String::from_utf8(message).expect("Fail to change to utf8 format!");

        let protocol=select_protocol(&mut message);
        let message=format!(
            "{}: {}",
            addr,
            message
        );
        (message,protocol)
    }

    pub fn run(&self, listener:TcpListener){
        let mut clients=vec![];
        let (sender,recv)=mpsc::channel::<Vec<u8>>();
        loop{
            if let Ok((mut socket,address))=listener.accept(){
                println!("Client {} connected", address);
                let sender=sender.clone();
                clients.push(socket.try_clone().expect("Fail to clone client"));
                    thread::spawn(move || loop{
                    let mut buf=vec![0;MESSAGE_SIZE];
                    match socket.read_exact(&mut buf){
                        Ok(_)=>{
                            let copy=buf.clone();
                            let (message, protocol)=Server::build_message(buf, address);
                            let bytes=message.clone().into_bytes();
                            sender.send(bytes).expect("Fail to send message to recv!");
                            match protocol{
                                Protocol::VTP=>{
                                    let mut sound=vec![0;MESSAGE_SIZE];
                                    socket.read_exact(&mut sound).expect("Fail to receive sound!");
                                    sender.send(sound).expect("Fail to send sound to recv!");
                                },
                                Protocol::FTP=>{
                                    sender.send(copy).expect("Fail to send file to recv!");
                                },
                                _ =>{}
                            }
                        },
                        Err(ref err) 
                        if err.kind()==ErrorKind::WouldBlock=>(),
                        Err(_)=>{
                            println!("Usr {} offline!", address);
                            break;
                        }
                    }
                    Server::sleep();
                });
            }
            if let Ok(message)=recv.try_recv(){
                clients=clients.into_iter().filter_map(|mut client|
                    {
                        let mut buf =message.clone();
                        buf.resize(MESSAGE_SIZE,0);
                        client.write_all(&buf).map(|_| client).ok()
                    }
                ).collect::<Vec<_>>();
            }
            Server::sleep();
        }
    }
}