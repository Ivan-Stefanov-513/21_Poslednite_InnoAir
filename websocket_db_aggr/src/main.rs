use websocket_db_aggr::Config;
use clap::{Arg, App};
use std::process;

fn main()
{
	// TODO: Add parameters
	let arg_matches = App::new("websocket_db_aggr")
		.version("0.1.0")
		.author("Vladimir Garistov <vl.garistov@gmail.com>")
		.about("Multithreaded server for aggregating data from multiple secure websocket connections into MySQL/MariaDB database")
		.get_matches();

	let config = Config::new(arg_matches);

	if let Err(error) = websocket_db_aggr::run(config)
	{
		eprintln!("Program stopped due to error: {}\n", error);
		drop(error);
		process::exit(1);
	}
}
