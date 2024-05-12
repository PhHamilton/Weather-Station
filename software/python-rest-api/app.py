from flask import Flask, jsonify, Response
#import json

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

if __name__ == '__main__':
	app.run(debug=True, host='0.0.0.0', port=PORT)
