use std::net::TcpStream;
use std::io::{self, ErrorKind,Read,Write};
use std::sync::mpsc::{self,TryRecvError};
use std::time::Duration;
use std::thread;
use std::str;
use super::*;
use audio::{Audio, SAMPLE_RATE};
use chat_room::Protocol;

pub struct Client{
    addr:&'static str
}
impl Client{
    pub fn new(add:&'static str)->Self{
        Self{
            addr:add
        }
    }

    pub fn connect(&self)->TcpStream{
        let client=TcpStream::connect(self.addr).expect("Fail to connect the server!");
        client.set_nonblocking(true).expect("Fail to set nonblocking!");
        client
    }

    pub fn select_protocol(message:&str)->Protocol{
        if message.find("VTP")!=None{
            Protocol::VTP
        }else if message.find("FTP")!=None{
            Protocol::FTP
        }else{
            Protocol::MTP
        }
    }

    pub fn sleep(){
        thread::sleep(::std::time::Duration::from_millis(100));
    }
    pub fn main_sleep(){
        thread::sleep(::std::time::Duration::from_secs(1));
    }

    pub fn run(&self, mut client:TcpStream){
        let (sender,recv)=mpsc::channel::<Vec<u8>>();

            thread::spawn(move || loop{
            let mut buf =vec![0;MESSAGE_SIZE];
            
            match client.read_exact(&mut buf){
                Ok(_)=>{
                    let message=buf.into_iter().take_while(|&x| x!=0).collect::<Vec<_>>();
                    let message=str::from_utf8(&message).unwrap();
                    match Client::select_protocol(message){
                        Protocol::MTP=>{
                            println!("Message: {}", message);
                        },
                        Protocol::FTP=>{
                            download(message);
                        },
                        Protocol::VTP=>{
                            let mut sound=vec![0;MESSAGE_SIZE];
                            client.read_exact(&mut sound).expect("Fail to gather sound!");
                            let pcm=Audio::new_playback();
                            Audio::set_hw(&pcm);
                            let sound=Audio::u8_to_i16(&sound[..]);
                            Audio::play(&pcm,sound);
                            Client::sleep();
                            println!("Playback done!");
                        },
                        _=>{}
                    }
                },
                Err(ref err) if err.kind()==ErrorKind::WouldBlock=>(),
                Err(_)=>{
                    println!("Fail to connect!");
                    break;
                }
            }
            match recv.try_recv(){
                Ok(message)=>{
                    let mut buf =message.clone();
                    buf.resize(MESSAGE_SIZE,0);
                    client.write_all(&buf).expect("Fail to write to socket!");
                },
                Err(TryRecvError::Empty)=>(),
                Err(TryRecvError::Disconnected)=>break
            }
            thread::sleep(Duration::from_millis(100));
        });
println!("Welcome to chatroom!");
        loop{
            let mut buf=String::new();
       
            io::stdin().read_line(&mut buf).expect("Fail to read input!");
            let mut message=buf.trim().to_string();

            if let Some(sound)=select_protocol(&mut message){
                let secs=sound.len()/(SAMPLE_RATE*2);
                let bytes=message.clone().into_bytes();
                if sender.send(bytes).is_err(){break}
                if sender.send(sound).is_err(){
                    break
                }
                thread::sleep(::std::time::Duration::from_secs((secs+5)as u64));
            }else {
                let bytes=message.clone().into_bytes();
                if message=="exit"|| sender.send(bytes).is_err(){
                    break
                }
            }
            Client::main_sleep();
        }
        println!("Goodbye!");
    }
}
