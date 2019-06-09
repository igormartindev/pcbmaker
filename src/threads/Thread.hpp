#include <cstdint>
#include <contracts/ThreadContract.hpp>

template <uint16_t stackSize>
class Thread
{
public:
    explicit Thread(const char* name, char priority, int flags) noexcept;
    kernel_pid_t create();

protected:
    /**
     * Main function of the thread
     */
    virtual void run() = 0;

private:
    char stack[stackSize];
    const char* name;
    char priority;
    int flags;
    kernel_pid_t pid;

    static void* call(void* args)
    {
        (static_cast<Thread*>(args))->run();
        return nullptr;
    }
};

/**
 * Constructor
 * @tparam stackSize Stack size for the thread (ex: THREAD_STACKSIZE_MAIN)
 * @param name       Name of thread
 * @param priority   Priority of the thread (ex: THREAD_PRIORITY_MAIN)
 * @param flags      Thread modifiers (ex: THREAD_CREATE_WOUT_YIELD)
 */
template <uint16_t stackSize>
Thread<stackSize>::Thread(const char* name, char priority, int flags)
{
    this->name = name;
    this->priority = priority;
    this->flags = flags;
}

/**
 * Create current thread and add to scheduler
 * @return kernel_pid_t Process id of the thread
 */
template <uint16_t stackSize>
kernel_pid_t Thread<stackSize>::create()
{
    this->pid = thread_create(
            this->stack,
            sizeof(this->stack),
            this->priority,
            this->flags,
            Thread::call,
            this,
            this->name
    );

    return this->pid;
}
