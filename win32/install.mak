# NMake Makefile snippet for copying the built libraries, utilities and headers to
# a path under $(PREFIX).

install: all
	@if not exist $(PREFIX)\bin\ mkdir $(PREFIX)\bin
	@if not exist $(PREFIX)\lib\ mkdir $(PREFIX)\lib
	@if not exist $(PREFIX)\include\raqm\ mkdir $(PREFIX)\include\raqm
	@copy /b $(RAQM_DLL_FILENAME).dll $(PREFIX)\bin
	@copy /b $(RAQM_DLL_FILENAME).pdb $(PREFIX)\bin
	@copy /b $(CFG)\$(PLAT)\raqm.lib $(PREFIX)\lib
	@rem Copy the generated introspection files
