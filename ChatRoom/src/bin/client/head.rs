use std::fs;
use std::io::prelude::*;
use crate::MESSAGE_SIZE;
use audio::{SAMPLE_RATE,Audio};
use std::str::FromStr;
use crate::client::Client;
use chat_room::{MTP,FTP,VTP,rename_filename};

pub fn select_protocol(message:&mut String)->Option<Vec<u8>>{
    if message.as_str().starts_with(MTP){
        mtp_slave(message);
        None
    }else if message.as_str().starts_with(FTP){
        ftp_slave(message);
        None
    }else if message.as_str().starts_with(VTP){
        let sound=vtp_slave(message);
        Some(sound)
    }else if message=="exit"{
        *message=String::from("exit");
        None
    }else{
        println!("message: {}", message);
        panic!("Error Protocol");
    }
}

pub fn mtp_slave(message:&mut String){
    let s:Vec<&str>=message.split(MTP).collect();
    *message=format!(
        "{}: {}","MTP",s[1]
    );
}

pub fn ftp_slave(message:&mut String){
    let s:Vec<&str>=message.split(FTP).collect();
    let filename:&str=s[1];
    let contents=fs::read_to_string(filename).expect("Fail to read file!");
    if contents.len()>MESSAGE_SIZE{
        *message=String::from("The file is beyond buffer!");
    }else {
        *message=format!(
            "{}: {}: {}","FTP",String::from(filename),contents
        );
    }
}

pub fn vtp_slave(message:&mut String)-> Vec<u8>{
    let s: Vec<&str>=message.split(' ').collect();
    let size=s[1];
    let size:usize=usize::from_str(size).expect("Fail to exchange to usize");
    println!("size: {}",size);

    Client::sleep();
    assert!(size<=10,"Sound is too long!");

    let buf_size=size*SAMPLE_RATE;
    let pcm=Audio::new_capture();
    Audio::set_hw(&pcm);
    println!("Please say!");
    let sound=Audio::capture(&pcm,buf_size);
    println!("Sound captured done!");
    *message=format!(
        "{}: {}s",
        "VTP",size
    );
    Audio::i16_to_u8(&sound[0..buf_size]).to_vec()
}

pub fn download(message:&str){
    let message=String::from(message);
    let s:Vec<&str>=message.split(":").collect();
    let name:String=String::from(s[1]);
    let s2:Vec<&str>=name.split("/").collect();
    let mut filename:String=String::from("client_files/");
    filename.push_str(s2[1]);
    let contents=s[2];
    rename_filename(&mut filename,0);
    let copy=filename.clone();

    match fs::File::create(filename){
        Ok(head)=>{
            let mut file=head;
            if file.write_all(contents.as_bytes()).is_err(){
                println!("Fail to download file {}", copy);
            }
            println!("Success to download file {}", copy);
        }
        Err(err) => {
            println!("error: {}", err);
        }
    }
}