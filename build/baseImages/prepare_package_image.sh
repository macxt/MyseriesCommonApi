##This scripts is to prepare some folders and files for packaging algorithm images.
##include copy from other folders and modify file content.

#!/bin/bash

current_dir=$(pwd)
echo "current_dir: ${current_dir}"

##define some variables.
from_folder=hc_water
to_folder=

#parse scripts parameters with getopts built-in shell tools
#f:specify from_folder from which files and scripts will be copied.
#t:specify to_folder where files and scripts will be copied to.
#h:output some help info
while getopts "f:t:h" opt; do
  case $opt in
    f)
      from_folder=$OPTARG
      ;;
    t)
      to_folder=$OPTARG
      ;;
    h)
      echo -e "\nUsage: ./prepare_package_image.sh -f srcFolder -t dstFolder."
      echo -e "Must specify -t parameter value when use this scripts.\n"
      exit 0
      ;;
  esac
done

## determine whether to_folder variable is empty ##
if [[ ! ${to_folder} ]]; then
	echo "to_folder variable is empty. Failed!"
	echo "Must specify -t parameter value."
	exit 1
fi

from_dir=${current_dir}/${from_folder}
echo "from_dir: ${from_dir}"

to_dir=${current_dir}/${to_folder}
echo "to_dir: ${to_dir}"

## determine from_dir exists and create to_dir.
if [[ ! -d ${from_dir} ]]; then
  echo "${from_dir} is not exist"
  exit 1
fi

mkdir -p ${to_dir}

## copy Dockfile Miniconda_xxx.sh service.tar.gz execute_image.sh from from_dir to to_dir.
docker_file=Dockerfile
conda_script=Miniconda3-py38_23.1.0-1-Linux-x86_64.sh
service_tar=service.tar.gz
execute_script=execute_package_image.sh

cp ${from_dir}/${docker_file} ${to_dir}
cp ${from_dir}/${conda_script} ${to_dir}
cp ${from_dir}/${service_tar} ${to_dir}
cp ${from_dir}/${execute_script} ${to_dir}

echo "copy from from_dir to to_dir done."

## create ${to_folder}_process folder in ${to_dir}
alg_package=${to_dir}/${to_folder}_process
mkdir -p ${alg_package}

## scp -r maxl@192.168.53.3:/home/maxl/Append/rs_testdata/tmp_exefile/* ${alg_package}

scp -r maxl@192.168.53.3:/home/maxl/Append/rs_testdata/tmp_exefile/* ${alg_package}

echo -e "preparation work done before package images.\n"

