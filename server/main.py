from flask import Flask, request
import sqlite3


app = Flask(__name__)


def execute(query):
    connection = sqlite3.connect("db.sqlite", isolation_level=None)
    connection.execute('pragma journal_mode=wal')
    cursor = connection.cursor()
    cursor.execute(query)
    cursor.close()


def insert_weather(zone, weather, timestamp):
    execute(f"INSERT INTO weather_data VALUES ({zone}, {weather}, {timestamp})")


@app.route("/weather", methods=["PUT","POST"])
def weather_put_post_handler():
    data = request.data.decode("utf-8")
    parts = data.split(",")

    zone = int(parts[0])
    weather = int(parts[1])
    timestamp = int(parts[2])

    insert_weather(zone, weather, timestamp)

    return "WeatherReporter::Success", 200


@app.route("/weather", methods=["GET"])
def weather_get_handler():
    return "WeatherReporter::Success", 200


@app.route("/", methods=["GET"])
def main_get_handler():
    return "Success", 200


if __name__ == "__main__":
    execute("CREATE TABLE IF NOT EXISTS weather_data (zone SMALLINT, weather TINYINT, timestamp BIGINT)")
    app.run(port=80, host="0.0.0.0")
