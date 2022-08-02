import os

rt_dir = os.getcwd() + '\\'
classes = os.listdir(rt_dir + 'training_set')
test_dir = rt_dir + 'test_set\\'
val_dir = rt_dir + 'val_set\\'

with open(rt_dir + 'classes.txt', 'w', encoding = 'utf-8') as f:
	for line in classes:
		str_line = line +'\n'
		f.write(str_line)

with open(rt_dir + 'test.txt', 'w', encoding = 'utf-8') as f:
	for cnt in range(len(classes)):
		t_dir = test_dir + classes[cnt]
		files = os.listdir(t_dir)
		for line in files:
			str_line = classes[cnt] + '\\' + line + ' ' + str(cnt) + '\n'
			f.write(str_line)

with open(rt_dir + 'val.txt', 'w', encoding = 'utf-8') as f:
	for cnt in range(len(classes)):
		t_dir = val_dir + classes[cnt]
		files = os.listdir(t_dir)
		for line in files:
			str_line = classes[cnt] + '\\' + line + ' ' + str(cnt) + '\n'
			f.write(str_line)