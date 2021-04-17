use std::error::Error;
use std::thread;
use std::io::{ErrorKind,Read,Write};
use std::net::TcpListener;
use std::sync::mpsc;

const SERVER_ADDR:&str="127.0.0.1:1234";
const MESSAGE_LEN:usize=128;
fn sleep() 
{
    thread::sleep(::std::time::Duration::from_millis(100));
} 

fn main()->Result<(),Box<dyn Error>>{
    let listener= TcpListener::bind(SERVER_ADDR)?;
    listener.set_nonblocking(true)?;

    let mut clients_array=vec![];
    let (sender,receiver)=mpsc::channel::<String>();
    loop{
        if let Ok((mut socket, address)) =listener.accept(){
            println!("usr {} connect",address);
            let sender=sender.clone();
            clients_array.push(socket.try_clone().expect("fail to clone a client to array!"));
            std::thread::spawn(move || loop{
                let mut buf=vec![0;MESSAGE_LEN];
                match socket.read_exact(&mut buf){
                    Ok(_)=>{
                        let message=buf.into_iter().take_while(|&x| x!=0).collect::<Vec<_>>();
                        let message=String::from_utf8(message).expect("fail to load utf8 format message");
                        println!("{}:{:?}",address,message);
                        sender.send(message).expect("fail to send message!");
                    },
                    Err(ref err) if err.kind()==ErrorKind::WouldBlock=>(),
                    Err(_)=>{
                        println!("usr {} logout!",address);
                        break;
                    }
                }
                sleep();
            });
        }
        if let Ok(message) = receiver.try_recv() {
            let message_copy=message.clone();
            println!("message [{}] is received.", message_copy);

            clients_array=clients_array.into_iter().filter_map(|mut client|{
                let mut buf =message.clone().into_bytes();
                buf.resize(MESSAGE_LEN,0);
                client.write_all(&buf).map(|_| client).ok()
            }).collect::<Vec<_>>();
        }
        sleep();

    }

}
