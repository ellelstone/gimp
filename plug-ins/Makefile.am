## Process this file with automake to produce Makefile.in

if HAVE_WEBKIT
help_browser = help-browser
endif

if HAVE_OPENEXR
file_exr = file-exr
endif

if BUILD_PRINT
print = print
endif

if BUILD_PYTHON
pygimp = pygimp
endif

if OS_WIN32
twain = twain
endif

if HAVE_WEBP
file_webp = file-webp
endif

SUBDIRS = \
	$(file_darktable)	\
	$(file_exr)		\
	file-fits		\
	file-jpeg		\
	file-psd		\
	file-raw		\
	file-tiff   \
	screenshot		\
	selection-to-path	\
	$(twain)		\
	ui			\
	common
