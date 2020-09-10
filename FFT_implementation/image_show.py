import numpy as np
import cv2
file = open("testfile.txt","r") 
img = cv2.imread("A1Image__2020-07-09__09-32-50.bmp",0)

lines = []
for line in file:
        inner_list = [int(elt.strip()) for elt in line.split(',')]
        lines.append(inner_list)
file.close()
lines = lines [:1080]
array = np.array(lines, dtype=np.uint8)
image = cv2.flip(array,0)

# for i in range(1):
# for j in range(image.shape[1]):
#         print(image[0][j])
#         print(img[0][j])
#         print("-------------")

print(image)
print(img)

cv2.imwrite('check.bmp',image)