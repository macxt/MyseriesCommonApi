# This file aims to bulid rs_baseimages:v1.3

FROM myb_baseimages:v1.2

# maintainer
LABEL maintainer="myb@geovis.com.cn" name="rs_baseimages:v1.3" deccription="install build-essential libhdf4-dev python-dev tools based on v1.2"

# set work dir
WORKDIR /opt/earth_brain

# copy dockerfiles to current doc folder
COPY Dockerfile* ./doc/

# update software list and upgrade software installed  
RUN apt-get update -y && apt-get upgrade -y 

# additional install build-essential libhdf4-dev python-dev tools 2023/09/01
RUN apt-get install -y build-essential && apt-get install -y libhdf4-dev && apt-get install -y python-dev 

# set environment variable
ENV PATH=/opt/earth_brain/bin:$PATH LD_LIBRARY_PATH=/usr/lib:/usr/local/lib:/opt/earth_brain/lib

# set encoding type utf-8 in container
ENV LANG C.UTF-8

