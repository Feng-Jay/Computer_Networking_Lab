mod server;
mod head;
use head::*;
use server::*;
use chat_room::*;

fn main()
{
    let server:Server = Server::new(LOCAL_HOST);
    let listener = server.listen();

    // Listen IP and response
    server.run(listener);
}     