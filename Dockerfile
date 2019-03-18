FROM ubuntu:18.10

WORKDIR /home/server

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update; \
    apt-get install -y software-properties-common; \
    add-apt-repository ppa:apt-fast/stable; \
    apt-get -y install apt-fast; \
    apt-fast update;

RUN apt-fast install -y git cmake gcc-8 g++-8 libboost-all-dev; \
    update-alternatives --remove-all gcc; \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 80 --slave /usr/bin/g++ g++ /usr/bin/g++-8;

WORKDIR /home/server

RUN git clone --recursive https://github.com/bomkvilt/2019-I-Highload.git;
RUN cd ./2019-I-Highload; \
    git submodule update; \
    /bin/bash ./1_gen_project.sh; \
    /bin/bash ./2_build_project.sh;

EXPOSE 80

CMD cd ./2019-I-Highload; \
    /bin/bash ./3_run_project.sh;