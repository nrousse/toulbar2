###############################################################################
# 
#              Dockerfile container for pytoulbar2 software
# 
#             including toulbar2 and its pytoulbar2 Python API
# 
# Build -----------------------------------------------------------------------
#
# To build pytoulbar2-docker image :
#
#   docker build -t pytoulbar2-docker:latest .
#
# Work inside -----------------------------------------------------------------
#
# Then to work inside pytoulbar2-docker image, under /WS 
# (as local folder to remaining results) :
#
#   docker run -v $PWD:/WS -ti pytoulbar2-docker /bin/bash
#
#   and once inside : source /WS.sh
#
# Note :
#
# - To build pytoulbar2.simg from pytoulbar2-docker image :
#   sudo singularity build -F pytoulbar2.simg docker-daemon://pytoulbar2-docker:latest
#
###############################################################################

FROM debian:buster-slim

RUN apt-get update -yq \
&& apt-get install git-core -y \
&& apt-get install vim -y \
&& apt-get install cmake -y \
&& apt-get install g++ -y \
&& apt-get install libgmp-dev -y \
&& apt-get install libboost-graph-dev -y \
&& apt-get install libboost-iostreams-dev -y \
&& apt-get install zlib1g-dev -y \
&& apt-get install liblzma-dev -y \
&& apt-get install libxml2-dev -y \
&& apt-get install libopenmpi-dev -y \
&& apt-get install libjemalloc-dev -y \
&& apt-get install pkg-config -y \
&& apt-get install python3 -y \
&& apt-get install python3-pip -y \
&& pip3 install pybind11 \
&& apt-get clean -y

###############################################################################

