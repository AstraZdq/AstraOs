void _start()
{
    volatile char* vga =
        (volatile char*)0xB8000;

    const char* msg =
        "Hello from user ELF!";

    int i = 0;

    while (msg[i])
    {
        vga[i * 2] = msg[i];
        vga[i * 2 + 1] = 0x0A;
        i++;
    }

    while (1)
    {
    }
}