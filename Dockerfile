FROM gcc:13 AS build
WORKDIR /app
COPY main.cpp .
RUN g++ -O2 -std=c++17 -pthread main.cpp -o simulator

FROM debian:stable-slim
WORKDIR /app
COPY --from=build /app/simulator .
COPY index.html .
CMD ["./simulator"]
