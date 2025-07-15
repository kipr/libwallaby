#FROM arm64v8/python:3.11-bookworm
FROM python:3.9-bullseye
LABEL authors="Tobias Madlberger"

WORKDIR /app

RUN apt-get update && apt-get install -y build-essential cmake git gcc-aarch64-linux-gnu g++-aarch64-linux-gnu gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf swig doxygen

COPY . .

RUN chmod +x build.sh

RUN ./build.sh
