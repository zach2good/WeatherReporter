# WeatherReporter

## Client

`/client/ashita` and `/client/windower/`

Ashita-v4 and Windower-v4 plugins which non-intrusively collects and sends weather data to the cloud when you change zones or when the weather changes. The data sent to the cloud is:

- Zone number
- Weather type
- UTC timestamp

In the format: `zonenumber,weathertype,timestamp`

### Usage (Ashita-v4)

Download the pre-compiled plugins here: [here](https://github.com/zach2good/WeatherReporter/raw/main/client/bin/Ashita/WeatherReporter.dll).

- Copy in compiled DLL to `<ashita>/plugins/`
- Add `/load WeatherReporter` to the bottom of your `<ashita>/scripts/default.txt` file

### Usage (Windower-v4)

Download the contents of the addon folder here: [here](https://github.com/zach2good/WeatherReporter/raw/main/client/bin/Windower/).

- Place the downloaded files in your addons folder:

```txt
Windower/
    addons/
        WeatherReporter/
            lib/
                _WeatherReporter.dll
            WeatherReporter.lua
```

- Add `lua load WeatherReporter` to the bottom of your `<windower>/scripts/init.txt` file

### NOTES

- The pre-compiled DLL may be flagged by your antivirus. This is a false positive, sorry! If you're unsure about the DLL, feel free to poke around in the code and to build it locally.
- Nothing bad will happen if you use this (or leave it on) while playing on private servers, the plugin will just be idle and not collect or send any information.

### Thanks

- atom0s([Ashita-v4beta](https://github.com/AshitaXI/Ashita-v4beta), [ExamplePlugin](https://github.com/AshitaXI/ExamplePlugin))
- Heals ([XIPivot](https://github.com/Shirk/XIPivot))
- Windower Team ([Website](https://www.windower.net/), [GitHub](https://github.com/Windower))

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
```

## TODO

- Make the server address and endpoints configurable
- Collect more types of information (events, event params, Mob/NPC data, etc.)
