FROM    ubuntu:18.04
RUN     apt-get update
RUN     apt-get install -y g++ build-essential cmake

EXPOSE 8080