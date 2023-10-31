##This scripts is to execute package image, tag and push target images.
##based on the Dockerfile under current directory.

#!/bin/bash

current_dir=$(pwd)
echo "current_dir: ${current_dir}"

alg_name=$(basename ${current_dir})
echo "alg_name: ${alg_name}"

alg_package=${alg_name}_process
echo "alg_package: ${alg_package}"

##define some variables.
image_name=
version_major=v1
version_minor=2
version_amend=0
#remote_repository=harbor.hyperdl.cn:180
remote_repository=10.1.108.166:8081
#environment_name=geovis-prd
environment_name=geors

#parse scripts parameters with getopts built-in shell tools
#i:specify image name.
#m:specify image major version.
#n:specify image minor version.
#a:specify image amend version.
#r:specify remote repository name.
#e:specify environment name.
#h:output some brief information.
while getopts "i:m:n:a:r:e:h" opt; do
  case $opt in
    i)
      image_name=$OPTARG
      ;;
    m)
      version_major=$OPTARG
      ;;
    n)
      version_minor=$OPTARG 
      ;;
    a)
      version_amend=$OPTARG 
      ;;
    r)
      remote_repository=$OPTARG
      ;;
    e)
      environment_name=$OPTARG
      ;;
    h)
      echo -e "\nUsage: ./execute_package_image.sh -i image_name."
      echo -e "-i image_name (required)."
      echo -e "-m image major version (optional)."
      echo -e "-n image minor version (optional)."
      echo -e "-a image amend version (optional)."
      echo -e "-r remote repository name including domain name and port for pushing (optional)."
      echo -e "-e environment name for image using (optional).\n"
      exit 0
      ;;
  esac
done

## determine whether image_name variable is empty ##
if [[ ! ${image_name} ]]; then
	echo "image_name variable is empty. Failed!"
	echo -e "Must specify -i image_name parameter value.\n"
	exit 1
fi

image_full=${image_name}:${version_major}.${version_minor}.${version_amend}

image_tag=${remote_repository}/${environment_name}/geovis_${image_full}

## print arguments value ##
echo
echo "###################################################" 
echo "image_name : ${image_name}"
echo "version_major : ${version_major}"
echo "version_minor : ${version_minor}"
echo "version_amend : ${version_amend}"
echo "image_full : ${image_full}"
echo "image_tag : ${image_tag}"
echo "###################################################"
echo 

## docker build 

docker build -t ${image_full} --build-arg ALG_FOLDER=${alg_package} .

build_res=$?

if [[ ${build_res} -ne 0 ]];then
	echo -e "Failed to build ${image_full}.\n"
	exit ${build_res}
else
	echo -e "Success to build ${image_full}.\n"
fi

## docker tag and docker push

docker tag ${image_full} ${image_tag}

docker push ${image_tag}

push_res=$?

if [[ ${push_res} -ne 0 ]];then
	echo -e "Failed to push ${image_tag}.\n"
	exit ${push_res}
else
	echo -e "Success to push ${image_tag}.\n"
fi

echo -e "build and push target image with dockerfile done.\n"
