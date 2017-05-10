//To be done, 
// 1. think of inline
// 2.
// 3. TriggerType 

#ifndef MBED_DMA_H
#define MBED_DMA_H


#include "dma_api.h"
#include "platform.h"
#include "FunctionPointer.h"

/** A generic DMA for transfer data without hanging the CPU.
 * It can be used for m2m, m2p, p2m, m2m transfer
 * It will choose the DMA with the priority you have set.
 * Lower number means higher prority.
 * If no prority is set, it will choose which ever free channel.
 * Example:
 * @code
 * // Send the memory data "Hello world" in source address to destination address via DMA
 * // attach a function to swtich LED on when the transfer finish
 *  char src[] = "Hello world\r\n";
 *  uint8_t size = sizeof (src);
 *  char *dst  = (char *) malloc(size);
 *  memset (dst, '\0', size);
 *  LPC_SC->PCONP |= (1 << 29);    // Enable LPC1768 GPDMA clock
 *  DMA dma1 (0) ;
 *  dma1.source (src,0,1);
 *  dma1.destination (dst,0,1);
 *  dma1.attach_TC(led_switchon) ;
 *  dma1.start(size);
 *  dma1.wait();
 *  printf("dst text: %s", dst);
 * @endcode
*/

namespace mbed{
class DMA
{
public:
    /** @brief  Create the DMA Channel according to the prority or choose whichever free if no prority given.
    *                     Initialize the number of channels in the device
    *  @param   priority: The DMA prority.
    *    @note    If prority has been given but that channel is not avaiable, it will wait until the channel avaiable.
    */
    DMA(int priority = -1);

    ~DMA ();
    /** @brief  Get source starting address, transfer width and setting auto increment.
     *  @param  src. The source starting address.
			* @param  inc. Set memory automatice increment.
     *  @param  width. The transfer data width
     *  @note   If width is not given, it will set the width automatically according to the data type in the memory
     */
    template<typename T>    // To be reviewd, should put the template into the cpp rather than the header?
    void source(const T* src, bool inc, unsigned int width = sizeof(T)*8) {
        //inc = isMemory ((uint32_t)src); // To be reviewed. How to put this line into the arguments?
        uint32_t src_prt= (uint32_t) src;
        dma_source(dma_init_struct, src_prt, width, inc);
    }

    /** @brief  Get destination starting address, transfer width and setting auto increment.
     *  @param  dst. The destination starting address.
		 *  @param  inc. Set memory automatice increment.
     *  @param  width. The transfer data width
     *  @note   If width is not given, it will set the width automatically according to the data type in the memory
     *          
     */
    template<typename T>
    void destination(const T* dst, bool inc, unsigned int width =sizeof(T)*8) {
        //inc = isMemory ((uint32_t) dst);
        uint32_t dst_ptr=(uint32_t) dst;
        dma_destination(dma_init_struct, dst_ptr, width, inc);
    }

    /** @brief  Get source trigger type
     *  @param  trig. The source triggert type.
     *  @note   If the source is memory. The trigger type would be ALWAYS even you set it differently.
     */
    void trigger_source(TriggerType trig = ALWAYS);

    /** @brief  Get destination trigger type
     *  @param  trig. The destination triggert type.
     *  @note   If the destination is memory. The trigger type would be ALWAYS even you set it differently.
     */
    void trigger_destination(TriggerType trig = ALWAYS);
		

    /** @brief  Start the DMA to transfer data
     *  @param  lengh. Define how many data the DMA needs to transfer
     */
    void start(unsigned int len);

    /** @brief  Wait for DMA transaction finishs
		 * @retval  None
     */
    void wait();
				
    /** @brief  Attach a function to DMA IRQ handler. The attached function will be called when the transfer has completed successfully.
     *  @param  *fptr. The function pointer.
     */
    void attach_finish(func_ptr fptr) {
        dma_IRQ_attach(chan, FINISH, fptr);
        NVIC_SetVector(DMA_IRQn, (uint32_t)&dma_IRQ_handler);
        NVIC_EnableIRQ(DMA_IRQn);
    }
		
		
    /** @brief  Attach a function to DMA IRQ handler. The attached function will be called when the transfer has completed successfully.
     *  @param  *fptr. The function pointer.
     *  @note   There are two attach function attach_TC and attach_Err in case you want to attach different functions when tranfer finishs or fails
     */
    void attach_err(func_ptr fptr) {
        dma_IRQ_attach(chan, ERR, fptr);
        NVIC_SetVector(_DMA_IRQ, (uint32_t)&dma_IRQ_handler);
        NVIC_EnableIRQ(_DMA_IRQ);
    }

    DMA_InitTypeDef* dma_init_struct;
		
    int channel_num;
		int chan; // The chosen channel number

private:
    
    int _choose_free_channel(int channel);
};

}
#endif
