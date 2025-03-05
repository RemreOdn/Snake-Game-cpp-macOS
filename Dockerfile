# Ubuntu 22.04 tabanlı bir Docker imajı kullan
FROM ubuntu:22.04 as builder

# Paketleri güncelle ve gerekli bağımlılıkları yükle
RUN apt update && apt install -y \
    cmake \
    g++ \
    make \
    ninja-build \
    lldb \
    clang

# Çalışma dizini
WORKDIR /workspace

COPY src src
COPY CMakeLists.txt .

WORKDIR /workspace/build
RUN cmake .. && make

FROM ubuntu:22.04 as runtime
COPY --from=builder /workspace/build/snake_game .
CMD ["./snake_game"]




