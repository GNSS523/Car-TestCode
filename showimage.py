
import cv2

imgFile = cv2.imread('/home/ubuntu/Desktop/1.jpg')

imgFile.shape
cv2.imshow("cat",imgFile)
cv2.waitKey(0)
cv2.destroyAllWindows()
