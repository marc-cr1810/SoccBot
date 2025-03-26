import sensor, image, time, pyb, ustruct
from pyb import UART

uart = UART(3, 115200, timeout_char=1000)
uart.init(115200, bits=8, parity=None, stop=1, timeout_char=1000)

#
#   Variables
#

led = pyb.LED(2)    # Red LED = 1, Green LED = 2, Blue LED = 3, IR LEDs = 4.
led_yellow = pyb.LED(1)    # Red LED = 1, Green LED = 2, Blue LED = 3, IR LEDs = 4.
led_blue = pyb.LED(3)    # Red LED = 1, Green LED = 2, Blue LED = 3, IR LEDs = 4.
usb = pyb.USB_VCP() # This is a serial port object that allows you to communciate with your computer.
                    # While it is not open the code below runs.

threshold_index = 0 # 0 for yellow goal, 1 for blue goal

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [(80, 95, -10, 28, 20, 60),    # yellow goal
              (35, 55, 0, 35, -75, -50)]   # blue goal

white = (85, 100, -5, 5, -5, 5)

clock = time.clock()


#
#   Functions to use for the camera program
#

def init_camera():
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA)
    sensor.skip_frames(time = 500)
    sensor.set_auto_gain(False)     # must be turned off for color tracking
    sensor.set_auto_whitebal(False) # must be turned off for color tracking

    sensor.set_vflip(True)

def get_opposing_goal():
    blue = False
    yellow = False

    led_on = False
    t_prev = clock.avg()

    while (True):
        blue = False
        yellow = False

        if clock.avg() - t_prev > 500:
            if led_on:
                t_prev = clock.avg();
                led.off()
                led_on = False
            else:
                t_prev = clock.avg();
                led.on()
                led_on = True

        img = sensor.snapshot()
        for blob in img.find_blobs([thresholds[0]], pixels_threshold=200, area_threshold=200, merge=True):
            img.draw_rectangle(blob.rect(), color=[255, 255, 0])
            img.draw_cross(blob.cx(), blob.cy(), color=[255, 255, 0])
            yellow = True   # found a yellow goal
        for blob in img.find_blobs([thresholds[1]], pixels_threshold=200, area_threshold=200, merge=True):
            img.draw_rectangle(blob.rect(), color=[0, 127, 255])
            img.draw_cross(blob.cx(), blob.cy(), color=[0, 127, 255])
            blue = True     # found a blue goal

        if yellow == True and blue == False:
            global threshold_index
            threshold_index = 0
            print(threshold_index)
            break
        elif yellow == False and blue == True:
            global threshold_index
            threshold_index = 1
            print(threshold_index)
            break
        print(threshold_index)
    led.off()

def send_data(color, width, height, center_x, center_y):
    print("Sending...")
    width1 = width & 0xFF
    width2 = (width >> 8) & 0xFF
    height1 = height & 0xFF
    height2 = (height >> 8) & 0xFF
    cx1 = center_x & 0xFF
    cx2 = (center_x >> 8) & 0xFF
    cy1 = center_y & 0xFF
    cy2 = (center_y >> 8) & 0xFF
    #
    #msg = chr(97) + chr(98) + chr(99) + chr(100)
    data = ustruct.pack("BBBBBBBBB", threshold_index, width1, width2, height1, height2, cx1, cx2, cy1, cy2)
    uart.write(data)

def get_goal_area():
    # Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
    # returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
    # camera resolution. "merge=True" merges all overlapping blobs in the image.

    while(True):
        clock.tick()
        img = sensor.snapshot()
        img.binary([thresholds[threshold_index]])
        blobs = img.find_blobs([white], pixels_threshold=200, area_threshold=200, merge=True)
        biggest = [0, 0, 0, 0]
        biggest_blob = None
        for blob in blobs:
            current = blob.rect()
            if current[2] * current[3] > biggest[2] * biggest[3]:
                biggest = current
                biggest_blob = blob
        if biggest_blob is not None:
            img.draw_rectangle(biggest_blob.rect())
            img.draw_cross(biggest_blob.cx(), biggest_blob.cy())
            send_data(threshold_index, img.width(), img.height(), blob.cx(), blob.cy())
        else:
            send_data(threshold_index, img.width(), img.height(), -1, -1)
        #print(clock.fps())


#
#   Beginning of the camera program
#

init_camera()
get_opposing_goal()

if threshold_index == 0:
    led_yellow.on()
else:
    led_blue.on()
time.sleep_ms(50)
if threshold_index == 0:
    led_yellow.off()
else:
    led_blue.off()

get_goal_area()
