set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

target_link_libraries(server PRIVATE protobuf::libprotobuf gRPC::grpc++)