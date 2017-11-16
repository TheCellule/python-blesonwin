from time import sleep
from blesonwin import (initialise, 
                       start_advertiser, stop_advertiser,
                       start_observer, stop_observer)

initialise()


start_advertiser()
sleep(5)
stop_advertiser()

exit(0)
sleep(3)

start_observer()
sleep(5)
stop_observer()