FROM ubuntu:20.04

RUN apt update

# For installing gcc
RUN apt install -y build-essential

# Utility so that we can see who binds to what port (lsof -i:<PORT_NUM>)
RUN apt install -y lsof