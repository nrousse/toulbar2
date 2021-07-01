###############################################################################
# 
#                       Dockerfile container 
# 
#           dedicated to pytoulbar2 Python API of toulbar2
# 
# Desc ------------------------------------------------------------------------
# 
# Installation with python3 -m pip install pytoulbar2
#              pip3 install --cache-dir ... -t ... --upgrade pytoulbar2
# 
# Build -----------------------------------------------------------------------
#
# To build pytoulbar2-docker image :
#
#   docker build -t pytoulbar2-docker:latest .
#
# Work inside -----------------------------------------------------------------
#
# Then to work inside pytoulbar2-docker image, under /WORK 
# (as local folder to remaining results) :
#
#   docker run -v $PWD:/WORK -ti pytoulbar2-docker /bin/bash
#
# Note :
#
# - To build pytoulbar2.simg from pytoulbar2-docker image :
#   sudo singularity build -F pytoulbar2.simg docker-daemon://pytoulbar2-docker:latest
#
###############################################################################

#FROM debian:buster-slim
FROM ubuntu-latest

RUN apt-get update -yq \
&& apt-get install git-core -y \
&& apt-get install vim -y \
&& apt-get install python3 -y \
&& apt-get install python3-pip -y \
&& apt-get clean -y

RUN mkdir /WORK \
&& export PYTHONPATH=$PYTHONPATH:/WORK:/WORK/pythonplus \
&& mkdir /WORK/pythonplus \
&& mkdir /WORK/cache \
&& cd /WORK

#RUN python3 -m pip install pytoulbar2
#RUN pip3 install --cache-dir /WORK/cache -t /WORK/pythonplus --upgrade pytoulbar2
RUN python3 -m pip install --cache-dir /WORK/cache -t /WORK/pythonplus --upgrade pytoulbar2

RUN rm -fr /WORK/pythonplus \
&& rm -fr /WORK/cache


###############################################################################

