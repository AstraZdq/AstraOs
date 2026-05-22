void syscall_print(const char* str)
{
    __asm__ volatile (
        "int $0x80"
        :
        : "a"(1), "b"(str)
    );
}

void syscall_exit()
{
    __asm__ volatile (
        "int $0x80"
        :
        : "a"(2)
    );
}

int main()
{
    syscall_print(
        "Hello from user mode!\n"
    );

    syscall_exit();

    while (1)
    {
    }

    return 0;
}