## Task: 
https://github.com/init/http-test-suite

## Usage:
    docker build -t hld  .
    docker run -p 80:80 --name hld hld

## Local tests:
    # setup test enviroment
    git clone https://github.com/init/http-test-suite.git build/sute;
    cp ./config.conf ./build/sute/httpd.conf

    # containers
    docker run -p 80:80 -v $(pwd)/build/sute/httpd.conf:/etc/httpd.conf:ro -v $(pwd)/build/sute:/var/www/html:ro --name hld -t hld;
    docker ps -a;
    docker rm hld;

    # tests invocation
    ./build/sute/httptest.py
