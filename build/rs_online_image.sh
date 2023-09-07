##This scripts is to build geb rs online environment runtime images
##based on dockerfile in the current directory.

#!/bin/bash

current_dir=$(pwd)
echo "current_dir: ${current_dir}"

project_dir=${current_dir}/../..
echo "project_dir: ${project_dir}"


## default variable value ##
region=
name=rs_algorithm_process
version_major=v1
version_minor=2
tar_name=${name}

## parse script arguments ##
while getopts "n:a:i:t:" opt; do
  case $opt in
    n)
      name=$OPTARG
      ;;
    a)
      version_major=$OPTARG
      ;;
    i)
      version_minor=$OPTARG 
      ;;
    t)
      tar_name=$OPTARG
      ;;
  esac
done


## print arguments value ##
echo
echo "###################################################" 
echo "region : ${region}"
echo "name : ${name}"
echo "version_major : ${version_major}"
echo "version_minor : ${version_minor}"
echo "tar_name : ${tar_name}"
echo "###################################################"
echo 

project_name=GEBOperatorProcess

code_dir=${project_dir}/${project_name}
echo "code_dir: ${code_dir}"

if [ ! -d ${code_dir} ]; then
  echo "${code_dir} is not exist"
  exit 1
fi


dockerfile=${code_dir}/build/Dockerfile
echo "dockerfile: ${dockerfile}"

if [ ! -f ${dockerfile} ]; then
  echo "${dockerfile} is not exist"
  exit 1
fi


## determine whether region variable is empty ##
if [[ ! ${region} ]]; then
	echo "region variable is empty."
	image_name="${name}:${version_major}.${version_minor}"
else
	echo "region variable is not empty."
	image_name="${region}/${name}:${version_major}.${version_minor}"
fi

echo "image_name: ${image_name}"


cd ${project_dir}

sudo docker build -t ${image_name} -f ${dockerfile} .

build_ret=$?

if [[ ${build_ret} -ne 0 ]];then
	echo "Failed to build ${image_name}"
	exit ${build_ret}
else
	echo "Success to build ${image_name}"
fi


## docker tag ##
#docker_tag=ip:port/rs_algorithm_process:v1.2
#sudo docker tag ${image_name} ${docker_tag}

## docker push ##
#sudo docker push ${docker_tag}

#push_ret=$?
#if [[ ${push_ret} -ne 0 ]]; then
#	echo "Failed to push ${docker_tag}"
#	exit ${push_ret}
#else
#	echo "Success to push ${docker_tag}"
#fi

echo 
echo "finish to build rs online images."

