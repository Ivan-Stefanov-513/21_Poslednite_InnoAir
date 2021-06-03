use clap::ArgMatches;
use std::error::Error;
use threadpool::ThreadPool;
use std::fs::File;
use std::io::{Read};
use native_tls::{Identity, TlsAcceptor, TlsStream};
use std::net::{TcpListener, TcpStream};
use tungstenite::{server, Message};
use tungstenite::protocol::WebSocket;
use thiserror::Error;
use mysql::PooledConn;
use std::sync::Arc;
use std::convert::TryFrom;
use byteorder::{ByteOrder, LittleEndian};
use mysql::chrono::ParseResult;

const WEBSOCKET_BIND: &str = "10.21.42.2:1234";
const DATABASE_ACCESS: &str = "mysql://poslednite:drenki_gl0g@localhost:3306/poslednite";
const CERT_FILENAME: &str = "Poslednite_21.pkcs12";
const CERT_PASS: &str = "drenki_gl0g";

const MESSAGE_T_SIZE: u8 = 7;

pub struct Config;

impl Config
{
	// TODO: Add parameters
	pub fn new(arguments: ArgMatches) -> Config
	{
		Config
	}
}

// This thing might not be needed after all
#[derive(Error, Debug)]
pub enum RuntimeError
{
	#[error("failed to bind to server socket")]
	WebsocketBindError,
	#[error("unknown error")]
	UnknownError,
}

pub enum SensorMessageData
{
	Humidity(uint32_t),
	Temperature(int32_t),
	Pressure(uint32_t),
	Noise(uint32_t),
	Dust(uint32_t),
	AirQuality(uint32_t),
	CO2(uint32_t)
}

pub struct SensorMessage
{
	data_and_type: SensorMessageData,
	station_id: u16
}

impl TryFrom<&[u8]> for SensorMessage
{
	type Error = String;

	fn try_from(input: &[u8]) -> Result<Self, Self::Error>
	{
		if input.len() == MESSAGE_T_SIZE
		{
			Ok(SensorMessage
			{
				data_and_type: match input[0]
				{
					1 => SensorMessageData::Humidity(LittleEndian::read_u32(&input[1..5])),
					2 => SensorMessageData::Temperature(LittleEndian::read_i32(&input[1..5])),
					3 => SensorMessageData::Pressure(LittleEndian::read_u32(&input[1..5])),
					4 => SensorMessageData::Noise(LittleEndian::read_u32(&input[1..5])),
					5 => SensorMessageData::Dust(LittleEndian::read_u32(&input[1..5])),
					6 => SensorMessageData::AirQuality(LittleEndian::read_u32(&input[1..5])),
					7 => SensorMessageData::CO2(LittleEndian::read_u32(&input[1..5])),
					_ => return Err(String::from("invalid message type! Received {}, expected a value between 1 and 7 inclusive.", input[0]))
				},
				station_id: LittleEndian::read_u16(&input[5..7])
			})
		}
		else
		{
			Err(format!("incorrect size of input when converting from Vec<u8> to SensorMessage! Espected {}, provided {}.",
			            MESSAGE_T_SIZE, input.len()))
		}
	}
}

pub fn run(config: Config) -> Result<(), Box<dyn Error>>
{
	let mut cert_file = File::open(CERT_FILENAME)?;
	let mut pkcs12 = vec![];
	cert_file.read_to_end(&mut pkcs12)?;
	let pkcs12_identity = Identity::from_pkcs12(&pkcs12, CERT_PASS)?;

	let websocket_server = TcpListener::bind(WEBSOCKET_BIND)?;
	let acceptor = TlsAcceptor::new(pkcs12_identity)?;
	let acceptor = Arc::new(acceptor);

	let pool = mysql::Pool::new(DATABASE_ACCESS)?;
	let n_worker_threads = 4;
	let thread_pool = ThreadPool::new(n_worker_threads);

	for client_stream in websocket_server.incoming()
	{
		// TODO: Refactor this, maybe move more stuff into the worker threads
		if let Ok(client_stream) = client_stream
		{
			let acceptor_clone = acceptor.clone();
			if let Ok(client_stream) = acceptor_clone.accept(client_stream)
			{
				if let Ok(mut client_socket) = server::accept(client_stream)
				{
					let mut db_conn = pool.get_conn()?;
					thread_pool.execute(move ||
					{
						// TODO: Handle errors, possibly by sending result to the main thread?
						handle_client(client_socket, db_conn);
					});
				};
			};
		};
	}

	Ok(())
}

pub fn handle_client(mut client_socket: WebSocket<TlsStream<TcpStream>>, mut db_conn: PooledConn) -> Result<(), Box<dyn Error>>
{
	//println!("DEI, RABOTI!");
	db_conn.query("INSERT INTO stations (name, location) VALUES ('test', 'here')")?;

	let msg = client_socket.read_message()?;
	match msg
	{
		Message::Binary(msg_data) =>
		{
			let decoded_msg = SensorMessage::try_from(&msg_data[..])?;
			
		},
		_ => ()
	}

	Ok(())
}