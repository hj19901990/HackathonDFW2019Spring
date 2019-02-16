import socket
from threading import Thread, Lock, Event
from threading import Condition
import picamera
import io
import time
#65536
#46080
class StreamingOutput(object):
    def __init__(self):
        self.frame = None
        self.buffer = io.BytesIO()
        self.condition = Condition()

    def write(self, buf):
        if buf.startswith(b'\xff\xd8'):
            # New frame, copy the existing buffer's content and notify all
            # clients it's available
            self.buffer.truncate()
            with self.condition:
                self.frame = self.buffer.getvalue()
                self.condition.notify_all()
            self.buffer.seek(0)
        return self.buffer.write(buf)

class PiCamHardware(Thread):

	def __init__(self, **kwargs):

		super(PiCamHardware, self).__init__()

		self.width = 640

		self.height = 480

		self.pakcet_contains_lines = 24

		self.lock = kwargs['lock']

		self.buffer = kwargs['buffer']

		self.img_stream_format = 'rgb'

		self.camera = picamera.PiCamera(resolution=(self.width, self.height), framerate=60)

		self.output = StreamingOutput()

		self.target_ip = '192.168.111.13'
		self.target_port = 4242
		
		self.local_ip = "192.168.111.98"
		self.local_udp_port = 4242

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP # Internet 
		self.sock.settimeout(1)

		try:
			self.sock.bind((self.local_ip, self.local_udp_port))
		except OSError:
			print("Only one usage of each socket address (protocol/network address/port) is normally permitted")

	def __get_timestamp_bytes__(self):

		return (int(round(time.time() * 1000)) % 0xffffffff).to_bytes(4, byteorder='little')

	def __send_rgb__(self, frame):

		frame_size = len(frame)

		if frame_size % self.height != 0:

			raise Exception("Frame Size Error!")

		timestamp_buf = self.__get_timestamp_bytes__()

		for i in range(0, self.height, self.pakcet_contains_lines):

			startPos = i * self.width * 3

			curBuf = frame[startPos : (startPos + self.width * 3 * self.pakcet_contains_lines)]

			self.sock.sendto(timestamp_buf + int(i / self.pakcet_contains_lines).to_bytes(4, byteorder='little') + curBuf, (self.target_ip, self.target_port))

	def run(self):

		stream = io.BytesIO()
		frame = None

		for foo in self.camera.capture_continuous(stream, format='rgb', use_video_port = True):

			stream.truncate()

			frame = stream.getvalue()

			# newframe = b'P6\n640 480 255\n' + frame
			# f = open("/home/pi/demofile.ppm", "wb")
			# f.write(newframe)
			# f.close()


			stream.seek(0)
			# print(len(frame))

			self.__send_rgb__(frame)

			# print(time.time())

if __name__ == '__main__':

	data = {
		"lock": Lock(),
		"buffer": [b''], # Initialize by hardware driver
	}

	piCamHardware = PiCamHardware(**data)

	piCamHardware.start()