#include <iostream>
#include <memory>
#include <string>
#include <cmath>
#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "func_calc.grpc.pb.h"
#else

#include "func_calc.grpc.pb.h"

#endif

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using func_calc::FuncCalc;
using func_calc::CalculateRequest;
using func_calc::CalculateReply;

class FuncCalcService final : public FuncCalc::Service {
    Status CalculateSigmoidFunc(ServerContext *context, const CalculateRequest *request,
                                CalculateReply *reply) override {
        reply->set_y(1 / (1 + pow(exp(1), -request->x())));
        return Status::OK;
    }
};

std::string getServerAddress(int argc, char **argv) {
    if (argc == 2) {
        return argv[1];
    }
    return "0.0.0.0:8888";
}

void RunServer(const std::string &server_address) {
    FuncCalcService service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr <Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char **argv) {
    auto addr = getServerAddress(argc, argv);
    RunServer(addr);
    return 0;
}
