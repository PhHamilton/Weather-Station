import os, time
from datetime import datetime

class deviceData():
	def __init__(self, name, unit, filePath):
		self.__name 	= name
		self.__unit		= unit
		self.__filePath = filePath

	def GetName(self):
		return self.__name

	def GetData(self):
		with open(self.__filePath, 'r') as file:
			fileContent = file.read()
		return fileContent
	
	def GetUnit(self):
		return self.__unit

	def GetLastSeen(self):
		lastSeen = os.path.getmtime(self.__filePath)
		lastSeenDatetime = datetime.fromtimestamp(lastSeen)
		lastSeenFormatted = lastSeenDatetime.strftime('%d/%m/%Y %H:%M:%S')
		return lastSeenFormatted
