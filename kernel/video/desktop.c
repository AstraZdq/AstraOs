#include "desktop.h"
#include "framebuffer.h"
#include "mouse.h"
#include "io.h"

static int win_x = 180;
static int win_y = 150;
static int win_w = 420;
static int win_h = 280;
static int win_open = 1;
static int is_dragging = 0;
static int drag_offset_x = 0;
static int drag_offset_y = 0;
static int prev_ml = 0;

static void draw_beautiful_cursor(int mx, int my)
{
    // A classic white arrow cursor with a black outline
    static const int cursor_data[16][16] = {
        {2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {2,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {2,1,1,2,0,0,0,0,0,0,0,0,0,0,0,0},
        {2,1,1,1,2,0,0,0,0,0,0,0,0,0,0,0},
        {2,1,1,1,1,2,0,0,0,0,0,0,0,0,0,0},
        {2,1,1,1,1,1,2,0,0,0,0,0,0,0,0,0},
        {2,1,1,1,1,1,1,2,0,0,0,0,0,0,0,0},
        {2,1,1,1,1,1,1,1,2,0,0,0,0,0,0,0},
        {2,1,1,1,1,1,1,1,1,2,0,0,0,0,0,0},
        {2,1,1,1,1,1,2,2,2,2,2,0,0,0,0,0},
        {2,1,1,2,1,1,2,0,0,0,0,0,0,0,0,0},
        {2,1,2,0,2,1,1,2,0,0,0,0,0,0,0,0},
        {2,2,0,0,2,1,1,2,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,2,1,1,2,0,0,0,0,0,0,0},
        {0,0,0,0,0,2,1,1,2,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0}
    };
    
    for (int y = 0; y < 16; y++)
    {
        for (int x = 0; x < 16; x++)
        {
            int val = cursor_data[y][x];
            if (val == 1)
            {
                framebuffer_pixel(mx + x, my + y, 0xFFFFFF);
            }
            else if (val == 2)
            {
                framebuffer_pixel(mx + x, my + y, 0x000000);
            }
        }
    }
}

void desktop_render()
{
    uint32_t width = framebuffer_get_width();
    uint32_t height = framebuffer_get_height();

    if (width == 0 || height == 0)
    {
        return; // Fallback for text mode if called
    }

    // 1. Draw a beautiful vertical background gradient (Indigo -> Purple)
    framebuffer_draw_gradient();

    // Get mouse status
    int mx = mouse_x();
    int my = mouse_y();
    int ml = mouse_button_left();

    // 2. Window Dragging & Closing Logic
    if (win_open)
    {
        if (ml)
        {
            if (!is_dragging)
            {
                // Clicked on title bar
                if (mx >= win_x && mx < win_x + win_w &&
                    my >= win_y && my < win_y + 28)
                {
                    // Check if clicked close dot (Red)
                    if (mx >= win_x + 10 && mx <= win_x + 22 &&
                        my >= win_y + 8 && my <= win_y + 20)
                    {
                        win_open = 0;
                    }
                    else
                    {
                        is_dragging = 1;
                        drag_offset_x = mx - win_x;
                        drag_offset_y = my - win_y;
                    }
                }
            }
            else
            {
                win_x = mx - drag_offset_x;
                win_y = my - drag_offset_y;
                
                // Bounds checks to keep window partially on screen
                if (win_x < -win_w + 50) win_x = -win_w + 50;
                if (win_x > (int)width - 50) win_x = (int)width - 50;
                if (win_y < 25) win_y = 25;
                if (win_y > (int)height - 80) win_y = (int)height - 80;
            }
        }
        else
        {
            is_dragging = 0;
        }
    }

    // 3. Top Bar (translucent dark bar)
    framebuffer_rect(0, 0, width, 25, 0x161622);
    framebuffer_rect(0, 24, width, 1, 0x323244);
    framebuffer_text(15, 6, "AstraOS   File   Edit   View   Help", 0xE0E0FF, 0x161622);
    framebuffer_text(width - 240, 6, "2026-05-23 14:31  [SYS: OK]", 0xB0B0D0, 0x161622);

    // 4. Desktop Icons
    // Text background should match the gradient color around it for seamless look
    uint32_t text_bg_y = 100;
    uint32_t r_bg = 0x1A + (0x24 * text_bg_y / height);
    uint32_t g_bg = 0x1A + (0x12 * text_bg_y / height);
    uint32_t b_bg = 0x35 + (0x30 * text_bg_y / height);
    uint32_t icon_bg = (r_bg << 16) | (g_bg << 8) | b_bg;

    // Icon 1: Computer
    framebuffer_rect(40, 60, 36, 26, 0x3E78B3); // screen
    framebuffer_rect(48, 86, 20, 4, 0x2C5680);  // stand
    framebuffer_rect(44, 90, 28, 2, 0x2C5680);  // base
    framebuffer_text(30, 96, "Computer", 0xEAEAEA, icon_bg);

    // Icon 2: Trash
    uint32_t trash_bg_y = 175;
    r_bg = 0x1A + (0x24 * trash_bg_y / height);
    g_bg = 0x1A + (0x12 * trash_bg_y / height);
    b_bg = 0x35 + (0x30 * trash_bg_y / height);
    uint32_t trash_icon_bg = (r_bg << 16) | (g_bg << 8) | b_bg;

    framebuffer_rect(44, 136, 28, 30, 0x6E6E7E);
    framebuffer_rect(40, 132, 36, 4, 0x5A5A6A);
    framebuffer_rect(48, 140, 4, 22, 0x5A5A6A);
    framebuffer_rect(58, 140, 4, 22, 0x5A5A6A);
    framebuffer_text(42, 172, "Trash", 0xEAEAEA, trash_icon_bg);

    // 5. Draw Window if open
    if (win_open)
    {
        // Shadow/glow border (subtle premium styling)
        framebuffer_rect(win_x - 2, win_y - 2, win_w + 4, win_h + 4, 0x0F0F16);
        
        // Window Body
        framebuffer_rect(win_x, win_y, win_w, win_h, 0x13131A);
        
        // Title Bar
        framebuffer_rect(win_x, win_y, win_w, 28, 0x22222E);
        framebuffer_rect(win_x, win_y + 27, win_w, 1, 0x323246);
        
        // Window Title
        framebuffer_text(win_x + 65, win_y + 8, "AstraOS System Console", 0xE0E0FF, 0x22222E);

        // macOS Window Buttons (Red, Yellow, Green dots)
        int is_close_hover = (mx >= win_x + 10 && mx <= win_x + 22 && my >= win_y + 8 && my <= win_y + 20);
        int is_min_hover = (mx >= win_x + 28 && mx <= win_x + 40 && my >= win_y + 8 && my <= win_y + 20);
        int is_max_hover = (mx >= win_x + 46 && mx <= win_x + 58 && my >= win_y + 8 && my <= win_y + 20);

        framebuffer_rect(win_x + 12, win_y + 9, 10, 10, is_close_hover ? 0xFF7A70 : 0xFF5F56);
        framebuffer_rect(win_x + 28, win_y + 9, 10, 10, is_min_hover ? 0xFFD460 : 0xFFBD2E);
        framebuffer_rect(win_x + 44, win_y + 9, 10, 10, is_max_hover ? 0x56E070 : 0x27C93F);

        // Terminal Content Mockup
        framebuffer_text(win_x + 15, win_y + 40, "AstraOS Kernel version 0.1.0", 0x8AE234, 0x13131A);
        framebuffer_text(win_x + 15, win_y + 56, "Memory: 512 MB | Paging Active", 0x8AE234, 0x13131A);
        
        // Cute Cat Neofetch
        framebuffer_text(win_x + 20, win_y + 85, "   /\\_/\\     System Details", 0xFFFFFF, 0x13131A);
        framebuffer_text(win_x + 20, win_y + 101, "  ( o.o )    -------------", 0xFFFFFF, 0x13131A);
        framebuffer_text(win_x + 20, win_y + 117, "   > ^ <     OS: AstraOS 32-bit", 0xFFFFFF, 0x13131A);
        framebuffer_text(win_x + 20, win_y + 133, "             Shell: AstraShell", 0xFFFFFF, 0x13131A);
        framebuffer_text(win_x + 20, win_y + 149, "             Kernel: x86-32 v0.1", 0xFFFFFF, 0x13131A);

        framebuffer_text(win_x + 15, win_y + 185, "astra@AstraOS:~$ _", 0xAD7FA8, 0x13131A);
    }

    // 6. Dock (Floating bottom bar)
    int dock_w = 260;
    int dock_h = 45;
    int dock_x = width / 2 - dock_w / 2;
    int dock_y = height - 60;

    framebuffer_rect(dock_x - 1, dock_y - 1, dock_w + 2, dock_h + 2, 0x1F1F2B);
    framebuffer_rect(dock_x, dock_y, dock_w, dock_h, 0x2A2A3D);
    framebuffer_rect(dock_x + 1, dock_y + 1, dock_w - 2, dock_h - 2, 0x3A3A52);

    // Dock Icon 1: System Manager
    int is_icon1_hover = (mx >= dock_x + 20 && mx < dock_x + 52 && my >= dock_y + 7 && my < dock_y + 39);
    framebuffer_rect(dock_x + 20, dock_y + 7, 32, 32, is_icon1_hover ? 0x2D88FF : 0x0055D4);
    framebuffer_text(dock_x + 32, dock_y + 15, "A", 0xFFFFFF, is_icon1_hover ? 0x2D88FF : 0x0055D4);

    // Dock Icon 2: Terminal Console
    int is_icon2_hover = (mx >= dock_x + 75 && mx < dock_x + 107 && my >= dock_y + 7 && my < dock_y + 39);
    framebuffer_rect(dock_x + 75, dock_y + 7, 32, 32, is_icon2_hover ? 0x48485E : 0x23232E);
    framebuffer_text(dock_x + 88, dock_y + 15, ">", 0x8AE234, is_icon2_hover ? 0x48485E : 0x23232E);
    if (win_open)
    {
        framebuffer_rect(dock_x + 89, dock_y + 40, 4, 2, 0xFFFFFF);
    }

    // Dock Icon 3: Settings
    int is_icon3_hover = (mx >= dock_x + 130 && mx < dock_x + 162 && my >= dock_y + 7 && my < dock_y + 39);
    framebuffer_rect(dock_x + 130, dock_y + 7, 32, 32, is_icon3_hover ? 0x8A96A6 : 0x5C6975);
    framebuffer_text(dock_x + 143, dock_y + 15, "*", 0xFFFFFF, is_icon3_hover ? 0x8A96A6 : 0x5C6975);

    // Dock Icon 4: Reboot
    int is_icon4_hover = (mx >= dock_x + 185 && mx < dock_x + 217 && my >= dock_y + 7 && my < dock_y + 39);
    framebuffer_rect(dock_x + 185, dock_y + 7, 32, 32, is_icon4_hover ? 0xFF4D4D : 0xCC0000);
    framebuffer_text(dock_x + 198, dock_y + 15, "X", 0xFFFFFF, is_icon4_hover ? 0xFF4D4D : 0xCC0000);

    // 7. Click events processing
    if (ml && !prev_ml)
    {
        if (is_icon2_hover)
        {
            win_open = !win_open;
        }
        
        if (is_icon4_hover)
        {
            outb(0x64, 0xFE); // Pulse reset line (standard PC reboot)
        }
    }
    prev_ml = ml;

    // 8. Draw Beautiful Mouse Cursor on top of everything
    draw_beautiful_cursor(mx, my);
}