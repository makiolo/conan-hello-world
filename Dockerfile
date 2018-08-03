FROM makiolo/linux-x64
ENV PYTHONUNBUFFERED 1
RUN echo 'deb http://ftp.us.debian.org/debian testing main contrib non-free' > /etc/apt/sources.list.d/gcc.testing.list
RUN apt-get update
RUN apt-get install -y -t testing g++
WORKDIR /work
ADD requirements.txt /work
RUN pip install -r requirements.txt

