# Libraries
import os
import sys
import cv2
import dlib
import time
import numpy
import tkinter
import argparse
import face_recognition

# Variables
CONFIG = None
PROGRAM_END = False
BASE_DIR = (os.path.dirname(sys.executable) if hasattr(sys, 'frozen') else os.path.dirname(__file__))

# Functions
def parse_command_line_arguments():
	global CONFIG
	parser = argparse.ArgumentParser(description = 'Learning Focus command line arguments.')
	parser.add_argument('--debug', action = 'store_true', help = 'Enable debug mode.')
	parser.add_argument('--delay', type = int, default = -1, help = 'Delay time after predicting (Millisecond).')
	CONFIG = parser.parse_args()

def show_plot_image_data(input: list):
	stick = []
	l1 = []
	l2 = []
	for i in range(len(input[0])):
		stick.append('Face ' + str(i))
		l1.append(round(input[2][i] / 60, 2))
		l2.append(round(input[3][i] / 60, 2))
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
	plot.xticks(numpy.arange(len(input[0])) + 0.4 / 2, stick)
	plot.ylabel('Time (Minute)')
	plot.legend(loc = 'upper left')
	plot.show()

def main():
	global CONFIG, PROGRAM_END, BASE_DIR
	cap = cv2.VideoCapture(0)
	time.sleep(1)
	detector = dlib.get_frontal_face_detector()
	predictor = dlib.shape_predictor(os.path.join(BASE_DIR, './learning_focus.dat'))
	trackers = []
	known = [[], [], [], [], [], [], []]
	while PROGRAM_END == False:
		timestampstart = time.time()
		frame = cap.read()[1]
		gray = cv2.cvtColor(src = frame, code = cv2.COLOR_BGR2GRAY)
		faces = detector(gray)
		if len(faces) > len(trackers):
			for _ in range(len(faces) - len(trackers)):
				trackers.append(dlib.correlation_tracker())
		for i in range(len(known[0])):
			known[6][i] = False
		for i in range(len(faces)):
			trackers[i].start_track(frame, faces[i])
			trackers[i].update(frame)
			pos = trackers[i].get_position()
			try:
				currentface = face_recognition.face_encodings(frame[int(pos.top() - 20): int(pos.top() - 20) + int(pos.bottom() + 20), int(pos.left() - 20): int(pos.left() - 20) + int(pos.right() + 20)])[0]
			except IndexError:
				continue
			if len(known[0]) == 0:
				known[0].append('green')
				known[1].append(currentface)
				known[2].append(0.0)
				known[3].append(0.0)
				known[4].append(time.time())
				known[5].append(0.0)
				known[6].append(True)
			compareresult = face_recognition.compare_faces(known[1], currentface)
			for issame in compareresult:
				if issame == False:
					known[0].append('green')
					known[1].append(currentface)
					known[2].append(0.0)
					known[3].append(0.0)
					known[4].append(time.time())
					known[5].append(0.0)
					known[6].append(True)
			for issame in compareresult:
				if issame == True:
					id = next((i for i, val in enumerate(known[1]) if numpy.all(val == currentface)), -1)
					known[6][id] = True
					break
			cv2.rectangle(frame, (int(pos.left() - 20), int(pos.top() - 20)), (int(pos.right() + 20), int(pos.bottom() + 20)), ((0, 0, 255) if known[0][id] == 'red' else (0, 255, 0)), 2)
		for i in range(len(faces)):
			pos = trackers[i].get_position()
			try:
				currentface = face_recognition.face_encodings(frame[int(pos.top() - 20): int(pos.top() - 20) + int(pos.bottom() + 20), int(pos.left() - 20): int(pos.left() - 20) + int(pos.right() + 20)])[0]
			except IndexError:
				continue
			for issame in face_recognition.compare_faces(known[1], currentface):
				if issame == True:
					id = next((i for i, val in enumerate(known[1]) if numpy.all(val == currentface)), -1)
					break
			landmarks = predictor(image = gray, box = faces[i])
			if 1.5 * (landmarks.part(37).y - landmarks.part(19).y) < (landmarks.part(8).y - landmarks.part(57).y):
				try:
					b, g, r = frame[int(landmarks.part(40).x - 1), int(landmarks.part(40).y - 3)]
				except IndexError:
					known[0][id] = 'red'
					known[5][id] = time.time()
					break
				if 0.11 * b + 0.59 * g + 0.3 * r >= 130:
					known[0][id] = 'red'
					known[5][id] = time.time()
					break
			if 2 * (landmarks.part(29).x - landmarks.part(1).x) < (landmarks.part(15).x - landmarks.part(29).x):
				try:
					b, g, r = frame[int(landmarks.part(42).x + 6), int(landmarks.part(42).y - 3)]
				except IndexError:
					known[0][id] = 'red'
					known[5][id] = time.time()
					break
				if 0.11 * b + 0.59 * g + 0.3 * r >= 100:
					known[0][id] = 'red'
					known[5][id] = time.time()
					break
			if (landmarks.part(29).x - landmarks.part(1).x) > (2 * (landmarks.part(15).x - landmarks.part(29).x)):
				try:
					b, g, r = frame[int(landmarks.part(42).x + 4), int(landmarks.part(39).y - 3)]
				except IndexError:
					known[0][id] = 'red'
					known[5][id] = time.time()
					break
				if 0.11 * b + 0.59 * g + 0.3 * r <= 100:
					known[0][id] = 'red'
					known[5][id] = time.time()
					break
			try:
				if (landmarks.part(40).y - landmarks.part(38).y > 10) and (landmarks.part(46).y - landmarks.part(44).y < 10):
					known[0][id] = 'red'
					known[5][id] = time.time()
					break
			except IndexError:
				known[0][id] = 'red'
				known[5][id] = time.time()
				break
			if CONFIG.debug == True:
				for n in range(68):
					x = landmarks.part(n).x
					y = landmarks.part(n).y
					cv2.circle(img = frame, center = (x, y), radius = 2, color = (255, 255, 255), thickness = -1)
			if 'id' in locals():
				if CONFIG.debug == True:
					print(known)
				if known[0][id] == 'green':
					known[2][id] += time.time() - known[4][id]
				else:
					known[3][id] += time.time() - known[4][id]
					if time.time() - known[5][id] >= 3:
						known[0][id] = 'green'
				known[4][id] = time.time()
		for i in range(len(known[0])):
			if known[6][i] == False:
				known[0][i] = 'red'
				known[5][id] = time.time()
		cv2.imshow('Face', cv2.flip(frame, 1))
		while True:
			if 0xFF & cv2.waitKey(100) == 27:
				PROGRAM_END = True
				break
			if (time.time() - timestampstart) * 1000 >= CONFIG.delay:
				break
	cap.release()
	cv2.destroyAllWindows()
	for i in range(len(known[0])):
		print('Total good studying time for Face ' + str(i) + ' (Minute): ' + str(round(known[2][i] / 60, 2)))
		print('Total bad studying time for Face ' + str(i) + ' (Minute): ' + str(round(known[3][i] / 60, 2)))
	show_plot_image_data(known)

# Main
if __name__ == '__main__':
	from matplotlib import use as usebackend
	usebackend('TkAgg')
	from matplotlib import pyplot as plot
	parse_command_line_arguments()
	main()