## Process this file with automake to produce Makefile.in
# elle: file-openraster.py removed because of HSV blend modes
# elle: other python plug-ins also removed.
## Process this file with automake to produce Makefile.in

pluginexecdir = $(gimpplugindir)/plug-ins

scripts = \
	python-eval.py

pluginexec_SCRIPTS = $(scripts)

# python-console has a data file.
# Therefore let's move it to its own sub-directory.
consoleexecdir = $(gimpplugindir)/plug-ins/python-console
console_scripts = python-console.py
consoleexec_SCRIPTS = $(console_scripts)
dist_consoleexec_DATA = pyconsole.py

EXTRA_DIST = $(scripts) $(test_scripts) $(console_scripts)
