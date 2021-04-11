use std::net::UdpSocket;
use std::{io,str};

fn main()->std::io::Result<()> {
    let socket=UdpSocket::bind("127.0.0.1:1234")?;
    
    loop{
    println!("Input THE String");
    let mut s=String::new();
    io::stdin().read_line(&mut s)?;

    socket.send_to(s.as_bytes(), "127.0.0.1:1235")?;
    let mut buf=[0u8;30];
    socket.recv_from(&mut buf)?;
    println!("recv:{}",str::from_utf8(&buf).expect("no str"));
    }
}
