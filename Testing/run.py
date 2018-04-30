'''
must have pynput, via pip
'''

#1. import dependecies

#parsing command line arguments
import argparse


#reading and writing files and launching capture program
import os

#matrix math
import numpy as np
#image manipulation
from PIL import Image

#helper class
'''
put this in same folder
'''
import utils
# to generate key presses
from pynput.keyboard import Key, Controller, Listener
import pyautogui 

#to sleep thread
import time

#2. load our saved model

from keras.models import load_model

#3. method to launch screen capturing program
agentOn = False
keyboard = Controller()

#loop:
def play_game():

    # give user time to get to program
    time.sleep(30)

    counter = 0
    img_path = ''
#add a start button
    while True:

        # release keys
        release_keys()
        img_path = str(counter) + ".jpeg"   # set up to only accept one image at a time?

        #4. retrieve images from file [file should hold 4 images and cycles through]
        # get screen
        image = Image.open(img_path)

        # run screen through network and assign to key presses
        try:
            image = np.asarray(image)       # from PIL image to numpy array
            image = utils.preprocess(image) # apply the preprocessing
            image = np.array([image])       # the model expects 4D array

            #5. run images through network to make predictions
            # predict the keypresses
            keys = model.predict(image, batch_size=1)

            # modify array to make binary and 1D
            

            print(keys) # 1D arry length 14?

            # press keys
            press_keys(keys)

        except Exception as e:
            print(e)

        
        # increment image counter
        counter += 1
        if counter == 4:
            counter = 0
            
        time.sleep(.250)

#y = data_df[['space','1','2','3','4','5','6','7','left','right','up','down','shift','ctrl']].values

def release_keys():

    pyautogui.keyUp('space')
    pyautogui.keyUp('1')
    pyautogui.keyUp('2')
    pyautogui.keyUp('3')
    pyautogui.keyUp('4')
    pyautogui.keyUp('5')
    pyautogui.keyUp('6')
    pyautogui.keyUp('7')
    pyautogui.keyUp('left')
    pyautogui.keyUp('right')
    pyautogui.keyUp('up')
    pyautogui.keyUp('down')
    pyautogui.keyUp('shiftleft')
    pyautogui.keyUp('ctrlleft')

    '''
    keyboard.release(Key.space)
    keyboard.release('1')
    keyboard.release('2')
    keyboard.release('3')
    keyboard.release('4')
    keyboard.release('5')
    keyboard.release('6')
    keyboard.release('7')
    keyboard.release(Key.left)
    keyboard.release(Key.right)
    keyboard.release(Key.up)
    keyboard.release(Key.down)
    keyboard.release(Key.shift_l)
    keyboard.release(Key.ctrl_l)
    '''
#6. Programably generate keypresses 

#y = data_df[['space','1','2','3','4','5','6','7','left','right','up','down','shift','ctrl']].values
def press_keys(keys):
    if keys[0] == 1:
        pyautogui.keyDown('space')
    if keys[1] == 1:
        pyautogui.keyDown('1')
    if keys[2] == 1:
        pyautogui.keyDown('2')
    if keys[3] == 1:
        pyautogui.keyDown('3')
    if keys[4] == 1:
        pyautogui.keyDown('4')
    if keys[5] == 1:
        pyautogui.keyDown('5')
    if keys[6] == 1:
        pyautogui.keyDown('6')
    if keys[7] == 1:
        pyautogui.keyDown('7')
    if keys[8] == 1:
        pyautogui.keyDown('left')
    if keys[9] == 1:
        pyautogui.keyDown('right')
    if keys[10] == 1:
        pyautogui.keyDown('up')
    if keys[11] == 1:
        pyautogui.keyDown('down')        
    if keys[12] == 1:
        pyautogui.keyDown('shiftleft')
    if keys[13] == 1:          
        pyautogui.keyDown('ctrlleft')

'''
def on_press(key):
    print('{0} pressed'.format(key))

def on_release(key):
    print('{0} release'.format(key))
    if key == Key.esc:
        # Stop listener
        return False

#put this in a function        
with Listener(on_press=on_press, on_release=on_release) as listener:
    listener.join()
'''

#exit

#init our model and image array as empty
model = None
prev_image_array = None

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Remote Play')
    parser.add_argument(
        'model',
        type=str,
        help='Path to model h5 file. Model should be on the same path.'
    )
    args = parser.parse_args()

    #load model
    model = load_model(args.model)

    #start agent 
    play_game()


