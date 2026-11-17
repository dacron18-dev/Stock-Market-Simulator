FROM gcc:13-bookworm AS build

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    libcurl4-openssl-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

COPY main.cpp .

RUN g++ -O2 -std=c++17 -pthread main.cpp -lcurl -o simulator

FROM gcc:13-bookworm

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
    libcurl4 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /app/simulator .
COPY index.html .

CMD ["./simulator"]
