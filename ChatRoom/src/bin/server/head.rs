use std::fs;
use std::io::prelude::*;
use chat_room::{
    MTP,FTP,VTP,FILE_SUCCESS, FILE_FAIL_RESPONSE, FILE_SUCCESS_RESPONSE, Protocol, rename_filename 
};

pub fn select_protocol(message: &mut String) -> Protocol {
    println!("{}", message);
    if message.as_str().starts_with(MTP){
        mtp_slave(message);                      
        Protocol::MTP
    }else if message.as_str().starts_with(FTP){
        ftp_slave(message);
        Protocol::FTP
    }else if message.as_str().starts_with(VTP){
        vtp_slave(message);
        Protocol::VTP
    }else if message == "exit"{
        *message = String::from("exit");
        Protocol::Other
    }else{
        panic!("Error Protocol!");
    }
}


pub fn mtp_slave(message: &mut String){
    let s:Vec<&str> = message.split(": ").collect();
    *message = String::from(s[1]);
}

pub fn ftp_slave(message: &mut String){
    // Debug
    // println!("message: {}", message);
    let s:Vec<&str> = message.split(": ").collect();

    let name:String = String::from(s[1]);

    let s2:Vec<&str> = name.split("/").collect();
    let mut filename:String = String::from("server_files/");
    filename.push_str(s2[1]);
    let contents = s[2];

    // solve filename conflict
    rename_filename(&mut filename, 0);

    // let mut file = fs::File::create(filename).expect(FILE_FAIL);
    // let mut file;
    let copy_filename = filename.clone();
    match fs::File::create(filename) {
        Ok(handler) => {
            let mut file = handler;
            if file.write_all(contents.as_bytes()).is_err(){
                println!("{}", FILE_FAIL_RESPONSE);
            }
        
            println!("{}", FILE_SUCCESS);
            *message = format!(
                "{}: {}",
                copy_filename, FILE_SUCCESS_RESPONSE
            );
        }
        Err(err) => {
            println!("error: {}", err);
            *message = format!(
                "{}: {}",
                copy_filename, FILE_FAIL_RESPONSE
            );
        }
    }

}

pub fn vtp_slave(message: &mut String) {
    *message = String::from("VTP");
}