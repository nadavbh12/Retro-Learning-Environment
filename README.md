# Retro-Learning-Environment
A learning framework based on the Arcade Learning Environment (ALE) and Libretro (Stella for Atari and SNES9X for the Super Nintendo Entertainment System).

The environment provides an interface to training and evaluating AI algorithms against different console games using its screen as input.

The currently supported games can be found in the src/games/supported directory .
Some popular games include: _Mortal Kombat, Super Mario All Stars, F-Zero, Castle Wolfenstein_ and _Gradius III_.

A paper is available for RLE at http://arxiv.org/abs/1611.02205.
If you use RLE in your publication, please use the following BibTex entry:
  
    @article{bhonker2016playing,
      title = {Playing SNES in the Retro Learning Environment},
      author = {Bhonker, Nadav and Rozenberg, Shai and Hubara, Itay},
      journal = {arXiv preprint arXiv:1611.02205},
      year = {2016}
    }
   
Quick Start
===============================

Install main dependencies:
```
sudo apt-get install libsdl1.2-dev libsdl-gfx1.2-dev libsdl-image1.2-dev cmake
```
To install as a Gym environment:  
Go to the [gym-rle](https://github.com/nadavbh12/gym-rle) repository and follow the instructions there.

To install the python interface:  
Either install via PyPi:
```
$ pip install rle-python-interface
```
or by cloning the repository and running the following:
```
$ pip install .
or
$ pip install --user .
```

To use the shared_library interface:

```
$ mkdir build && cd build
$ cmake -DUSE_SDL=ON -DBUILD_EXAMPLES=ON ..
$ make -j4
```

To install the lua (Torch) interface, the additional alewrap module is required:
```sh
luarocks install https://raw.githubusercontent.com/nadavbh12/Retro-Learning-Environment/master/ale-2-0.rockspec
luarocks install https://raw.githubusercontent.com/nadavbh12/alewrap/master/alewrap-0-0.rockspec
```

DQN Implementations Using RLE
===============================
- A [fork](https://github.com/nadavbh12/deep_q_rl) of [@spragunr](https://github.com/spragunr)'s DQN implementation in Python.

## Acknowledgements
- [@mgbellemare](https://github.com/mgbellemare) for his work on ALE and his useful advice.
- [@Alcaro](https://github.com/Alcaro) and the [@libretro](https://github.com/libretro) community for their assistance in incorporating their work into our framework.
