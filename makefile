STORM=StormLib

.PHONY: all stormlib clean clean-stormlib

all: stormlib
	node-gyp configure build

clean: clean-stormlib
	node-gyp clean

stormlib:
	cd $(STORM) && cmake CMakeLists.txt && make

clean-stormlib:
	cd $(STORM) && make clean
	rm -f $(STORM)/cmake_install.cmake
	rm -f $(STORM)/CMakeCache.txt
	rm -f $(STORM)/makefile
	rm -rf $(STORM)/CMakeFiles
	rm -rf $(STORM)/storm.framework