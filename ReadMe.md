task: https://github.com/init/http-test-suite

sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc g++ gcc-5 g++-5 gcc-6 g++-6 gcc-7 g++-7 g++-8
sudo update-alternatives --remove-all gcc

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 50 --slave /usr/bin/g++ g++ /usr/bin/g++-5
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 70 --slave /usr/bin/g++ g++ /usr/bin/g++-7
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 80 --slave /usr/bin/g++ g++ /usr/bin/g++-8

sudo update-alternatives --config gcc

sudo apt-get install git libboost-all-dev


git clone --recurse-submodules -j8 https://github.com/bomkvilt/2019-I-Highload.git

cd 2019-I-Highload
./1_gen_project.sh
