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
use mysql::prelude::*;

const WEBSOCKET_BIND: &str = "10.21.42.2:443";
const DATABASE_ACCESS: &str = "mysql://poslednite:drenki_gl0g@localhost:3306/poslednite";
const CERT_FILENAME: &str = "/home/cartogan/Programs/21_Poslednite_InnoAir/websocket_db_aggr/Poslednite_21.pkcs12";
const CERT_PASS: &str = "drenki_gl0g";

const MESSAGE_T_SIZE: usize = 7;

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
	Humidity(u32),
	Temperature(i32),
	Pressure(u32),
	Noise(u32),
	Dust(u32),
	AirQuality(u32),
	CO2(u32)
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
					_ => return Err(format!("invalid message type! Received {}, expected a value between 1 and 7 inclusive.", input[0]))
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
				if let Ok(client_socket) = server::accept(client_stream)
				{
					let db_conn = pool.get_conn()?;
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
	//db_conn.query("INSERT INTO stations (name, location) VALUES ('test', 'here')")?;

	let msg = client_socket.read_message()?;
	client_socket.close(None);
	client_socket.write_pending()?;
	match msg
	{
		Message::Binary(msg_data) =>
		{
			let decoded_msg = SensorMessage::try_from(&msg_data[..])?;
			match decoded_msg.data_and_type
			{
				SensorMessageData::Humidity(value) =>
				{
					let res:Vec<String> = db_conn.exec("INSERT INTO humidity (station_id, value) VALUES (?, ?)", (decoded_msg.station_id, value))?;
				},
				SensorMessageData::Temperature(value) =>
				{
					let res:Vec<String> = db_conn.exec("INSERT INTO temperature (station_id, value) VALUES (?, ?)", (decoded_msg.station_id, value))?;
				},
				SensorMessageData::Pressure(value) =>
				{
					let res:Vec<String> = db_conn.exec("INSERT INTO pressure (station_id, value) VALUES (?, ?)", (decoded_msg.station_id, value))?;
				},
				SensorMessageData::Noise(value) =>
				{
					let res:Vec<String> = db_conn.exec("INSERT INTO noise (station_id, value) VALUES (?, ?)", (decoded_msg.station_id, value))?;
				},
				SensorMessageData::Dust(value) =>
				{
					let res:Vec<String> = db_conn.exec("INSERT INTO dust (station_id, value) VALUES (?, ?)", (decoded_msg.station_id, value))?;
				},
				SensorMessageData::AirQuality(value) =>
				{
					let res:Vec<String> = db_conn.exec("INSERT INTO air_quality (station_id, value) VALUES (?, ?)", (decoded_msg.station_id, value))?;
				},
				SensorMessageData::CO2(value) =>
				{
					let res:Vec<String> = db_conn.exec("INSERT INTO co2 (station_id, value) VALUES (?, ?)", (decoded_msg.station_id, value))?;
				},
			}
		},
		_ => ()
	}

	Ok(())
}