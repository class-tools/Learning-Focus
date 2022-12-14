'''
Learning Focus Main Source File 1.0.0
This source code file is under MIT License.
Copyright (c) 2022 Class Tools Develop Team
Contributors: ren-yc svvkii
'''
# Libraries
import os
import cv2
import sys
import dlib
import time
import numpy
import tkinter
import argparse
import threading
import face_recognition
import face_recognition_models

# Variables
CONFIG = None
PROGRAM_END = False
BASE_DIR = (os.path.dirname(sys.executable) if hasattr(sys, 'frozen') else os.path.dirname(__file__))
rawframe = None
processedframe = None
coveredframe = None

# Classes
class face:
	def __init__(self, currentface):
		self.status = 'green'
		self.face = currentface
		self.totgreen = 0.0
		self.totred = 0.0
		self.timestampround = time.time()
		self.timestampred = 0.0
		self.appeared = True
	def debug(self):
		print('Status: ' + self.status + ', Totgreen: ' + str(self.totgreen) + ', Totred: ' + str(self.totred) + ', Timestampround: ' + str(self.timestampround) + ', Timestampred: ' + str(self.timestampred) + ', Appeared: ' + str(self.appeared))

# Functions
def parse_command_line_arguments():
	global CONFIG
	parser = argparse.ArgumentParser(description = 'Learning Focus command line arguments.')
	parser.add_argument('--correction', type = float, default = 0.05, help = 'Correction threshold (Between 0 and 1) of error face measurement. (Default: 0.05)')
	parser.add_argument('--debug', action = 'store_true', help = 'Enable debug mode. (Default: Disable)')
	parser.add_argument('--delayface', type = float, default = 0.5, help = 'Delay of face detecting and recognizing loop. (Default: 0.5)')
	parser.add_argument('--delayshow', type = float, default = 0.5, help = 'Delay of frame showing loop. (Default: 0.5)')
	parser.add_argument('--device', type = int, default = 0, help = 'Device ID of the camera using. (Default: 0)')
	parser.add_argument('--noconsole', action = 'store_true', help = 'Hide the console window. (Default: Disable)')
	parser.add_argument('--tolerance', type = float, default = 0.5, help = 'Tolerance (Between 0 and 1) for face comparing. (Default: 0.5)')
	CONFIG = parser.parse_args()

def set_console_window_status():
	if os.name == 'nt' and CONFIG.noconsole == True:
		import ctypes
		hwnd = ctypes.windll.kernel32.GetConsoleWindow()
		if hwnd != 0:
			ctypes.windll.user32.ShowWindow(hwnd, 0)
			ctypes.windll.kernel32.CloseHandle(hwnd)

def get_class_face_array(input: list):
	result = []
	for i in range(len(input)):
		result.append(input[i].face)
	return result

def show_plot_image_data(input: list):
	stick = []
	l1 = []
	l2 = []
	for i in range(len(input)):
		stick.append('Face ' + str(i))
		l1.append(round(input[i].totgreen / 60, 2))
		l2.append(round(input[i].totred / 60, 2))
	figure = plot.figure()
	figure.canvas.manager.window.title('Statistics')
	figure.canvas.manager.window.iconphoto(False, tkinter.PhotoImage(file = os.path.join(BASE_DIR, './learning_focus.png')))
	plot.title('Statistics')
	plot.bar(numpy.arange(len(l1)), l1, width = 0.4, color = 'green', label = 'Good')
	for x, y in enumerate(l1):
		plot.text(x, y, y)
	plot.bar(numpy.arange(len(l2)) + 0.4, l2, width = 0.4, color = 'red', label = 'Bad')
	for x, y in enumerate(l2):
		plot.text(x + 0.4, y, y)
	plot.xticks(numpy.arange(len(input)) + 0.4 / 2, stick)
	plot.ylabel('Time (Minute)')
	plot.legend(loc = 'upper left')
	plot.show()

def cover_numpy_frame_func(first: numpy.ndarray, second: numpy.ndarray):
	for i in range(first.shape[0]):
		k = second[i, :]
		for j in range(first.shape[1]):
			g, b, r = k[j]
			if g != 255 or b != 255 or r != 255:
				first[i, j] = k[j]
	return first

def cover_numpy_frame_thread():
	global PROGRAM_END, rawframe, processedframe, coveredframe
	while PROGRAM_END == False:
		timestampbegin = time.time()
		coveredframe = cover_numpy_frame_func(rawframe, processedframe)
		while time.time() - timestampbegin < CONFIG.delayshow:
			time.sleep(0.1)

def show_capture_image_thread():
	global PROGRAM_END, rawframe, processedframe, coveredframe
	time.sleep(5)
	subthread = threading.Thread(target = cover_numpy_frame_thread)
	subthread.setDaemon(True)
	subthread.start()
	time.sleep(5)
	while True:
		cv2.imshow('Face', cv2.flip(coveredframe, 1))
		if 0xFF & cv2.waitKey(1) == 27:
			PROGRAM_END = True
			break

def main():
	global CONFIG, PROGRAM_END, rawframe, processedframe
	cap = cv2.VideoCapture(CONFIG.device)
	time.sleep(1)
	detector = dlib.get_frontal_face_detector()
	predictor = dlib.shape_predictor(face_recognition_models.pose_predictor_model_location())
	trackers = []
	known = []
	subthread = threading.Thread(target = show_capture_image_thread)
	subthread.setDaemon(True)
	subthread.start()
	while PROGRAM_END == False:
		timestampbegin = time.time()
		rawframe = cap.read()[1]
		processedframetmp = numpy.ones_like(rawframe) * 255
		gray = cv2.cvtColor(rawframe, cv2.COLOR_BGR2GRAY)
		faces = detector(gray)
		if len(faces) > len(trackers):
			for _ in range(len(faces) - len(trackers)):
				trackers.append(dlib.correlation_tracker())
		for i in range(len(known)):
			known[i].appeared = False
		for i in range(len(faces)):
			trackers[i].start_track(rawframe, faces[i])
			trackers[i].update(rawframe)
			pos = trackers[i].get_position()
			try:
				currentface = face_recognition.face_encodings(rawframe[int(pos.top() - 20): int(pos.top() - 20) + int(pos.bottom() + 20), int(pos.left() - 20): int(pos.left() - 20) + int(pos.right() + 20)])[0]
			except IndexError:
				continue
			if len(known) == 0:
				known.append(face(currentface))
			flag = False
			compareresult = face_recognition.compare_faces(get_class_face_array(known), currentface, CONFIG.tolerance)
			for issame in compareresult:
				if issame == True:
					flag = True
					break
			if flag == False:
				known.append(face(currentface))
			for issame in compareresult:
				if issame == True:
					id = compareresult.index(True)
					known[id].appeared = True
			cv2.rectangle(processedframetmp, (int(pos.left() - 20), int(pos.top() - 20)), (int(pos.right() + 20), int(pos.bottom() + 20)), ((0, 0, 255) if known[id].status == 'red' else (0, 255, 0)), 2)
			processedframetmp = cv2.flip(cv2.putText(cv2.flip(processedframetmp, 1), 'Face ' + str(id), (int(cap.get(cv2.CAP_PROP_FRAME_WIDTH) - pos.left() - (pos.right() - pos.left() - len(str(id)) - 5) / 2), int(pos.top() - 25)), cv2.FONT_HERSHEY_SIMPLEX, 0.5, ((0, 0, 255) if known[id].status == 'red' else (0, 255, 0)), 2), 1)
		for i in range(len(faces)):
			pos = trackers[i].get_position()
			try:
				currentface = face_recognition.face_encodings(rawframe[int(pos.top() - 20): int(pos.top() - 20) + int(pos.bottom() + 20), int(pos.left() - 20): int(pos.left() - 20) + int(pos.right() + 20)])[0]
			except IndexError:
				continue
			compareresult = face_recognition.compare_faces(get_class_face_array(known), currentface, CONFIG.tolerance)
			for issame in compareresult:
				if issame == True:
					id = compareresult.index(True)
			if 'id' not in locals():
				continue
			landmarks = predictor(image = gray, box = faces[i])
			if 1.5 * (landmarks.part(37).y - landmarks.part(19).y) < (landmarks.part(8).y - landmarks.part(57).y):
				try:
					b, g, r = rawframe[int(landmarks.part(40).x - 1), int(landmarks.part(40).y - 3)]
				except IndexError:
					known[id].status = 'red'
					known[id].timestampred = time.time()
					break
				if 0.11 * b + 0.59 * g + 0.3 * r >= 130:
					known[id].status = 'red'
					known[id].timestampred = time.time()
					break
			if 2 * (landmarks.part(29).x - landmarks.part(1).x) < (landmarks.part(15).x - landmarks.part(29).x):
				try:
					b, g, r = rawframe[int(landmarks.part(42).x + 6), int(landmarks.part(42).y - 3)]
				except IndexError:
					known[id].status = 'red'
					known[id].timestampred = time.time()
					break
				if 0.11 * b + 0.59 * g + 0.3 * r >= 100:
					known[id].status = 'red'
					known[id].timestampred = time.time()
					break
			if (landmarks.part(29).x - landmarks.part(1).x) > (2 * (landmarks.part(15).x - landmarks.part(29).x)):
				try:
					b, g, r = rawframe[int(landmarks.part(42).x + 4), int(landmarks.part(39).y - 3)]
				except IndexError:
					known[id].status = 'red'
					known[id].timestampred = time.time()
					break
				if 0.11 * b + 0.59 * g + 0.3 * r <= 100:
					known[id].status = 'red'
					known[id].timestampred = time.time()
					break
			try:
				if (landmarks.part(40).y - landmarks.part(38).y > 10) and (landmarks.part(46).y - landmarks.part(44).y < 10):
					known[id].status = 'red'
					known[id].timestampred = time.time()
					break
			except IndexError:
				known[id].status = 'red'
				known[id].timestampred = time.time()
				break
			if CONFIG.debug == True:
				for n in range(68):
					x = landmarks.part(n).x
					y = landmarks.part(n).y
					cv2.circle(img = processedframetmp, center = (x, y), radius = 2, color = (255, 255, 255), thickness = -1)
			if 'id' in locals():
				if known[id].status == 'green':
					known[id].totgreen += time.time() - known[id].timestampround
				else:
					known[id].totred += time.time() - known[id].timestampround
					if time.time() - known[id].timestampred >= 3:
						known[id].status = 'green'
				known[id].timestampround = time.time()
		for i in range(len(known)):
			if known[i].appeared == False:
				known[i].status = 'red'
				known[i].timestampred = time.time()
				known[i].totred += time.time() - known[i].timestampround
				known[i].timestampround = time.time()
		if CONFIG.debug == True:
			for i in range(len(known)):
				print('Face ' + str(i) + ':')
				known[i].debug()
			print('---')
		while time.time() - timestampbegin < CONFIG.delayface:
			time.sleep(0.1)
		processedframe = processedframetmp
	cap.release()
	cv2.destroyAllWindows()
	for i in known:
		if i.totgreen / (i.totgreen + i.totred) <= CONFIG.correction:
			known.remove(i)
	for i in range(len(known)):
		print('Total good studying time for Face ' + str(i) + ' (Minute): ' + str(round(known[i].totgreen / 60, 2)))
		print('Total bad studying time for Face ' + str(i) + ' (Minute): ' + str(round(known[i].totred / 60, 2)))
	show_plot_image_data(known)

# Main
if __name__ == '__main__':
	from matplotlib import use as usebackend
	usebackend('TkAgg')
	from matplotlib import pyplot as plot
	parse_command_line_arguments()
	set_console_window_status()
	main()