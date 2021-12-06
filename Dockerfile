FROM ubuntu
COPY . /csapp-labs
WORKDIR /
RUN sed  -i "s/archive.ubuntu.com/mirrors.aliyun.com/g" /etc/apt/sources.list \
	&& apt update \
	&& apt install -y gcc make vim gdb
