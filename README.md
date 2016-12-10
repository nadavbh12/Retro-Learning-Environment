# Retro-Learning-Environment
A learning framework based on the Arcade Learning Environment (ALE) and LibRetro (Stella for Atari and SNES9X for the Super Nintendo Entertainment System).

The environment provides an inreface to training and evaluating AI algorithms against different console games using its screen as input.

The currently supported games can be found in the src/games/supported directory .
Some popular games include: _Mortal Kombat, Super Mario All Stars, F-Zero, Castle Wolfenstein_ and _Gradius III_.

===============================
Quick Start
===============================

Install main dependences:
```
sudo apt-get install libsdl1.2-dev libsdl-gfx1.2-dev libsdl-image1.2-dev cmake
```

Compilation:

```
$ mkdir build && cd build
$ cmake -DUSE_SDL=ON -DBUILD_EXAMPLES=ON ..
$ make -j 4
```

To install the python module:

```
$ pip install .
or
$ pip install --user .
```

To install the lua (Torch) interface, the additional alewrap module is required:
```sh
luarocks install https://raw.githubusercontent.com/nadavbh12/Retro-Learning-Environment/master/ale-2-0.rockspec
luarocks install https://raw.githubusercontent.com/nadavbh12/alewrap/master/alewrap-0-0.rockspec
```
===============================
DQN Implementations Using RLE
===============================
- A [fork](https://github.com/nadavbh12/deep_q_rl) of [@spragunr](https://github.com/spragunr)'s DQN implementation in Python.

## Acknowledgements
- [@mgbellemare](https://github.com/mgbellemare) for his work on ALE and his useful advice.
- [@Alcaro](https://github.com/Alcaro) and the [@libretro](https://github.com/libretro) community for their assistance in incorporating their work into our framework.
