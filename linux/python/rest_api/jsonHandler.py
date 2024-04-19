from flask import Flask, jsonify

class jsonHandler():
	def composeData(deviceData):
		sensorData = {
						'Name': deviceData.GetName(),
						'Value': deviceData.GetData(),
						'Unit': deviceData.GetUnit(),
						'LastSeen': deviceData.GetLastSeen(),
					 }

		return sensorData
