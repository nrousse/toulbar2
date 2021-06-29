###############################################################################
# 
#                       Dockerfile container 
# 
#           dedicated to pytoulbar2 Python API of toulbar2
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
#   and once inside : source /WORK.sh
#
# Note :
#
# - To build pytoulbar2.simg from pytoulbar2-docker image :
#   sudo singularity build -F pytoulbar2.simg docker-daemon://pytoulbar2-docker:latest
#
###############################################################################

FROM debian:buster-slim

# !! RUN a MODIFIER (pip install...)

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

# 'git clone' remains ?
RUN git clone https://github.com/toulbar2/toulbar2.git
RUN cd toulbar2 \
&& mkdir build \
&& cd build \
&& cmake -DPYTB2=ON .. \
&& make

RUN cp /toulbar2/build/lib/Linux/pytb2.cpython-37m-x86_64-linux-gnu.so /toulbar2/pytoulbar2/pytb2.cpython-37m-x86_64-linux-gnu.so

RUN CMDFILE=/WORK.sh \
&& echo "#!/bin/bash" > $CMDFILE \
&& chmod 755 $CMDFILE \
&& echo "#####################################################" >> $CMDFILE \
&& echo "# Init to work into /WORK folder with (py)toulbar2   " >> $CMDFILE \
&& echo "#####################################################" >> $CMDFILE \
&& echo "PATH=\$PATH:/toulbar2/build/bin/Linux:/toulbar2/src:/WORK" >> $CMDFILE \
&& echo "ln -s /toulbar2/pytoulbar2 /WORK/pytoulbar2"           >> $CMDFILE \
&& echo "cd /WORK"                                              >> $CMDFILE


###############################################################################

