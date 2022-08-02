# Libraries
import cv2
import dlib
import json
import time
import numpy
import datetime
import matplotlib.pyplot as plot

# Functions
def show_plot_image_data(input: list):
    l1 = []
    l2 = []
    stick = []
    for time, totgreen, totred in input:
        stick.append(time)
        l1.append(totgreen)
        l2.append(totred)
    figure = plot.figure()
    figure.canvas.manager.window.title('Recent')
    figure.canvas.manager.window.iconbitmap('learning_focus.ico')
    plot.title('Recent')
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

def save_total_time_data(time: str, totgreen: float, totred: float):
    try:
        with open('./recent.json', 'r', encoding = 'utf-8') as f:
            input = json.load(f)
    except (FileNotFoundError, json.decoder.JSONDecodeError):
        input = []
    with open('./recent.json', 'w', encoding = 'utf-8') as f:
        input.append([time, totgreen, totred])
        input = input[-3:]
        show_plot_image_data(input)
        json.dump(input, f)

def main():
    cap = cv2.VideoCapture(0)
    time.sleep(1)
    detector = dlib.get_frontal_face_detector()
    predictor = dlib.shape_predictor('learning_focus.dat')
    trackers = [dlib.correlation_tracker() for _ in range(10)]
    status = 'green'
    totgreen = 0
    totred = 0
    while True:
        timestampstart = time.time()
        frame = cap.read()[1]
        gray = cv2.cvtColor(src = frame, code = cv2.COLOR_BGR2GRAY)
        faces = detector(gray)
        for k in range(len(faces)):
            trackers[k].start_track(frame, faces[k])
            trackers[k].update(frame)
            pos = trackers[k].get_position()
            cv2.rectangle(frame, (int(pos.left() - 20), int(pos.top() - 20)), (int(pos.right() + 20), int(pos.bottom() + 20)), ((0, 0, 255) if status == 'red' else (0, 255, 0)), 2)
        if len(faces) == 0:
            status = 'red'
        else:
            status = 'green'
        for face in faces:
            landmarks = predictor(image = gray, box = face)
            if 1.5 * (landmarks.part(37).y - landmarks.part(19).y) < (landmarks.part(8).y - landmarks.part(57).y):
                try:
                    b, g, r = frame[int(landmarks.part(40).x - 1), int(landmarks.part(40).y - 3)]
                except IndexError:
                    status = 'red'
                    break
                if 0.11 * b + 0.59 * g + 0.3 * r >= 130:
                    status = 'red'
                    break
            if 2 * (landmarks.part(29).x - landmarks.part(1).x) < (landmarks.part(15).x - landmarks.part(29).x):
                try:
                    b, g, r = frame[int(landmarks.part(42).x + 6), int(landmarks.part(42).y - 3)]
                except IndexError:
                    status = 'red'
                    break
                if 0.11 * b + 0.59 * g + 0.3 * r >= 100:
                    status = 'red'
                    break
            if (landmarks.part(29).x - landmarks.part(1).x) > (2 * (landmarks.part(15).x - landmarks.part(29).x)):
                try:
                    b, g, r = frame[int(landmarks.part(42).x + 4), int(landmarks.part(39).y - 3)]
                except IndexError:
                    status = 'red'
                    break
                if 0.11 * b + 0.59 * g + 0.3 * r <= 100:
                    status = 'red'
                    break
            try:
                if (landmarks.part(40).y - landmarks.part(38).y > 10) and (landmarks.part(46).y - landmarks.part(44).y < 10):
                    status = 'red'
                    break
            except IndexError:
                status = 'red'
                break
            for n in range(68):
                x = landmarks.part(n).x
                y = landmarks.part(n).y
                cv2.circle(img = frame, center = (x, y), radius = 2, color = (255, 255, 255), thickness = -1)
        if status == 'green':
            totgreen += time.time() - timestampstart
        else:
            totred += time.time() - timestampstart
        cv2.imshow(winname = 'Face', mat = frame)
        if 0xFF & cv2.waitKey(1) == 27:
            break
    cap.release()
    cv2.destroyAllWindows()
    print('Total good studying time (Minute): ' + str(round(totgreen / 60, 2)))
    print('Total bad studying time (Minute): ' + str(round(totred / 60, 2)))
    save_total_time_data('{:04}-{:02}-{:02} {:02}:{:02}:{:02}'.format(datetime.datetime.now().year, datetime.datetime.now().month, datetime.datetime.now().day, datetime.datetime.now().hour, datetime.datetime.now().minute, datetime.datetime.now().second), round(totgreen / 60, 2), round(totred / 60, 2))

# Main
if __name__ == '__main__':
    main()