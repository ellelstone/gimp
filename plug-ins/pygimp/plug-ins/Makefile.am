## Process this file with automake to produce Makefile.in

pluginexecdir = $(gimpplugindir)/plug-ins

scripts = \
	file-openraster.py	\
	histogram-export.py     \
	palette-offset.py	\
	palette-sort.py		\
	palette-to-gradient.py	\
	python-console.py	\
	python-eval.py

pluginexec_SCRIPTS = $(scripts)

pluginexec_DATA = pyconsole.py

EXTRA_DIST = $(pluginexec_DATA) $(scripts) $(test_scripts)
