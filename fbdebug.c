#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{
	// Open the framebuffer device
    int fb = open("/dev/fb0", O_RDWR);
    if (fb == -1) {
        perror("Unable to open framebuffer device");
        return 1;
    }

    // Get screen information
    struct fb_var_screeninfo vinfo;
    if (ioctl(fb, FBIOGET_VSCREENINFO, &vinfo)) {
        perror("Unable to get screen info");
        close(fb);
        return 1;
    }

    printf("Resolution: %dx%d, %d bpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Calculate the screen size in bytes
    size_t screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the framebuffer to memory
    unsigned char *fbmem = (unsigned char *)malloc(screensize);
    if (!fbmem) {
        perror("Unable to allocate memory");
        close(fb);
        return 1;
    }

    // Fill the screen with a solid color (e.g., red for RGB565)
    unsigned short color = 0xF800; // Red in RGB565
    for (size_t i = 0; i < screensize / 2; i++) {
        ((unsigned short *)fbmem)[i] = color;
    }

    // Write the buffer to the framebuffer
    write(fb, fbmem, screensize);

    // Clean up
    free(fbmem);
    close(fb);

	return 0;
}
