#This Dockerfile is to build geb runtime images for pushing online environment.
#Automatically compile project code before building final images.

#step1:compile project code
FROM myb_baseimages:v1.2 AS project-compile-result

#some infomation about runtime-images
LABEL maintainer="myb@geovis.com.cn" name="geb_algorithm_process_images:v1.2" deccription="geovis earth brain algorithm process analysis images_v1.2"

WORKDIR /opt/code/geb_operator_process
COPY GEBOperatorProcess .

RUN ln -s /usr/bin/g++-9 /usr/bin/g++ && cd build/ && ./build.sh -c || exit 1

#step2:build finally runtime images
FROM myb_baseimages:v1.2

WORKDIR /opt/geb_analysis

COPY --from=project-compile-result /opt/code/geb_operator_process/package/ .

WORKDIR /opt/geb_analysis/bin

#set environment variable
ENV PATH=/opt/geb_analysis/bin:$PATH LD_LIBRARY_PATH=/usr/lib:/usr/local/lib:/opt/geb_analysis/lib LANG=C.UTF-8

