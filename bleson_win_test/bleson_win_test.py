from time import sleep
from blesonwin import (initialise, 
                       start_advertiser, stop_advertiser,
                       start_observer, stop_observer)

initialise()

start_observer()
sleep(5)
stop_observer()

start_advertiser()
sleep(5)
stop_advertiser()

