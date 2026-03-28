import cv2

pipeline = (
    "nvarguscamerasrc sensor-id=0 ! "
    "video/x-raw(memory:NVMM), width=1920, height=1080, framerate=30/1 ! "
    "nvvidconv ! video/x-raw, format=BGRx ! "
    "videoconvert ! video/x-raw, format=BGR ! appsink"
)

cap = cv2.VideoCapture(pipeline, cv2.CAP_GSTREAMER)
ret, frame = cap.read()

if ret:
    cv2.imwrite("test_frame.jpg", frame)
    print("Frame saved to test_frame.jpg")
else:
    print("Failed to capture frame")

cap.release()