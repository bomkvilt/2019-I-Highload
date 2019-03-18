task: https://github.com/init/http-test-suite

sudo docker build -t hld  .
sudo docker run -p 80:80 --name aa hld

sudo docker ps -a
sudo docker rm aa
