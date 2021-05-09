use std::path::Path;

#[derive(Clone,Copy,PartialEq,Debug)]
pub enum Protocol{
    MTP,
    FTP,
    VTP,
    Other
}
#[repr(C,packed)]
#[derive(Debug,Clone)]
pub struct Stream{
    pub protocol:Protocol,
    pub message: Vec<u8>,
    pub size: usize
}

impl Stream{
    pub unsafe fn serialize(&self)->Vec<u8>{
            ::std::slice::from_raw_parts(
            (self as *const Stream) as *const u8,
            ::std::mem::size_of::<Stream>(),
        ).to_vec()
    }

    pub unsafe fn deserialize(bytes:Vec<u8>)->Stream{
        let(head,body,_)=bytes.align_to::<Stream>();
        assert!(head.is_empty(),"deserialize Fail!");
        body[0].clone()
    }
}

pub fn rename_filename(filename: &mut String, mut num: usize){
    let mut clone_filename = filename.clone();
    while Path::new(&clone_filename).exists(){
        if filename.find(".") != None {
            num += 1;
            let s:Vec<&str> = filename.as_str().split(".").collect();

            let file = s[0];
            let extension = s[1];
            let mut file = String::from(file);
            file.push_str(num.to_string().as_str());
            file.push_str(".");
            file.push_str(extension);
            
            clone_filename = file;
        }
    }

    *filename = clone_filename;
}

pub const MTP:&'static str="MTP: ";
pub const FTP:&'static str="FTP: ";
pub const VTP:&'static str="VTP: ";

pub const FILE_SUCCESS: &'static str = "Succeed to download file!";
pub const FILE_SUCCESS_RESPONSE: &'static str = "Server has succeeded to receive and download file!";
pub const FILE_FAIL_RESPONSE: &'static str = "Server failed to download file!";


// Localhost with a port in it
pub const LOCAL_HOST: &str = "127.0.0.1:1234";

// The buffer size of messages
pub const MESSAGE_SIZE: usize = 1024 * 1024;