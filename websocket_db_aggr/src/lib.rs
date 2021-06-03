use clap::ArgMatches;
use std::error::Error;
use threadpool::ThreadPool;
use std::sync::mpsc::channel;
use std::net::{TcpListener, TcpStream};
use tungstenite::server::accept;
use tungstenite::protocol::WebSocket;
use thiserror::Error;
use mysql::PooledConn;

const WEBSOCKET_BIND: &str = "10.21.42.2:1234";
const DATABASE_ACCESS: &str = "mysql://poslednite:drenki_gl0g@localhost:3306/poslednite";

pub struct Config;

impl Config
{
	// TODO: Add parameters
	pub fn new(arguments: ArgMatches) -> Config
	{
		Config
	}
}

#[derive(Error, Debug)]
pub enum RuntimeError
{
	#[error("failed to bind to server socket")]
	WebsocketBindError,
	#[error("unknown error")]
	UnknownError,
}

pub fn run(config: Config) -> Result<(), Box<dyn Error>>
{
	let websocket_server = TcpListener::bind(WEBSOCKET_BIND)?;
	let pool = mysql::Pool::new(DATABASE_ACCESS)?;
	let n_worker_threads = 4;
	let thread_pool = ThreadPool::new(n_worker_threads);

	for client_stream in websocket_server.incoming()
	{
		if let Ok(client_stream) = client_stream
		{
			if let Ok(mut client_socket) = accept(client_stream)
			{
				let mut db_conn = pool.get_conn()?;
				thread_pool.execute(move||
				{
					// TODO: Handle errors, possibly by sending result to the main thread?
					handle_client(client_socket, db_conn);
				});
			};
		};
	}

	Ok(())
}

pub fn handle_client(mut client_socket: WebSocket<TcpStream>, mut db_conn: PooledConn) -> Result<(), Box<dyn Error>>
{
	println!("RAZGELE!");
	Ok(())
}