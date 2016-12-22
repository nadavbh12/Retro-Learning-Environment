import unittest
import os
from rle_python_interface import RLEInterface
import numpy as np


class RleTest(unittest.TestCase):
    def setUp(self):
        cwd = os.getcwd()
        coreName = os.path.join(cwd, '..', 'snes9x2010', 'snes9x2010_libretro.so')
        romName = os.path.join(cwd, '..', 'roms', 'mortal_kombat.sfc')
        self.rle = RLEInterface()
        self.rle.loadROM(romName, coreName)

    def test_ram(self):
        ram_size = self.rle.getRAMSize()
        self.assertEqual(128 * 1024, ram_size)
        ram = np.array(ram_size, dtype=np.uint8)
        ram = self.rle.getRAM()
        ram[0]
        ram[ram_size-1]

if __name__ == '__main__':
    unittest.main()