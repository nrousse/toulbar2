
Following https://tech.michaelaltfield.net/2020/07/18/sphinx-rtd-github-pages-1/

#------------------------------------------------------------------------------
Project :

Documentation :
/home/nrousse/workspace_git/github_nrousse/toulbar2/docs

C++ code :
/home/nrousse/workspace_git/github_nrousse/toulbar2/src

python code :
/home/nrousse/workspace_git/github_nrousse/toulbar2/pytoulbar2

#------------------------------------------------------------------------------
INIT sphinx doc

- INSTALL sphinx
sudo apt-get install -y python3-sphinx
sudo apt-get install -y python3-sphinx-rtd-theme

- setup sphinx site  :
cd docs
sphinx-quickstart
...answers...

- save original files :
cp conf.py conf.py.orig
cp index.rst index.rst.orig

- conf.py :
  change theme from 'alabaster' to 'sphinx_rtd_theme'
  python code path

- autodoc.rst (a enlever ?)

#------------------------------------------------------------------------------
CI/CD

docs_pages_workflow.yml
docs/buildDocs.sh

#------------------------------------------------------------------------------

