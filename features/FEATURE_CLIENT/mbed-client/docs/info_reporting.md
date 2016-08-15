# The Information Reporting feature

mbed Device Server uses the Information Reporting to observe any changes in a registered Resource on the client. The server registers to observe the Resource. When the Resource changes, it sends a notification with its new value to all servers who asked to observe it.

The interface supports the following sub-features:

## The Observe feature

mbed Device Server initiates an observation request to observe the changing value of either an Object, Object Instance or Resource.

<span class="tips">**Tip:** Related parameters for the **Observe** operation are described in the [Write Attributes](dev_man_serv_enable.md#write-attributes) section.</span>

## The Notify feature

The client sends the **Notify** operation to mbed Device Server during a valid observation on a Resource, when the notification conditions are met.

## Cancel

mbed Device Server sends the **Cancel Observation** operation to the client to end an observation relationship for an Object Instance or a Resource.

