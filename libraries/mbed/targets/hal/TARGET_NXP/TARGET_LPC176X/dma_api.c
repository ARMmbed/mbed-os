#include "dma_api.h"
#include <assert.h>


int _channel_num = 8;

// Define LPC1768 DMA IRQ. Different platforms might have different name
IRQn_Type _DMA_IRQ = DMA_IRQn;

/*  DMA channel control register bit set */
#define DMA_CCxControl_TransferSize_Pos 0
#define DMA_CCxControl_SBSize_Pos 12
#define DMA_CCxControl_DBSize_Pos 15
#define DMA_CCxControl_SWidth_Pos 18
#define DMA_CCxControl_DWidth_Pos 21
#define DMA_CCxControl_SI_Pos 26
#define DMA_CCxControl_DI_Pos 27
#define DMA_CCxControl_I_Pos 31

/*  DMA Channel config register bit set */
#define DMA_CCxConfig_E_Pos 0
#define DMA_CCxConfig_SrcPeripheral_Pos 1
#define DMA_CCxConfig_DestPeripheral_Pos 6
#define DMA_CCxConfig_TransferType_Pos 11
#define DMA_CCxConfig_IE_Pos 14
#define DMA_CCxConfig_ITC_Pos 15
#define DMA_CCxConfig_L_Pos 16
#define DMA_CCxConfig_A_Pos 17
#define DMA_CCxConfig_H_Pos 18

//*  DMA interrupt callback */
func_ptr dma_irq_finish[8];
func_ptr dma_irq_error[8];



// Optimisation 1. Use burst mode
#define BURST_ENABLED 1
// Optimisation 2. Pack bytes/half  to word
#define  WORD_TRANSFER 1



// Map the trigger index
int trigger_value [33] = {
   -1, // ALWAYS ,
    0, // _SSP0_TX,
    1, // _SSP0_RX,
    2, // _SSP1_TX,
    3, // _SSP1_RX,
    4, // _ADC,
    5, // _I2S0,
    6, // _I2S1,
    7, // _DAC,
    8, // _UART0_TX,
    9, // _UART0_RX,
    10, // _UART1_TX,
    11, // _UART1_RX,
    12, // _UART2_TX,
    13, // _UART2_RX,
    14, // _UART3_TX,
    15, // _UART3_RX,
    24, // _MATCH0_0,
    25, // _MATCH0_1,
    26, // _MATCH1_0,
    27, // _MATCH1_1,
    28, // _MATCH2_0,
    29, // _MATCH2_1,
    30, // _MATCH3_0,
    31  // _MATCH3_1,
};



typedef struct DMA_InitTypeDef{
    uint32_t DMA_DstAddr; // Specifies the destination base address for DMAy Channelx. 
    uint32_t DMA_SrcAddr; // Specifies the source base address for DMAy Channelx. 
    uint32_t DMA_TransferSize;// Specifies the source  transfer size  
    uint32_t DMA_SrcBurst; // Specifies the source  burst size    
    uint32_t DMA_DestBurst; // Specifies the destination burst size   
    uint32_t DMA_SrcWidth; // Specifies the source transfer width   
    uint32_t DMA_DestWidth; // Specifies the destination transfer width   
    uint32_t DMA_LLI; // Specifies the next link address   
    bool DMA_SrcInc;  // Specifies whether the source is incremented or not 
    bool DMA_DestInc; // Specifies whether the destination is incremented or not 
    bool DMA_TermInt; // Specifies whether the terminal count interrupt enabled or not 
    int DMA_TriggerSource; // Specifies the source trigger 
    int DMA_TriggerDestination; // Specifies the destination trigger 
    TransferType  DMA_TransferType;
}DMA_InitTypeDef ;



/**
  * @brief  Check whether the address is with memory ranage.
  * @param  src: the physical address
  * @retval 0 or 1
  */
__inline static bool is_memory(uint32_t addr);

/**
 * @brief  Static function. Automatice cacluate the transfer type according to the source and destination address
 * @param  src_addr. Source starting address.
 * @param  dst_addr. Destination starting address.
 * @retval Transfer type. It can be M2M,M2P,P2M or P2P
 */
static TransferType get_transfer_type(uint32_t src_addr, uint32_t dst_addr );

/**
  * @brief  Static function. Get the channel address according to the channel number
  * @param  channel. The chosen channel number
  * @retval Chosen channel base address
  */
	
__inline static LPC_GPDMACH_TypeDef* get_channel(int channel)
{
    return (LPC_GPDMACH_TypeDef*)(LPC_GPDMACH0_BASE + 0x20*channel);
}


/**
  * @brief    Enable DMA Burst mode for optimisation
  */


void dma_destination(DMA_InitTypeDef* DMA_InitStruct, const uint32_t dst, unsigned int width, bool inc)
{
    DMA_InitStruct->DMA_DstAddr = dst;
    DMA_InitStruct->DMA_SrcBurst = 0x00; // 1 byte To be done
    DMA_InitStruct->DMA_DestWidth = (width >>4);
    DMA_InitStruct->DMA_DestInc = inc;
}


void dma_source(DMA_InitTypeDef* DMA_InitStruct, const uint32_t src, unsigned int width, bool inc)
{
    DMA_InitStruct->DMA_SrcAddr = src;
    DMA_InitStruct->DMA_SrcBurst = 0x00; // 1 byte
    DMA_InitStruct->DMA_SrcInc = inc;
    DMA_InitStruct->DMA_SrcWidth = (width >> 4);
}


void dma_trigger_source(DMA_InitTypeDef* DMA_InitStruct, TriggerType trig)
{
    DMA_InitStruct->DMA_TriggerSource = trigger_value[trig];
}


void dma_trigger_destination(DMA_InitTypeDef* DMA_InitStruct, TriggerType trig)
{
    DMA_InitStruct->DMA_TriggerDestination = trigger_value[trig];
}



void dma_start(int channel, DMA_InitTypeDef* DMA_InitStruct, unsigned int length)
{
	   LPC_GPDMACH_TypeDef* volatile GPDMA_channel;
    // Get DMA channel address
    GPDMA_channel = get_channel(channel);

		// Set the transfer size
	  // Put it here rather than in DMA_Init. So that one could send new transfer data without reinit other registers.
	  DMA_InitStruct->DMA_TransferSize = (uint32_t)length; 
	
    
	  #if WORD_TRANSFER 
    int dst = (int)DMA_InitStruct->DMA_DstAddr;
    int src = (int)DMA_InitStruct->DMA_SrcAddr;  
	  int old_SrcWidth = DMA_InitStruct->DMA_SrcWidth;
		int old_DstWidth = DMA_InitStruct->DMA_DestWidth;
	  int new_length = 0;
	  int new_width = 2; // new width will be word
    if ((dst % sizeof(long) == 0) && (src % sizeof(long) == 0) && // Only use word aligned when starting addresss is at 4 bytes boundary
			  (length >=4) && // Only use word aligned when length is no less than 4
		    (DMA_InitStruct->DMA_SrcInc==1) &&   // Only use word aligned when the source is incretmental 
				 (DMA_InitStruct->DMA_DestInc == 1)) // Only use word aligned when the address is incretmental 
																																							
		{
		    new_length = (length << old_SrcWidth) >> 2; // Get the size of the word-aligned part
			 																					
			
	    	if (length % sizeof(long) == 0){ // If the original transfer size is word aligned
						DMA_InitStruct->DMA_TransferSize = new_length;
					  // Set destinatin width is word only when the destination is automatic incremental
					  DMA_InitStruct->DMA_DestWidth = ( DMA_InitStruct->DMA_DestInc == 0x1) ? new_width : old_DstWidth;
						DMA_InitStruct->DMA_SrcWidth = new_width;
				
					  GPDMA_channel->DMACCControl |= (DMA_InitStruct->DMA_SrcWidth<<DMA_CCxControl_SWidth_Pos) |
																					 (DMA_InitStruct->DMA_DestWidth<<DMA_CCxControl_DWidth_Pos);
			  }
				else
				{
					  /*Need to transfer the word aligned part firstly then non-word aligned. Starting address has to be at transfer width boundary*/
					 
					  DMA_InitStruct->DMA_TransferSize = new_length;
					  DMA_InitStruct->DMA_SrcWidth = new_width;
					  // set destinatin width is word only when the destination is automatic incremental
		        DMA_InitStruct->DMA_DestWidth = (DMA_InitStruct->DMA_DestInc == 0x1) ? new_width : old_DstWidth;	      
					  			
					  GPDMA_channel->DMACCControl |= (DMA_InitStruct->DMA_SrcWidth << DMA_CCxControl_SWidth_Pos) |
																					 (DMA_InitStruct->DMA_DestWidth << DMA_CCxControl_DWidth_Pos);
				  	GPDMA_channel->DMACCControl |= DMA_InitStruct->DMA_TransferSize << DMA_CCxControl_TransferSize_Pos;
					
					
	    		  // mask ITC and IE, so that interrupts won't be generated when transfer the first part
	 				  GPDMA_channel->DMACCConfig &= ~(1ul <<DMA_CCxConfig_ITC_Pos);
					  // mask IE
					  GPDMA_channel->DMACCConfig &= ~(1ul<< DMA_CCxConfig_IE_Pos);	

					  // Enable the selected DMAy Channelx
					  LPC_GPDMA->DMACConfig = 0x01;
					  GPDMA_channel->DMACCConfig |= 1ul<<DMA_CCxConfig_E_Pos;
		
					  while (dma_channel_active(channel)); // wait for the first part transfer finish
		
				   	// DMACCSrcAddr currently hold the end address of first part.Add the data width 
						// to point to the starting source address of the second part. 
				  	GPDMA_channel->DMACCSrcAddr = GPDMA_channel->DMACCSrcAddr +  (1 << DMA_InitStruct->DMA_SrcWidth);
						// DMACCDestAddr currently hold the end address of first part.Add the data width 
						// to point to the starting destination address of the second part. 
				  	if (DMA_InitStruct->DMA_DestInc == 0x1)
	 	  	    GPDMA_channel->DMACCDestAddr = GPDMA_channel->DMACCDestAddr +  (1 << DMA_InitStruct->DMA_DestWidth);
				 
				        
					  /* start to transfer the non-word aligned part */
						
					  DMA_InitStruct->DMA_SrcWidth = old_SrcWidth;
					  // set destinatin width is word only when the destination is automatic incremental
		        DMA_InitStruct->DMA_DestWidth = old_DstWidth;	      
					  // Note: no need to set the new destination or source address as it has been updated automatically			
						
						//Clear to the width to 0x000 firstly 
					  GPDMA_channel->DMACCControl &= ~(0x111 << DMA_CCxControl_SWidth_Pos) &
																					~(0x111 << DMA_CCxControl_DWidth_Pos);
																				
						GPDMA_channel->DMACCControl |= (DMA_InitStruct->DMA_SrcWidth << DMA_CCxControl_SWidth_Pos) |
																					 (DMA_InitStruct->DMA_DestWidth << DMA_CCxControl_DWidth_Pos);
				    DMA_InitStruct->DMA_TransferSize = (uint32_t)(length - new_length*4); // transfer the remaining part
						
				}
		}
		#endif    
		
		// Enable Interrupt
	  // Unmask ITC
    GPDMA_channel->DMACCConfig |= 1ul <<DMA_CCxConfig_ITC_Pos;
    GPDMA_channel->DMACCControl |= 1ul <<DMA_CCxControl_I_Pos;
    // Unmask IE
    GPDMA_channel->DMACCConfig |= 1ul<< DMA_CCxConfig_IE_Pos;	
   

	  
		GPDMA_channel->DMACCControl |= DMA_InitStruct->DMA_TransferSize << DMA_CCxControl_TransferSize_Pos;
	  // Enable DMA 
    LPC_GPDMA->DMACConfig = 0x01;
    GPDMA_channel->DMACCConfig |= 1ul<<DMA_CCxConfig_E_Pos;
}


bool dma_channel_active (int channel)
{
    LPC_SC->PCONP |= (1 << 29);
    return (LPC_GPDMA->DMACEnbldChns && (1<<channel));
}


void dma_init(int channel, DMA_InitTypeDef* DMA_InitStruct)
{

    LPC_GPDMACH_TypeDef* GPDMA_channel;
    // Get DMA channel address
    GPDMA_channel = get_channel(channel);
    // Calcuate transfer type according to source address and destination address
    DMA_InitStruct->DMA_TransferType = get_transfer_type(DMA_InitStruct->DMA_SrcAddr, DMA_InitStruct->DMA_DstAddr);
    // Reset interrupt pending bits for DMA Channel
    LPC_GPDMA->DMACIntTCClear = 1<<channel;
    LPC_GPDMA->DMACIntErrClr = 1<<channel;

    /*--------------------------- DMAy channelx request select register ----------------*/
    // Choose UART or Timer DMA request for DMA inputs 8 through 15

    if ( (DMA_InitStruct->DMA_TriggerSource & 0x10) || (DMA_InitStruct->DMA_TriggerDestination & 0x10))
        LPC_SC->DMAREQSEL |= 1 << ((DMA_InitStruct->DMA_TriggerSource || DMA_InitStruct->DMA_TriggerSource) - 24); // I don't know why. The user manual says DMAREQSET is GPDMA register, but in mbed, it is LPC_SC register
    else
        LPC_SC->DMAREQSEL &= ~(1 << ((DMA_InitStruct->DMA_TriggerSource - 24)&& (DMA_InitStruct->DMA_TriggerDestination - 24)));


    /*--------------------------- DMAy channelx source and destination ----------------*/
    // Write to DMA channel source address
    GPDMA_channel->DMACCSrcAddr = DMA_InitStruct->DMA_SrcAddr;
    // Write to DMA channel destination address
    GPDMA_channel->DMACCDestAddr = DMA_InitStruct->DMA_DstAddr;

		#if BURST_ENABLED

		if (DMA_InitStruct->DMA_TransferType == M2M)
		{
		    DMA_InitStruct->DMA_SrcBurst = 7; // always set burst size as 256, as it seems always work no matter the source size.
        DMA_InitStruct->DMA_DestBurst = 7; // always set destination size as 256, as it seems always work no matter the source size.
		}
		#endif
		
 
    /*--------------------------- DMAy channelx control register ----------------*/
    // Set TransferSize
    // Set source burst size
    // Set destination burst size
    // Set source width size
    // Set destination width size
    // Set source increment
    // Set destination increment
    // Set destination increment
    // Enable terminal count interrupt
    GPDMA_channel->DMACCControl |= (DMA_InitStruct->DMA_SrcBurst<<DMA_CCxControl_SBSize_Pos)  |
                                   (DMA_InitStruct->DMA_DestBurst<<DMA_CCxControl_DBSize_Pos) |
                                   (DMA_InitStruct->DMA_SrcWidth<<DMA_CCxControl_SWidth_Pos)  |
                                   (DMA_InitStruct->DMA_DestWidth<<DMA_CCxControl_DWidth_Pos) |
                                   (DMA_InitStruct->DMA_SrcInc<<DMA_CCxControl_SI_Pos)    |
                                   (DMA_InitStruct->DMA_DestInc<<DMA_CCxControl_DI_Pos)   |
                                   (DMA_InitStruct->DMA_TermInt<<DMA_CCxControl_I_Pos );


    /*--------------------------- DMAy channelx configuration register ----------------*/
    // Set source periheral trigger. If the source is memory, this bit is ignored. Set according to low 4 bit of DMA_Trigger
    // Set destination periheral trigger. If the destination is memory, this bit is ignored. Set according low 4 bit of DMA_Trigger
    // Set transfer type: M2M, M2P, P2M, M2M
    GPDMA_channel->DMACCConfig |= ((DMA_InitStruct->DMA_TriggerSource & 0x0f) <<DMA_CCxConfig_SrcPeripheral_Pos)|    //set SrcPeripheral according low 4 bit of DMA_Trigger
                                  ((DMA_InitStruct->DMA_TriggerDestination & 0x0f)<<DMA_CCxConfig_DestPeripheral_Pos)|      //set DstPeripheral according low 4 bit of DMA_Trigger
                                  (DMA_InitStruct->DMA_TransferType<<DMA_CCxConfig_TransferType_Pos);
																	
																	
		//LPC_GPDMA->DMACSync =0x1;															

}//end of DMA_init


void dma_reset(int channel)
{
	  assert (channel <= _channel_num && channel>=0); 
    LPC_SC->PCONP |= (1 << 29);
    LPC_GPDMACH_TypeDef* GPDMA_channel;
    //Get DMA channel address
    GPDMA_channel = get_channel(channel);
    GPDMA_channel->DMACCConfig = 0;
    GPDMA_channel->DMACCControl = 0;
    GPDMA_channel->DMACCLLI = 0;
    GPDMA_channel->DMACCSrcAddr = 0;
    LPC_GPDMA->DMACIntErrClr |= 1 << channel; // clear the interrupt
    LPC_GPDMA->DMACIntTCClear |= 1 << channel;
	  dma_irq_error [channel] = 0;
	  dma_irq_finish[channel] = 0;	
}


DMA_InitTypeDef* dma_struct_create(void)
{
	  DMA_InitTypeDef* DMA_InitStruct = (DMA_InitTypeDef* ) malloc(sizeof(DMA_InitTypeDef));
    /*-------------- Reset DMA init structure parameters values ------------------*/
    DMA_InitStruct->DMA_DstAddr = 0;
    DMA_InitStruct->DMA_SrcAddr = 0;
    DMA_InitStruct->DMA_TransferSize = 0;
    DMA_InitStruct->DMA_SrcBurst = 0;
    DMA_InitStruct->DMA_DestBurst = 0;
    DMA_InitStruct->DMA_SrcWidth = 0;
    DMA_InitStruct->DMA_DestWidth = 0;
    DMA_InitStruct->DMA_LLI = 0;
    DMA_InitStruct->DMA_SrcInc = 0;
    DMA_InitStruct->DMA_DestInc = 0;
    DMA_InitStruct->DMA_TermInt = 0;
    DMA_InitStruct->DMA_TriggerSource = trigger_value[ALWAYS];
    DMA_InitStruct->DMA_TriggerDestination = trigger_value[ALWAYS];
    DMA_InitStruct->DMA_TransferType = M2M;
	  return DMA_InitStruct;
} //end of DMA_StructInit

void dma_struct_delete(DMA_InitTypeDef* ptr)
{
	free (ptr);
}

__inline static bool is_memory (uint32_t addr)
{
    if ((addr >> 28) == 0 || (addr >> 28)== 1)
        return 1;
    else
        return 0;
}


void  dma_IRQ_handler( void)
{
    //only call the attached function when certain interrupt happened on the right channel
		unsigned i ; 
	  uint32_t FinishStatus=0;
    uint32_t ErrStatus=0;
    FinishStatus = LPC_GPDMA->DMACIntTCStat & 0xFF;
    ErrStatus = LPC_GPDMA->DMACIntErrStat & 0xFF;
	
	 
    if (FinishStatus !=0) //only checking when there is interrupt happened
				for (i =0; i < 8; i++) // roundrobin checking how many channels get interrupts 
				{
				    if (FinishStatus & 1<<i)
						dma_irq_finish[i]();
				}
   if (ErrStatus !=0)			
	     for (i =0; i < 8; i++)	
			 {
						if (ErrStatus & 1<<i)
						dma_irq_error[i]();
			 }		 
  
    LPC_GPDMA->DMACIntTCClear |= FinishStatus;
    LPC_GPDMA->DMACIntErrClr |=ErrStatus;
}


void dma_IRQ_attach (int channel, int status, func_ptr ptr)
{
    assert (channel <= _channel_num && channel>=0);
    assert (status == ERR || status == FINISH);

    if (status == ERR)
        dma_irq_error[channel] = ptr;
    else if (status == FINISH)
        dma_irq_finish[channel] = ptr;
}



void dma_IRQ_detach (int channel)
{
    dma_irq_error[channel] = 0;
    dma_irq_finish[channel] = 0;
}




static TransferType get_transfer_type (uint32_t src_addr, uint32_t dst_addr )
{
    if (is_memory(src_addr)) {   //if source is memory
        if(is_memory(dst_addr))  //if destination is memory
            return M2M;    //return M2M if source is memory and destination is memory.
        else    //if destination is peripheral
            return M2P;    //return M2P if source is memory and destination is peripheral
    } else {        //if source is peripheral
        if(is_memory(dst_addr))      //if destination is memory
            return P2M;     //return P2M if source is peripheral and destination is memory
        else        //if destination is peripheral
            return P2P;     //return P2P if source is peripheral and destination is peripheral
    }
}

