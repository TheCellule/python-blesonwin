#!/usr/bin/env python3 
import unittest

from time import sleep
from blesonwin import (initialise, 
                       start_observer, stop_observer,
                       on_advertisement)

SCANTIME = 5          # seconds


class TestRoles(unittest.TestCase):

    def __init__(self, _):
       super().__init__(_)
       initialise()
 

    def on_advertisment_callback(self, args):
        print(args)

    def test_observer(self):
        on_advertisement(self.on_advertisment_callback)
        start_observer()
        sleep(SCANTIME)
        stop_observer()

        # TODO: assert MAC was found
