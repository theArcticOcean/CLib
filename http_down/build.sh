#qcc -Vgcc_ntoarmv7le -Wl,--no-keep-memory  -lang-c++ -lm -Wl,--rpath=. -lcurl -L./ -o main  http_request_mgr.cpp main.cpp SignatureGenerator.cpp
g++ -lcurl -L./ -o main http_request_mgr.cpp main.cpp SignatureGenerator.cpp
