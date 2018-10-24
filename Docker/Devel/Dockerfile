FROM kaajo/webcamcap:latest

MAINTAINER mirokrajicek@gmail.com

USER root

RUN apt-get -qq update && DEBIAN_FRONTEND=noninteractive apt-get install -qq -y terminator qt59creator

USER wcc

CMD sudo chgrp video /dev/video* && terminator #chgrp fix for older dockers https://github.com/docker/for-linux/issues/228
