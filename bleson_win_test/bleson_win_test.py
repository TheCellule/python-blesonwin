from time import sleep
from blesonwin import (initialise, 
                       start_advertiser, stop_advertiser,
                       start_observer, stop_observer,
                       on_advertisement)

def on_advertisment_callback(args):
    print(args)

initialise()

on_advertisement(on_advertisment_callback)
start_observer()
sleep(2)
stop_observer()

start_advertiser()
sleep(2)
stop_advertiser()
