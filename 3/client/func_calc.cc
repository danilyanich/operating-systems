#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

#ifdef BAZEL_BUILD
#include "func_calc.grpc.pb.h"
#else

#include "func_calc.grpc.pb.h"

#endif

using func_calc::CalculateReply;
using func_calc::CalculateRequest;
using func_calc::FuncCalc;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class FuncCalcClient {
   public:
    FuncCalcClient(std::shared_ptr<Channel> channel)
        : stub(FuncCalc::NewStub(channel)) {}

    double CalcSigmoidFunc(double x) {
        CalculateRequest request;
        request.set_x(x);

        CalculateReply reply;
        ClientContext context;

        Status status = stub->CalculateSigmoidFunc(&context, request, &reply);

        if (!status.ok()) {
            throw std::domain_error("got non ok response: " +
                                    status.error_message());
        }

        return reply.y();
    }

   private:
    std::unique_ptr<FuncCalc::Stub> stub;
};

std::string getAddress(int argc, char **argv) {
    if (argc != 2) {
        return "localhost:8888";
    }
    return std::string(argv[1]);
}

int main(int argc, char **argv) {
    FuncCalcClient calculator(grpc::CreateChannel(
        getAddress(argc, argv), grpc::InsecureChannelCredentials()));

    double res = 0;
    double x = 0;
    std::cout<<"Please tap your x value: ";
    std::cin >> x;
    try {
        res = calculator.CalcSigmoidFunc(x);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    std::cout << "Result of sigmoid function with x=" << x << " result is "
              << res << std::endl;
}
