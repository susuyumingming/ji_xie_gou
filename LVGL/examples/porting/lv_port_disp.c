/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
 
#include "lv_port_disp.h"
#include <stdbool.h>
#include "lcd.h"
#include "spi.h"
// #include "stm32h7xx_ll_spi.h"
#include "stm32h7xx.h"
// ********** 步骤2：添加函数声明前置（消除隐式声明警告，LVGL8.3专属）**********
// 声明这两个GPU绑定函数，放在调用之前即可
//void lv_disp_drv_set_gpu_fill_cb(lv_disp_drv_t * drv, void (*fill_cb)(lv_color_t * dest_buf, lv_coord_t dest_w, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_color_t color));
//void lv_disp_drv_set_gpu_copy_cb(lv_disp_drv_t * drv, void (*copy_cb)(lv_color_t * dest_buf, lv_coord_t dest_w, lv_color_t * src_buf, lv_coord_t src_w, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h));


/*********************
 *      DEFINES
 *********************/
#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    320
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height, default value 480 is used for now.
    #define MY_DISP_VER_RES    480
#endif

#define MY_DISP    60

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /**
     * LVGL requires a buffer where it internally draws the widgets.
     * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
     * The buffer has to be greater than 1 display row
     *
     * There are 3 buffering configurations:
     * 1. Create ONE buffer:
     *      LVGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer:
     *      LVGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LVGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Double buffering
     *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
     *      This way LVGL will always provide the whole rendered screen in `flush_cb`
     *      and you only need to change the frame buffer's address.
     */

    /* Example for 1) */
    // static lv_disp_draw_buf_t draw_buf_dsc_1;
    // static lv_color_t buf_1[MY_DISP_HOR_RES * MY_DISP];                          /*A buffer for 10 rows*/
    // lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * MY_DISP);   /*Initialize the display buffer*/
			

			
    /* Example for 2) */
   static lv_disp_draw_buf_t draw_buf_dsc_2;
   static lv_color_t buf_2_1[MY_DISP_HOR_RES * MY_DISP];                        /*A buffer for 10 rows*/
   static lv_color_t buf_2_2[MY_DISP_HOR_RES * MY_DISP];                        /*An other buffer for 10 rows*/
   lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * MY_DISP);   /*Initialize the display buffer*/

    /* Example for 3) also set disp_drv.full_refresh = 1 below*/
//    static lv_disp_draw_buf_t draw_buf_dsc_3;
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
//    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
//    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
//                          MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;
//		disp_drv.sw_rotate = 1;   // add for rotation
//		disp_drv.rotated = LV_DISP_ROT_90;
		
    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_2;
	disp_drv.full_refresh = 0;
	// ==============================================
    // 新增仅2行：LVGL8.3专属DMA2D硬件加速接口注册（核心！）
    // 替代原有的直接赋值，适配8.3版本规范
    // ==============================================
    // 4. DMA2D接口注册（与之前一致，无需修改，指向lcd_spi.c中的16位适配函数）
    // LVGL会自动调用这两个函数完成硬件加速，无需手动调用！
//    lv_disp_drv_set_gpu_fill_cb(&disp_drv, lv_gpu_dma2d_fill);
//    lv_disp_drv_set_gpu_copy_cb(&disp_drv, lv_gpu_dma2d_copy);
//		
    /*Required for Example 3)*/
    //disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
    /*You code here*/
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}
//static u8 conv_buf[MY_DISP_HOR_RES * 1*3] __attribute__((aligned(4))); // 对齐4字节，适配DMA
static void rgb565_to_rgb666_batch(lv_color_t *src, u8 *dst, u32 pixel_num)
{
    u32 i;
    u16 color;
    for(i = 0; i < pixel_num; i++)
    {
        color = src[i].full; // 取出LVGL16位颜色值（与直接取Data等价）
        // 按你原有规则转换，存入连续3字节（无CPU等待，纯内存操作）
        dst[i*3]   = (color >> 8) & 0xF8; // RED 第1字节
        dst[i*3+1] = (color >> 3) & 0xFC; // GREEN 第2字节
        dst[i*3+2] = color << 3;          // BLUE 第3字节
    }
}
/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static uint8_t dummy_rx_buffer[MY_DISP_HOR_RES * MY_DISP* 3];  // 虚拟接收缓冲区
static uint8_t line_buffer[MY_DISP_HOR_RES * MY_DISP* 3];  // 足够存储一行
static volatile bool dma_tx_complete = false;
static volatile bool dma_rx_complete = false;
static lv_disp_drv_t* current_disp_drv = NULL;
int num=0;
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(disp_flush_enabled) 
    {
        uint32_t width = area->x2 - area->x1 + 1;
        uint32_t height = area->y2 - area->y1 + 1;
        uint32_t total_pixels = width * height;
        uint32_t total_bytes = total_pixels * 3;

		LCD_SetWindows_lvgl(area->x1, area->y1, area->x2, area->y2); 

		LCD_CS_CLR;   // 拉低片选，准备写数据
		LCD_RS_SET;   // 设为数据模式（RS=1）
			
//         // 设置全局变量
        dma_tx_complete = false;
        dma_rx_complete = false;
        current_disp_drv = disp_drv;
		num++;
        for(uint32_t i = 0; i < total_pixels; i++) 
        {
            uint16_t color = color_p[i].full;
            line_buffer[i*3]   = (color >> 8) & 0xF8;
            line_buffer[i*3+1] = (color >> 3) & 0xFC;
            line_buffer[i*3+2] = color << 3;
		}
        if(HAL_SPI_TransmitReceive_DMA(&hspi1, line_buffer, dummy_rx_buffer, total_bytes) == HAL_OK)
        {
        }
        // LCD_CS_SET; // 必须释放片选！
    }
    // lv_disp_flush_ready(disp_drv);
}
//发送中断处理函数
void SPI1_TX_DMA_IRQHandler(void)
{
	// 1. 定义临界区保护的临时变量（中断中必须用）
    // 检查：DMA发送完成中断标志（TCIF0）
    // if(LL_DMA_IsActiveFlag_TC0(DMA2))
    // {
    //     LL_DMA_ClearFlag_TC0(DMA2);        // 清除标志
    //     LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_0);
    //     LL_SPI_DisableDMAReq_TX(SPI1);
        dma_tx_complete = true;
    //     // printf("dma_tx_complete\r\n");
        if(dma_tx_complete&&dma_rx_complete)
        {
            // 3. 释放SPI片选（关键：传输完成后再释放，避免数据中断）
		    LCD_CS_SET;
            // 通知LVGL
            if(current_disp_drv) {
                // printf("current_disp_drv\r\n");
                lv_disp_flush_ready(current_disp_drv);
                current_disp_drv = NULL;
            } 
        }
    // }
}
//接收中断处理函数
void SPI1_RX_DMA_IRQHandler(void)
{
    // if(LL_DMA_IsActiveFlag_TC2(DMA2))
    // {
    //     LL_DMA_ClearFlag_TC2(DMA2);        // 清除标志
    //     LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);
    //     LL_SPI_DisableDMAReq_RX(SPI1);
        dma_rx_complete = true;
    //     // printf("dma_rx_complete\r\n");
        if(dma_tx_complete&&dma_rx_complete)
        {
            // 3. 释放SPI片选（关键：传输完成后再释放，避免数据中断）
			LCD_CS_SET;
            // 通知LVGL
            if(current_disp_drv) {
                // printf("current_disp_drv\r\n");
                lv_disp_flush_ready(current_disp_drv);
                current_disp_drv = NULL;
            } 
        }
    // }
}
/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
