#include <iostream>

#include "ExpressionResolver.cpp"

using namespace std;

void setGreenColor() {
    cout << "\033[0;32m";
}

void setRedColor() {
    cout << "\033[0;31m";
}

void setDefaultColor() {
    cout << "\033[0m";
}

void printActionResult(ActionResult *result) {
    result->isSuccessful() ? setGreenColor() :setRedColor();
    cout << result->getMessage() << endl;
    setDefaultColor();
}

ExpressionResolver *getExpressionResolver(int totalMemory, int capacityOfCluster) {
    auto *interfaceFileSystem = new InterfaceFileSystem(totalMemory);
    auto *interfaceCluster = new InterfaceCluster(capacityOfCluster, totalMemory);
    auto *readWriteService = new ReadWriteService();
    readWriteService->setInterfaceCluster(interfaceCluster);
    auto *dumpService = new DumpService();
    dumpService->setInterfaceCluster(interfaceCluster);
    auto *service = new FileSystemService();
    service->setInterfaceFileSystem(interfaceFileSystem);
    service->setInterfaceCluster(interfaceCluster);
    service->setReadWriteService(readWriteService);
    service->setDumpService(dumpService);
    return new ExpressionResolver(service);
}

int main() {
    int totalMemory = 256;
    int capacityOfCluster = 8;
    ExpressionResolver *resolver = getExpressionResolver(totalMemory, capacityOfCluster);
    bool isActive = true;
    while (isActive) {
        try {
            string expression;
            getline(cin, expression);
            Action *action = resolver->resolve(expression);
            ActionResult *result = action->execute();
            printActionResult(result);
            delete action;
            delete result;
        } catch (IncorrectInputException &exception) {
            setRedColor();
            cout << exception.what() << endl;
            setDefaultColor();
        }
    }
    return 0;
}