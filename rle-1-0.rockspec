package = 'rle'
version = '1-0'

source = {
   url = 'git://github.com/nadavbh12/Retro-Learning-Environment.git'
}

description = {
  summary = "Rle"
}

dependencies = { 'torch >= 7.0' }
build = {
     type = "command",
     build_command = [[
          cmake -DINSTALL_CORES=ON -DUSE_SDL=OFF -DBUILD_EXAMPLES=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="$(LUA_BINDIR)/.." -DCMAKE_INSTALL_PREFIX="$(PREFIX)/../../../../../" .;
          $(MAKE) -j 4
	     ]],
     install_command = "$(MAKE) install"
     }
