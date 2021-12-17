
# Sphinx doc hosted on GitHub Pages and built with GitHub free CI/CD tool

Sources 
=======

docs_pages_workflow.yml and buildDocs.sh from
https://tech.michaelaltfield.net/2020/07/18/sphinx-rtd-github-pages-1/

Documentation of C++ code by Sphinx is possible

Project
=======

  - Documentation : toulbar2/docs

  - C++ code : toulbar2/src

  - python code : toulbar2/pytoulbar2

INIT/CREATE Sphinx doc
======================

- INSTALL sphinx

  sudo apt-get install -y python3-sphinx

  sudo apt-get install -y python3-sphinx-rtd-theme

- setup sphinx site  :

  cd docs

  sphinx-quickstart

  ...answers... (see 'Sources')

- save original files :

  cp conf.py conf.py.orig

  cp index.rst index.rst.orig

- conf.py :

  change theme from 'alabaster' to 'sphinx_rtd_theme'

  python code path

CI/CD
=====

Files :

  - .github/workflows/docs_pages_workflow.yml

  - docs/buildDocs.sh

+ On the repo : "Settings" / "Pages" choose 'gh-pages branch' under "Source"

CI/CD run :

   When push

CI/CD result :

  => Doc online at : **https://nrousse.github.io/toulbar2**

  => doc.pdf : ?

LOCAL generate
==============

See docs/local/install.txt to install pyvenv python virtual environment

Commands :

  source _pyvenv/bin/activate

  make clean

  make html

  make rinoh

Productions :

  _build/html/index.html

  _build/rinoh/toulbar2.pdf

#------------------------------------------------------------------------------

