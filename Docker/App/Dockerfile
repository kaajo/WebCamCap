FROM dymat/opencv:latest

MAINTAINER mirokrajicek@gmail.com

USER root

# install deps
##system
RUN apt-get -qq update && DEBIAN_FRONTEND=noninteractive apt-get install -qq -y sudo curl locales build-essential git software-properties-common && add-apt-repository -y ppa:beineri/opt-qt596-xenial
## application
RUN apt-get -qq update && DEBIAN_FRONTEND=noninteractive apt-get install -qq -y qt59charts-no-lgpl libglm-dev libglu1-mesa-dev libtbb-dev
## fbxsdk
RUN curl http://download.autodesk.com/us/fbx/2019/2019.0/fbx20190_fbxsdk_linux.tar.gz --output fbx20190_fbxsdk_linux.tar.gz \ 
    && mkdir fbx20190_fbxsdk_linux && tar -xzf fbx20190_fbxsdk_linux.tar.gz -C fbx20190_fbxsdk_linux && cd fbx20190_fbxsdk_linux \
    && yes yes | ./fbx20190_fbxsdk_linux /usr \
    && cd .. && rm fbx20190_fbxsdk_linux.tar.gz && rm -rf fbx20190_fbxsdk_linux
##weird library path but... Autodesk
ENV LD_LIBRARY_PATH=/usr/lib/gcc4/x64/release:$LD_LIBRARY_PATH

# create user wcc
RUN useradd --create-home --shell=/bin/bash wcc
RUN echo "wcc:wccpass" | chpasswd
RUN usermod -aG sudo wcc
RUN echo "wcc ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

#camera permissions
RUN usermod -a -G video root
RUN usermod -a -G video wcc

# set file system permitions
RUN chown -R wcc:wcc /home/wcc

USER wcc

ENV QT_BASE_DIR=/opt/qt59
ENV QTDIR=$QT_BASE_DIR
ENV PATH=$QT_BASE_DIR/bin:$PATH
ENV LD_LIBRARY_PATH=$QT_BASE_DIR/lib/x86_64-linux-gnu:$QT_BASE_DIR/lib:$LD_LIBRARY_PATH
ENV PKG_CONFIG_PATH=$QT_BASE_DIR/lib/pkgconfig:$PKG_CONFIG_PATH
#ENV QT_X11_NO_MITSHM=1 #rendering fix for qv4l2

RUN cd /home/wcc && git clone https://github.com/kaajo/WebCamCap.git && mkdir build && cd build && /opt/qt59/bin/qmake -o Makefile ../WebCamCap/WebCamCap.pro -spec linux-g++ && make qmake_all && make sub-WebCamCap2
RUN cd /home/wcc/build && sudo make install

ENV HOME=/home/wcc
CMD sudo chgrp video /dev/video* && /home/wcc/build/WebCamCap2/WebCamCap2 #chgrp fix for older dockers https://github.com/docker/for-linux/issues/228
