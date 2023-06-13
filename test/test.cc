#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include <iosfwd>
#include <thread>
#include <unistd.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/user.h>
#include <unwindstack/Unwinder.h>
#include <unwindstack/AndroidUnwinder.h>

#include <base/runtime_common.h>

void* thread_entry(void* arg) {
    // cause a SIGSEGV here
    int* p = nullptr;
    *p = 0;
    return nullptr;
}

struct sigaction old_action;

void HandleUnexpectedSignalAndroid(int signal_number, siginfo_t* info, void* raw_context) {
    art::HandleUnexpectedSignalCommon(signal_number,
                                      info,
                                      raw_context,
            /* handle_timeout_signal= */ false,
            /* dump_on_stderr= */ false);

    // Run the old signal handler if it exists.
    if (old_action.sa_sigaction != nullptr) {
        old_action.sa_sigaction(signal_number, info, raw_context);
    }
}

int main() {

    art::InitPlatformSignalHandlersCommon(HandleUnexpectedSignalAndroid,
                                          &old_action,
            /* handle_timeout_signal= */ false);

    std::thread t(thread_entry, nullptr);

    t.join();

    return 0;
}
