## Process this file with automake to produce Makefile.in

SUBDIRS = images

scriptdatadir = $(gimpdatadir)/scripts

scripts = \
	script-fu.init			\
	script-fu-compat.init		\
	plug-in-compat.init		\
	palette-export.scm		\
	paste-as-brush.scm		\
	paste-as-pattern.scm		\
	select-to-brush.scm		\
	select-to-image.scm		\
	select-to-pattern.scm

test_scripts = \
	contactsheet.scm		\
	test-sphere.scm			\
	ts-helloworld.scm


scriptdata_DATA = $(scripts)


EXTRA_DIST = $(scripts) $(test_scripts)
