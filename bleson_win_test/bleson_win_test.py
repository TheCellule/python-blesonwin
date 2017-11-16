from time import sleep
from blesonwin import initialise, start_observer, stop_observer

initialise()

start_observer()
sleep(5)
stop_observer()