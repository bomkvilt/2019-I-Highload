FROM ubuntu:18.10

WORKDIR /home/server

RUN sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test; \
    sudo apt-get -y update;                                 \
    sudo apt-get -y install gcc-8 g++-8;                    \
    sudo apt-get -y install libboost-all-dev;               \
    sudo update-alternatives --remove-all gcc;              \
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 80 --slave /usr/bin/g++ g++ /usr/bin/g++-8;

ADD . .

RUN git clone --recurse-submodules -j8 https://github.com/bomkvilt/2019-I-Highload.git directory; \
    cd ./directory;         \
    ./1_gen_project.sh;     \
    ./2_build_project.sh;

EXPOSE 8080

CMD ./3_run_project.sh