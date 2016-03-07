# NMake Makefile portion for code generation and
# intermediate build directory creation
# Items in here should not need to be edited unless
# one is maintaining the NMake build files.

# Copy the pre-defined config.h.win32
config.h: config.h.win32
	@-copy $@.win32 $@


# Create the build directories
$(CFG)\$(PLAT)\raqm:
	@-mkdir $@
