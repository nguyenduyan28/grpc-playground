#include <grpcpp/support/status.h>
#include "absl/strings/str_format.h"
#include "chat.grpc.pb.h"
#include "chat.pb.h"
#include "grpcpp/server_builder.h"
#include "grpcpp/server_context.h"
#include <chrono>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include "grpcpp/ext/proto_server_reflection_plugin.h"
#include "cstdint"


using chat::ChatService;
using chat::ChatMsg;
using grpc::Status;
using grpc::ServerContext;
using grpc::ServerBuilder;
using grpc::Server;



class ChatServiceImpl final : public ChatService::Service{
  Status SendMessage(ServerContext* context, const ChatMsg * request, ChatMsg * response) override{
    const auto time_stamp = std::chrono::system_clock::now();
    std::string name = request -> name();
    response -> set_name("Server");
    response -> set_text(response -> name(), "Message send sucess from: " + name + ", at: ", time_stamp);
    return Status::OK;
  }
};

void run_server(uint16_t port){
  const std::string server_address{absl::StrFormat("0.0.0.0%d", port)};
  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  ChatServiceImpl service;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
    // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char **argv){
  try{
    uint16_t port{};
    port = static_cast <uint16_t> (std::stoul(argv[1]));
    run_server(port);
  } catch (const std::invalid_argument &e){
    std::cerr << "Error: Invalid argument - " << e.what() << '\n';
  } 
}