
#include "dma.h"

namespace mbed {


DMA::DMA (int priority )
{
    chan = _choose_free_channel(priority) ;
    channel_num = _channel_num;
    dma_reset(chan);// reset the register value to default and clear the interrupts
    dma_init_struct = dma_struct_create(); // initialize the dma data structure to default value
}


DMA::~DMA(){
   dma_struct_delete(dma_init_struct);
	 dma_IRQ_detach(chan);
}

// If no channel choosed, choose whichever free channel, otherwise, wait until the channel is free
int DMA::_choose_free_channel (int channel)
{
    int reval=channel;
	  MBED_ASSERT((channel<channel_num && channel >=0) || channel == -1);
	
    if (channel == -1) {
        reval = 0;
        while (1) { // If not chosen channel, round robin checked which channel is free
            if (!dma_channel_active(reval))
                return reval;
            reval++;
            if (reval>(channel_num-1))
                reval=0;
        }
    } 
		else
		{
		    while (dma_channel_active(reval)); //if has chosen the channel, wait until the channel is free
		}
    return reval;
}

void DMA::trigger_source(TriggerType trig)
{
    dma_trigger_source(dma_init_struct, trig);
}

void DMA::trigger_destination(TriggerType trig)
{
    dma_trigger_destination(dma_init_struct, trig);
}



void DMA::start (unsigned int len)
{
  	dma_init(chan,dma_init_struct);
	  dma_start (chan, dma_init_struct, len);
}

void DMA::wait()
{
    while (dma_channel_active(chan));
}
}
