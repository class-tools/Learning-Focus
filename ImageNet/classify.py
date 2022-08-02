import os
import shutil

ext = '.jpg'
rt_dir = os.getcwd() + '\\'
classes = os.listdir(rt_dir + 'result_set')
read_dir = rt_dir + 'result_set\\'
training_dir = rt_dir + 'training_set\\'
test_dir = rt_dir + 'test_set\\'
val_dir = rt_dir + 'val_set\\'

for cnt in range(len(classes)):
	r_dir = read_dir + classes[cnt] + '\\'
	files = os.listdir(r_dir)
	files = files[:1000]
	offset1 = int(len(files) * 0.6)
	offset2 = int(len(files) * 0.8)
	training_data = files[:offset1]
	val_data = files[offset1:offset2]
	test_data = files[offset2:]
	for index, fileName in enumerate(training_data):
		w_dir = training_dir + classes[cnt] + '\\'
		if not os.path.exists(w_dir):
			os.makedirs(w_dir)
		shutil.copy(r_dir + fileName, w_dir + classes[cnt] + str(index) + ext)
	for index, fileName in enumerate(test_data):
		w_dir = test_dir + classes[cnt] + '\\'
		if not os.path.exists(w_dir):
			os.makedirs(w_dir)
		shutil.copy(r_dir + fileName, w_dir + classes[cnt] + str(index) + ext)
	for index, fileName in enumerate(val_data):
		w_dir = val_dir + classes[cnt] + '\\'
		if not os.path.exists(w_dir):
			os.makedirs(w_dir)
		shutil.copy(r_dir + fileName, w_dir + classes[cnt] + str(index) + ext)