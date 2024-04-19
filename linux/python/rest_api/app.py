from flask import Flask, jsonify, Response
from deviceData import deviceData
from jsonHandler import jsonHandler

JsonHandler = jsonHandler()
climateSensor = deviceData("BMP280", "/home/ares/testfile.txt")

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

@app.route('/get_climate_data')
def get_climate_data():
	return jsonify(jsonHandler.composeData(climateSensor))

@app.route('/get_wind_data')
def get_wind_data():
	pass
	
@app.route('/get_device_data')
def get_device_data():
	pass

if __name__ == '__main__':
	app.run(debug=True, host='0.0.0.0', port=PORT)
