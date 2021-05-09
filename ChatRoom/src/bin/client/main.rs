mod client;
mod head;
use head::*;
use client::*;
use chat_room::*;

use chat_room::{LOCAL_HOST,MESSAGE_SIZE};
fn main() 
{
    let client:Client = Client::new(LOCAL_HOST);
    loop {
        println!("Client connect");
        let connect = client.connect();
        client.run(connect);
        println!("Client exit current chatroom");
    }

}
