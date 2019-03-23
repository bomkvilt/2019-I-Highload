FROM ubuntu:18.10

WORKDIR /home/server

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update; \
    apt-get install -y software-properties-common; \
    add-apt-repository ppa:apt-fast/stable; \
    apt-get -y install apt-fast;

RUN apt-fast install -y git cmake gcc-8 g++-8 libboost-all-dev; \
    update-alternatives --remove-all gcc; \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 80 --slave /usr/bin/g++ g++ /usr/bin/g++-8;

WORKDIR /home/server/build

# RUN git clone --recursive https://github.com/bomkvilt/2019-I-Highload.git ./;
COPY . .
RUN rm -rf ./build
RUN ./3_gen_project.sh -p -g -b;

EXPOSE 80

CMD /bin/bash ./3_gen_project.sh -r;