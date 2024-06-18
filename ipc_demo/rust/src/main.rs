extern crate systemstat;
use std::sync::mpsc::channel;
use std::thread;
use std::time::Duration;

use std::io::prelude::*;
use std::os::unix::net::UnixStream;

use systemstat::{Platform, System};

fn main() {
    let sys = System::new();
    let mut stream = UnixStream::connect("/Users/jamesfrancis/projects/system-design-guild/ipc_demo/c/unix_socket5.sock").unwrap();

    let (tx, rx) = channel();

    thread::spawn(move || loop {
        thread::sleep(Duration::from_secs(3));

        match sys.load_average() {
            Ok(load_average) => {
                println!("sending {:?}", load_average);
                tx.send(format!("Load average is: {:?}", load_average)).unwrap();
            }
            Err(x) => {
                println!("{:?}", x);
                tx.send("error".to_string()).unwrap();
            }
        };
    });

    loop {
        let _ = rx
            .try_recv()
            .map(|reply| stream.write_all(reply.as_bytes()));
    }
}
