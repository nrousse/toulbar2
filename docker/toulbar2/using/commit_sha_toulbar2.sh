#!/bin/bash

###############################################################################
# Script to launch in Docker image command to know toulbar2 sha commit
###############################################################################

source /WORK.sh

export PATH

# run
cd /toulbar2
git log --pretty=format:'%H' -n 1

