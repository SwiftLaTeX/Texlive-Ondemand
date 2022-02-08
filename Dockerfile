FROM ubuntu:20.04
RUN   apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -q -y wget \
    texlive-full \
    python3 \
    python3-pip \
    git
COPY . /app
RUN pip3 install -r /app/requirements.txt && echo "0.5"
WORKDIR /app
CMD ["python3", "wsgi.py"]
