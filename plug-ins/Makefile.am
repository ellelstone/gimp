## Process this file with automake to produce Makefile.in

if HAVE_WEBKIT
help_browser = help-browser
endif

if HAVE_OPENEXR
file_darktable = file-darktable
file_exr = file-exr
endif

if BUILD_PRINT
print = print
endif

if BUILD_PYTHON
pygimp = pygimp
endif

if BUILD_SCRIPT_FU
script_fu = script-fu
endif

if OS_WIN32
twain = twain
endif

SUBDIRS = \
	$(script_fu)		\
	$(file_darktable)	\
	$(file_exr)		\
	file-fits		\
	file-jpeg		\
	file-tiff		\
	help			\
	$(help_browser)		\
	$(print)		\
	screenshot		\
	selection-to-path	\
	$(twain)		\
	ui			\
	common
