
FROM gcc:14

WORKDIR /app

COPY backened/ ./backened/
COPY public/ ./public/

RUN g++ -std=c++17 -pthread backened/final.cpp -o inventory_server

EXPOSE 8080

CMD ["./inventory_server"]
