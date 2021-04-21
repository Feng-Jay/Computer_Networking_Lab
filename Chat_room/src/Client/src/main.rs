use std::net::TcpStream;
use std::thread;
use std::time::Duration;
use std::str;
use std::io::{ErrorKind,Read,Write,self};
use std::sync::mpsc::{self,TryRecvError};

const SERVER_ADDR:&str="127.0.0.1:1234";
const MESSAGE_SIZE:usize=1024;

fn main()
{
    let mut client= TcpStream::connect(SERVER_ADDR).expect("Fail to connect the server!");
    client.set_nonblocking(true).expect("Fail to set nonblocking!");
    let (sender,receiver)=mpsc::channel::<String>();
    
    std::thread::spawn(move || loop{
        let mut buf = vec![0;MESSAGE_SIZE];
        match client.read_exact(&mut buf){
            Ok(_)=>{
                let message= buf.into_iter().take_while(|&x| x!=0).collect::<Vec<_>>();
                let message= str::from_utf8(&message).unwrap();
                println!("Message: {}",message);
            },
            Err(ref err) if err.kind() == ErrorKind::WouldBlock=>(),
            Err(_)=>{
                println!("Connection is down!");
                break;
            }
        }
        match receiver.try_recv(){
            Ok(message)=>{
                let mut buf =message.clone().into_bytes();
                buf.resize(MESSAGE_SIZE,0);
                client.write_all(&buf).expect("Fail to write to socket!");
            },
            Err(TryRecvError::Empty)=>(),
            Err(TryRecvError::Disconnected)=>break
        }
        thread::sleep(Duration::from_millis(100));
    });

    println!("Welcome to FFengJay's chatroom!");

    loop{
        let mut buf=String::new();
        io::stdin().read_line(&mut buf).expect("Fail to read from keyboard!");
        let message=buf.trim().to_string();
        if message== "exit" || sender.send(message).is_err()
        {
            break
        }
    }
    println!("Welcome back!");
}