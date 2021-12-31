#!/bin/sh

docker run --rm -it -v $(pwd):/sources docker.io/pfeerick/infinitime-build
