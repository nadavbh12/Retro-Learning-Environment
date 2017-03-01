import unittest
import os
from rle_python_interface.rle_python_interface import RLEInterface
import numpy as np


class RleTest(unittest.TestCase):
    def setUp(self):
        cwd = os.getcwd()
        core = 'snes'
        romName = os.path.join(cwd, 'roms', 'classic_kong.smc')
        self.rle = RLEInterface()
        self.rle.loadROM(romName, core)

    def test_ram(self):
        ram_size = self.rle.getRAMSize()
        self.assertEqual(128 * 1024, ram_size)
        ram = np.array(ram_size, dtype=np.uint8)
        ram = self.rle.getRAM()
        self.assertIsNotNone(ram[0])
        self.assertIsNotNone(ram[ram_size-1])

    def test_getScreenRGB(self):
        dimensions = self.rle.getScreenDims()
        self.assertEqual(dimensions[0], 256)
        self.assertEqual(dimensions[1], 224)
        screen_data = np.empty((dimensions[1], dimensions[0], 4), dtype=np.uint8)

        for _ in range(10):
            self.rle.act(0)
            self.rle.getScreenRGB(screen_data=screen_data)
            self.rle.getScreenGrayscale(screen_data=screen_data)

    def test_getScreenGrayscale(self):
        dimensions = self.rle.getScreenDims()
        self.assertEqual(dimensions[0], 256)
        self.assertEqual(dimensions[1], 224)
        screen_data = np.empty((dimensions[1], dimensions[0]), dtype=np.uint8)
        self.assertEqual(224*256, screen_data.size)
        for _ in range(100):
            self.rle.act(0)
        self.rle.getScreenGrayscale(screen_data=screen_data)

if __name__ == '__main__':
    unittest.main()