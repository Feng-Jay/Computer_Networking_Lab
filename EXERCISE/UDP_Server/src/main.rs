use std::net::UdpSocket;

fn main() ->std::io::Result<()>{

    let socket=UdpSocket::bind("127.0.0.1:1235")?;
    loop{
        let mut buf=[0u8;30];
        let (a,src)=socket.recv_from(&mut buf)?;

        let buf=&mut buf[..a];
        buf.reverse();
        socket.send_to(buf, &src)?;
    }

}
