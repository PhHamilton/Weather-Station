from flask import Flask, jsonify, Response
from deviceData import deviceData
from jsonHandler import jsonHandler

JsonHandler = jsonHandler()
temperatureSensor = deviceData("BMP280", "C", "/home/ares/testfile.txt")
pressureSensor = deviceData("BMP280", "Pa", "/home/ares/testfile.txt")
windSpeedSensor = deviceData("DP Wind Speed Sensor", "m/s", "/home/ares/testfile.txt")
windDirectionSensor = deviceData("Dp Wind Direction Sensor", "unknown", "/home/ares/testfile.txt")

PORT = 5000

app = Flask(__name__)

@app.route('/')
def index():
	return 'Hello World'

@app.route('/get_data')
def get_sensor_data():
	sensorData = [
					{'name':'Temperature',	'value':1},
					{'name':'Humidity',		'value':1},
					{'name':'Windsensor',	'value':1},
					{'name':'WindDirection','value':1}
				 ]

	status = 1

	return jsonify({'SensorData':sensorData, 'status':status})

@app.route('/get_temperature_data')
def get_temperature_data():
	return jsonify(jsonHandler.composeData(temperatureSensor))

@app.route('/get_pressure_data')
def get_pressure_data():
	return jsonify(jsonHandler.composeData(pressureSensor))

@app.route('/get_wind_speed_data')
def get_wind_speed_data():
	return jsonify(jsonHandler.composeData(windSpeedSensor))

@app.route('/get_wind_direction_data')
def get_wind_direction_data():
	return jsonify(jsonHandler.composeData(windDirectionSensor))

@app.route('/get_device_data')
def get_device_data():
	return jsonify(jsonHandler.composeData(climateSensor))

if __name__ == '__main__':
	app.run(debug=True, host='0.0.0.0', port=PORT)
