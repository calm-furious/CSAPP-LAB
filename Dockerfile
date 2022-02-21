FROM ubuntu
#COPY . /csapp-labs
WORKDIR /
ARG DEBIAN_FRONTEND=noninteractive
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8
RUN apt update \
	&& apt install -y gcc make vim gdb curl wget python3 python3-pip valgrind
RUN pip install keystone-engine unicorn capstone ropper
RUN wget -O ~/.gdbinit-gef.py -q http://gef.blah.cat/py
RUN echo source ~/.gdbinit-gef.py >> ~/.gdbinit

# docker build -t csapp .
# docker run --name attack -d -v "$(pwd)":/lab --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it csapp /bin/bash
# docker run --name cache -d -v "$(pwd)":/lab --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it csapp /bin/bash