import os
from PIL import Image
from torchvision import transforms

ext = '.jpg'
rt_dir = os.getcwd() + '\\'
classes = os.listdir(rt_dir + 'origin_set')
read_dir = rt_dir + 'origin_set\\'
new_dir = rt_dir + 'result_set\\'

for cnt in range(len(classes)):
	r_dir = read_dir + classes[cnt] + '\\'
	files = os.listdir(r_dir)
	for index, file in enumerate(files):
		img_path = r_dir + file
		img = Image.open(img_path)
		resize = transforms.Resize([20, 20])
		IMG = resize(img)
		w_dir = new_dir + classes[cnt] + '\\'
		if not os.path.exists(w_dir):
			os.makedirs(w_dir)
		save_path = w_dir + str(index) + ext
		IMG = IMG.convert('RGB')
		IMG.save(save_path)