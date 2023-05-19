#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include <iosfwd>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/user.h>
#include <unwindstack/Unwinder.h>
#include <unwindstack/AndroidUnwinder.h>

int main() {
    using namespace std;
    using namespace unwindstack;
    auto tid = getpid();
    auto pid = getppid();
    printf("tid: %d, pid: %d\n", tid, pid);
    AndroidLocalUnwinder unwinder;
    unwindstack::AndroidUnwinderData data;
    bool result = unwinder.Unwind(tid, data);
    if (!result) {
        printf("unwind failed: %s", data.GetErrorString().c_str());
        return 1;
    }
    auto& os = std::cout;
    data.DemangleFunctionNames();
    for (const auto& frame : data.frames) {
        os << unwinder.FormatFrame(frame) << "\n";
    }
    return 0;
}
