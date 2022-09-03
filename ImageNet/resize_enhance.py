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
		resize = transforms.Resize([224, 224])
		IMG1 = resize(img.transpose(Image.Transpose.FLIP_LEFT_RIGHT))
		IMG2 = resize(img.transpose(Image.Transpose.FLIP_TOP_BOTTOM))
		IMG3 = resize(img.transpose(Image.Transpose.ROTATE_90))
		IMG4 = resize(img.transpose(Image.Transpose.ROTATE_180))
		IMG5 = resize(img.transpose(Image.Transpose.ROTATE_270))
		w_dir = new_dir + classes[cnt] + '\\'
		if not os.path.exists(w_dir):
			os.makedirs(w_dir)
		save_path1 = w_dir + str(index * 5) + ext
		save_path2 = w_dir + str(index * 5 + 1) + ext
		save_path3 = w_dir + str(index * 5 + 2) + ext
		save_path4 = w_dir + str(index * 5 + 3) + ext
		save_path5 = w_dir + str(index * 5 + 4) + ext
		IMG1 = IMG1.convert('RGB')
		IMG2 = IMG2.convert('RGB')
		IMG3 = IMG3.convert('RGB')
		IMG4 = IMG4.convert('RGB')
		IMG5 = IMG5.convert('RGB')
		IMG1.save(save_path1)
		IMG2.save(save_path2)
		IMG3.save(save_path3)
		IMG4.save(save_path4)
		IMG5.save(save_path5)
	print(cnt)