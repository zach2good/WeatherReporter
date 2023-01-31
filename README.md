# WeatherReporter

## Client

`/client`

Download the pre-compiled plugin dll [here](https://github.com/zach2good/WeatherReporter/raw/main/client/Release/WeatherReporter.dll).

An Ashita-v4 C++ plugin which spins up a background thread and sends a HTTP `PUT` request to the server with the following data:

- Zone number
- Weather type
- UTC timestamp

In the format: `zonenumber,weathertype,timestamp`

### Usage

- Copy in compiled DLL to `<ashita>/plugins/`
- Either:
  - Add `/load WeatherReporter` to the bottom of your `<ashita>/scripts/default.txt` file
  - In game: `/load WeatherReporter`

## Server

`/server`

A simple Python Flask application which takes `PUT` and `POST` requests, parses them, and slaps them into a local SQLite file.
It has WAL mode enabled to place nicely with Flask's threading.

## Hosting

I currently host a small machine in GCP which runs the server process. `PUT` and `POST` requests can be sent to: `http://35.209.198.215/weather`

### Useful commands for the server

```sh
# Start
screen sudo python3 server.py

# Reattach
screen -ls
screen -r <session-id>

# Kill id
screen -XS <session-id> quit

# Print 10 most recent weather data entries
sqlite3 WeatherReporter/db.sqlite "SELECT *, DATETIME(timestamp, 'unixepoch') FROM weather_data ORDER BY timestamp DESC LIMIT 10;"
```
