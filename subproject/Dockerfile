FROM debian:bookworm-slim

WORKDIR /conan_v1_create_failure

RUN apt update && apt install -y cmake python3 python3-pip python3-venv clang clang-tools clang-format clang-tidy

# Use 1.61.0 for v1 or 2.0.10 for v2
ARG CONAN_VERSION=1.61.0

ENV VIRTUAL_ENV=/opt/venv
RUN python3 -m venv /opt/venv
ENV PATH="${VIRTUAL_ENV}/bin:$PATH"

RUN pip3 install --upgrade pip && pip3 install conan==${CONAN_VERSION}

COPY . ./

SHELL ["/bin/bash", "-c"]
RUN if [[ $(cut -c 1-1 <<< "${CONAN_VERSION}") > "1" ]]; \
    then \
        conan profile detect && \
        echo "conan create . --version=0.0.0 --build=missing" >> conan_create.sh; \
    else \
        conan profile new default --detect && \
        echo "conan create . 0.0.0@ --build=missing" >> conan_create.sh; \
    fi
RUN chmod +x conan_create.sh

CMD ./conan_create.sh