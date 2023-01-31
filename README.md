# WeatherReporter

## Client

`/client`

An Ashita-v4 C++ plugin which non-intrusively collects and sends weather data to the cloud when you change zones or when the weather changes. The data sent to the cloud is:

- Zone number
- Weather type
- UTC timestamp

In the format: `zonenumber,weathertype,timestamp`

Download the pre-compiled plugin dll [here](https://github.com/zach2good/WeatherReporter/raw/main/client/Release/WeatherReporter.dll).

### Usage

- Copy in compiled DLL to `<ashita>/plugins/`
- Either:
  - Add `/load WeatherReporter` to the bottom of your `<ashita>/scripts/default.txt` file
  - In game: `/load WeatherReporter`

### NOTES

- The pre-compiled DLL may be flagged by your antivirus. This is a false positive, sorry! If you're unsure about the DLL, feel free to poke around in the code and to build it locally.
- Nothing bad will happen if you use this (or leave it on) while playing on private servers, the plugin will just be idle and not collect or send any information.

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

### What's on the server?

Not much!

```sh
>>>:~$ ls -l WeatherReporter/
total 44
-rw-r--r-- 1 root      root       8192 Jan 31 13:42 db.sqlite
-rw-r--r-- 1 zrtoogood zrtoogood 32768 Jan 31 14:04 db.sqlite-shm
-rw-r--r-- 1 zrtoogood zrtoogood     0 Jan 31 13:51 db.sqlite-wal
-rw-r--r-- 1 zrtoogood zrtoogood  1112 Jan 29 00:22 server.py

>>>:~$ sqlite3 WeatherReporter/db.sqlite "SELECT *, DATETIME(timestamp, 'unixepoch') FROM weather_data ORDER BY timestamp ASC LIMIT 10;"
235|1|1674958474|2023-01-29 02:14:34
235|0|1674959304|2023-01-29 02:28:24
107|0|1674959375|2023-01-29 02:29:35
235|0|1674959447|2023-01-29 02:30:47
107|0|1674959470|2023-01-29 02:31:10
235|0|1674959500|2023-01-29 02:31:40
235|0|1675005229|2023-01-29 15:13:49
245|0|1675005274|2023-01-29 15:14:34
244|0|1675005441|2023-01-29 15:17:21
243|0|1675005461|2023-01-29 15:17:41

>>>:~$ 
```

## TODO

- Make the server address and endpoints configurable
- Collect more types of information (events, event params, Mob/NPC data, etc.)
