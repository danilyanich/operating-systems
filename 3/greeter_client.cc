#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "func_calc.grpc.pb.h"
#else

#include "func_calc.grpc.pb.h"

#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using func_calc::FuncCalc;
using func_calc::HelloReply;
using func_calc::HelloRequest;

class FuncCalcClient {
public:
    FuncCalcClient(std::shared_ptr <Channel> channel)
            : stub_(FuncCalc::NewStub(channel)) {}

    double SayHello(double x) {
        HelloRequest request;
        request.set_x(x);

        HelloReply reply;
        ClientContext context;

        Status status = stub_->SayHello(&context, request, &reply);

        if (!status.ok()) {
            throw std::domain_error("got non ok response: " + status.error_message());
        }

        return reply.y();
    }

private:
    std::unique_ptr <FuncCalc::Stub> stub_;
};

std::string getAddress(int argc, char **argv) {
    std::string server_address;
    if (argc != 2) {
        return "localhost:8888";
    }
    return std::string(argv[1]);
}

int main(int argc, char **argv) {
    FuncCalcClient greeter(
            grpc::CreateChannel(getAddress(argc, argv),
    grpc::InsecureChannelCredentials())
    );

    double res = 0;
    double x = 10;
    try {
        res = greeter.SayHello(x);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << "Result of sigmoid function with x=" << x << " result is " << res << std::endl;
}
