FROM debian:bookworm-slim

WORKDIR /conan2_pkg_test

RUN apt update && apt install -y cmake python3 python3-pip python3-venv clang clang-tools clang-format clang-tidy

# Use 1.61.0 for v1 or 2.0.10 for v2
ARG CONAN_VERSION=2.0.10

ENV VIRTUAL_ENV=/opt/venv
RUN python3 -m venv /opt/venv
ENV PATH="${VIRTUAL_ENV}/bin:$PATH"

RUN pip3 install --upgrade pip && pip3 install conan==${CONAN_VERSION}

COPY . ./

ARG USE_NANODBC=OFF
ARG USE_CURL=OFF
ARG USE_SUBPROJECT=ON

SHELL ["/bin/bash", "-c"]
RUN if [[ $(cut -c 1-1 <<< "${CONAN_VERSION}") > "1" ]]; \
    then \
        conan profile detect && \
        conan create subproject/ --version=0.0.0 --build=missing; \
    elif [[ ${USE_SUBPROJECT} == "ON" ]]; \
    then \
        conan profile new default --detect && \
        conan create subproject/ 0.0.0@ --build=missing; \
    fi

RUN mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Debug -DWITH_NANODBC=${USE_NANODBC} -DWITH_CURL=${USE_CURL} -DWITH_SUBPROJECT=USE_SUBPROJECT ..
RUN cmake --build build --target all --config Debug --
RUN ./build/sources/conan2_pkg_test_Runner 

CMD cd build && cpack -G TGZ