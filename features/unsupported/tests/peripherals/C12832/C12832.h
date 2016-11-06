/* mbed library for the mbed Lab Board  128*32 pixel LCD
 * use C12832 controller
 * Copyright (c) 2012 Peter Drescher - DC2PD
 * Released under the MIT License: http://mbed.org/license/mit
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef C12832_H
#define C12832_H

#include "mbed.h"
#include "GraphicsDisplay.h"


/**
 * Optional Defines:
 * #define debug_lcd  1  enable infos to PC_USB
 */

// some defines for the DMA use
#define DMA_CHANNEL_ENABLE      1
#define DMA_TRANSFER_TYPE_M2P   (1UL << 11)
#define DMA_CHANNEL_TCIE        (1UL << 31)
#define DMA_CHANNEL_SRC_INC     (1UL << 26)
#define DMA_MASK_IE             (1UL << 14)
#define DMA_MASK_ITC            (1UL << 15)
#define DMA_SSP1_TX             (1UL << 2)
#define DMA_SSP0_TX             (0)
#define DMA_DEST_SSP1_TX        (2UL << 6)
#define DMA_DEST_SSP0_TX        (0UL << 6)

/**
 * Draw mode
 * NORMAL
 * XOR set pixel by xor the screen
 */
enum {NORMAL,XOR};

/**
 * Bitmap
 */
struct Bitmap{
    int xSize;
    int ySize;
    int Byte_in_Line;
    char* data;
};

/**
 * The C12832 class
 */
class C12832 : public GraphicsDisplay
{
public:
    /**
     * Create a C12832 object connected to SPI1
     */
    C12832(PinName mosi, PinName sck, PinName reset, PinName a0, PinName ncs, const char* name = "LCD");

    /**
     * Get the width of the screen in pixel
     *
     * @returns width of screen in pixel
     *
     */
    virtual int width();

    /**
     * Get the height of the screen in pixel
     *
     * @returns height of screen in pixel
     */
    virtual int height();

    /**
     * Draw a pixel at x,y black or white
     *
     * @param x horizontal position
     * @param y vertical position
     * @param color - 1 set pixel, 0 erase pixel
     */
    virtual void pixel(int x, int y,int colour);

     /**
      * Draw a circle
      *
      * @param x0,y0 center
      * @param r radius
      * @param color - 1 set pixel, 0 erase pixel
      */
    void circle(int x, int y, int r, int colour);

    /**
     * Draw a filled circle
     *
     * @param x0,y0 center
     * @param r radius
     * @param color - 1 set pixel, 0 erase pixel
     *
     * Use circle with different radius,
     * Can miss some pixels
     */
    void fillcircle(int x, int y, int r, int colour);

    /**
     * Draw a 1 pixel line
     *
     * @param x0,y0 start point
     * @param x1,y1 stop point
     * @param color - 1 set pixel, 0 erase pixel
     */
    void line(int x0, int y0, int x1, int y1, int colour);

    /**
     * Draw a rect
     *
     * @param x0,y0 top left corner
     * @param x1,y1 down right corner
     * @param color - 1 set pixel, 0 erase pixel
     */
    void rect(int x0, int y0, int x1, int y1, int colour);

    /**
     * Draw a filled rect
     *
     * @param x0,y0 top left corner
     * @param x1,y1 down right corner
     * @param color - 1 set pixel, 0 erase pixel
     */
    void fillrect(int x0, int y0, int x1, int y1, int colour);

    /**
     * Copy display buffer to LCD
     */
    void copy_to_lcd(void);

    /**
     * Set the orienation of the screen
     */

    void set_contrast(unsigned int o);

    /**
     * Read the contrast level
     */
    unsigned int get_contrast(void);

    /**
     * Invert the screen
     *
     * @param o = 0 normal, 1 invert
     */
    void invert(unsigned int o);

    /**
     * Clear the screen
     */
    virtual void cls(void);

    /**
     * Set the drawing mode
     *
     * @param mode NORMAl or XOR
     */
    void setmode(int mode);

    virtual int columns(void);

    /**
     * Calculate the max number of columns.
     * Depends on actual font size
     *
     * @returns max column
     */
    virtual int rows(void);

    /**
     * Put a char on the screen
     *
     * @param value char to print
     * @returns printed char
     */
    virtual int _putc(int value);

    /**
     * Draw a character on given position out of the active font to the LCD
     *
     * @param x x-position of char (top left)
     * @param y y-position
     * @param c char to print
     */
    virtual void character(int x, int y, int c);

    /**
     * Setup cursor position
     *
     * @param x x-position (top left)
     * @param y y-position
     */
    virtual void locate(int x, int y);
    
    /**
     * Setup auto update of screen 
     *
     * @param up 1 = on , 0 = off
     *
     * if switched off the program has to call copy_to_lcd() 
     * to update screen from framebuffer
     */
    void set_auto_up(unsigned int up);

    /**
     * Get status of the auto update function
     *
     * @returns if auto update is on
     */
    unsigned int get_auto_up(void);

    /** Vars     */
    SPI _spi;
    DigitalOut _reset;
    DigitalOut _A0;
    DigitalOut _CS;
    unsigned char* font;
    unsigned int draw_mode;


    /**
     * Select the font to use
     *
     * @param f pointer to font array
     *
     *   font array can created with GLCD Font Creator from http://www.mikroe.com
     *   you have to add 4 parameter at the beginning of the font array to use:
     *   - the number of byte / char
     *   - the vertial size in pixel
     *   - the horizontal size in pixel
     *   - the number of byte per vertical line
     *   you also have to change the array to char[]
     */
    void set_font(unsigned char* f);
    
    /**
     * Print bitmap to buffer
     *
     * @param bm Bitmap in flash
     * @param x  x start
     * @param y  y start
     */
    void print_bm(Bitmap bm, int x, int y);

protected:

    /**
     * Draw a horizontal line
     *
     * @param x0 horizontal start
     * @param x1 horizontal stop
     * @param y vertical position
     * @param color - 1 set pixel, 0 erase pixel
     */
    void hline(int x0, int x1, int y, int colour);

    /**
     * Draw a vertical line
     *
     * @param x horizontal position
     * @param y0 vertical start
     * @param y1 vertical stop
     * @param color - 1 set pixel, 0 erase pixel
     */
    void vline(int y0, int y1, int x, int colour);

    /**
     * Init the C12832 LCD controller
     */
    void lcd_reset();

    /**
     * Write data to the LCD controller
     *
     * @param dat data written to LCD controller
     */
    void wr_dat(unsigned char value);

    /**
     * Write a command the LCD controller
     *
     * @param cmd: command to be written
     */
    void wr_cmd(unsigned char value);

    void wr_cnt(unsigned char cmd);

    unsigned int orientation;
    unsigned int char_x;
    unsigned int char_y;
    unsigned char buffer[512];
    unsigned int contrast;
    unsigned int auto_up;

};




#endif
