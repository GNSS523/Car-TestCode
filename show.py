import cv2
cv2.namedWindow('Video')
capture = cv2.VideoCapture(0)
_, frame = capture.read()
while frame is not None:
	cv2.imshow('Video',frame)
	key = cv2.waitKey(10)
	if key == ord('s'):
		cv2.imwrite('screenshot.jpg',frame)
	elif key == ord('q'):
		break
	_, frame = capture.read()
cv2.destroyWindow('Video')
