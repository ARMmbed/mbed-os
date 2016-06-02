
/*  Config-Store Client API Usage Call Flow Web Sequence Diagram data
 *
 * The following can be pasted into Web Sequence Diagram tool here:
 *   https://www.websequencediagrams.com/
 * This will generate a picture of the typicall call flow between a Config-Store
 * Client and the Config-Store API.
 */

##################################################################################
###### Config-Store Client API Usage Call Flow
##################################################################################

--- CUT_HERE-----------------------------------------------------------------------
title Config-Store Client API Usage Call Flow

cfstore_Client->Config-Store: cfstore_open_ctx(security_key_prefix)
Config-Store->cfstore_Client: returns hOwner

note left of cfstore_Client: Client find
cfstore_Client->Config-Store: cfstore_handle cfstore_find(cfstore_handle owner, const char* key_name_query, cfstore_handle previous=NULL)
Config-Store->cfstore_Client: returns key_handle to matching key_name

note left of cfstore_Client: Client uses key_handle as prev argument
cfstore_Client->Config-Store: cfstore_handle cfstore_find(cfstore_handle owner, const char* key_name_query, cfstore_handle previous=key_handle)
Config-Store->cfstore_Client: returns key handle to matching key_name

note over cfstore_Client,Config-Store: client iterates to get complete find list

note left of cfstore_Client: cfstore_Client create new object
cfstore_Client->Config-Store: cfstore_open(hOwner, key_name, valueL, kdesc with O_CREATE)
Config-Store->cfstore_Client: returns hObj

note left of cfstore_Client: cfstore_Client performs operations if required

note left of cfstore_Client: cfstore_Client close new object hObj
cfstore_Client->Config-Store: cfstore_close(hObj)
Config-Store->cfstore_Client: OK


note left of cfstore_Client: cfstore_Client opens pre-existing object
cfstore_Client->Config-Store: cfstore_open(hOwner, key_name, NULL, NULL)
Config-Store->cfstore_Client: returns hObj

note left of cfstore_Client: cfstore_Client seeks in pre-existing object
cfstore_Client->Config-Store: cfstore_rseek(hObj, ... )
Config-Store->cfstore_Client: returns data

note left of cfstore_Client: cfstore_Client reads pre-existing object
cfstore_Client->Config-Store: cfstore_read(hObj, ... )
Config-Store->cfstore_Client: returns data

note left of cfstore_Client: cfstore_Client closes pre-existing object
cfstore_Client->Config-Store: cfstore_close(hObj)
Config-Store->cfstore_Client: OK


note left of cfstore_Client: cfstore_Client closes security context with Config-Store
cfstore_Client->Config-Store: cfstore_close_ctx(hOwner)
Config-Store->cfstore_Client: OK
--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### GetVersion(), GetCapabilities
##################################################################################

--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API GetVersion(), GetCapabilities()

Cfstore_Client->Cfstore: GetVersion()
Cfstore->Cfstore_Client: returns ARM_DRIVER_VERSION

Cfstore_Client->Cfstore: GetCapabilities()
Cfstore->Cfstore_Client: returns ARM_Cfstore_Capabilities_t
--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Initialise(), Uninitialise() (Sync. Case, Success)
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Initialise(), Uninitialise() (Sync. Case, Success)

Cfstore_Client->Cfstore: (1) Initialise()
Cfstore->Cfstore_Client: (2) returns OK (success)

note over Cfstore_Client, Cfstore: (3) Perform Other Operations ...

Cfstore_Client->Cfstore: (4) Uninitialise()
Cfstore->Cfstore_Client: (5) returns OK
--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Initialise(), Uninitialise() (Async. Case, Success)
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Initialise(), Uninitialise() (Async. Case, Success) 

Cfstore_Client->Cfstore: (1) Initialise(Cfstore_Client_callback, client context)
Cfstore->Cfstore_Client: (2) returns OK (pending)

Cfstore->Cfstore: (3) Internal Initialisation Operations

Cfstore->Cfstore_Client: (4) Cfstore_Client_callback(OPCODE=INITIALISE, status, client context)

note over Cfstore_Client, Cfstore: (5) Perform Other Operations ...

Cfstore_Client->Cfstore: (6) Uninitialise()
Cfstore->Cfstore_Client: (7) returns OK (pending)
Cfstore->Cfstore: (8) Internal De-Initialisation Operations
Cfstore->Cfstore_Client: (9) Cfstore_Client_callback(OPCODE=UNINITIALISE, status, client context)

--- CUT_HERE-----------------------------------------------------------------------


##################################################################################
###### Create A Key (Synch. Success Case)
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Create A Key (Synch. Success Case) 

Cfstore_Client->Cfstore: (1) Create(key_name)
Cfstore->Cfstore_Client: (2) returns OK (success), key_handle

Cfstore_Client->Cfstore: (3) Write(key_handle, value_data)
Cfstore->Cfstore_Client: (4) returns OK (success)

note over Cfstore_Client, Cfstore: (5) Perform Other Operations ...

Cfstore_Client->Cfstore: (6) Close(key_handle)
Cfstore->Cfstore_Client: (7) returns OK (success)

opt (8) Repeat Key Creation Operation as many times as required
	note over Cfstore_Client, Cfstore: Repeat operations (1)-(7) as required.
end

Cfstore_Client->Cfstore: (9) Flush()
Cfstore->Cfstore_Client: (10) returns OK (success)
--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Create A Key (Asynch. Success Case)
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Create A Key (Async. Success Case) 

Cfstore_Client->Cfstore: (1) Create(key_name)
Cfstore->Cfstore_Client: (2) returns OK (pending)
Cfstore->Cfstore: (3) Internal Operations
Cfstore->Cfstore_Client: (4) Cfstore_Client_callback(OPCODE=CREATE, status, client context, key_handle)

Cfstore_Client->Cfstore: (5) Write(key_handle, value_data)
Cfstore->Cfstore_Client: (6) returns OK (pending)
Cfstore->Cfstore: (7) Internal Operations
Cfstore->Cfstore_Client: (8) Cfstore_Client_callback(OPCODE=WRITE, status, client context, key_handle)

note over Cfstore_Client, Cfstore: (9) Perform Other Operations ...

Cfstore_Client->Cfstore: (10) Close(key_handle)
Cfstore->Cfstore_Client: (11) returns OK (pending)
Cfstore->Cfstore: (12) Internal Operations
Cfstore->Cfstore_Client: (13) Cfstore_Client_callback(OPCODE=CLOSE, status, client context, key_handle)

opt (14) Repeat Key Creation Operation as many times as required
	note over Cfstore_Client, Cfstore: Repeat operations (1)-(13) as required.
end

Cfstore_Client->Cfstore: (15) Flush()
Cfstore->Cfstore_Client: (16) returns OK (pending)
Cfstore->Cfstore: (17) Internal Operations
Cfstore->Cfstore_Client: (18) Cfstore_Client_callback(OPCODE=FLUSH, status, client context)

--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Open Pre-Existing Key, Read, RSeek, Close(Synch. Success Case)
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Open Pre-Existing Key, Read, Rseek, Close (Synch. Success Case) 

Cfstore_Client->Cfstore: (1) Open(key_name, READONLY)
Cfstore->Cfstore_Client: (2) returns OK (success), key_handle

Cfstore_Client->Cfstore: (3) Rseek(key_handle, offset)
Cfstore->Cfstore_Client: (4) returns OK (success)

Cfstore_Client->Cfstore: (5) Read(key_handle, buffer)
Cfstore->Cfstore_Client: (6) returns OK (success), value data

note over Cfstore_Client, Cfstore: (7) Perform Other Operations ...

Cfstore_Client->Cfstore: (8) Close(key_handle)
Cfstore->Cfstore_Client: (9) returns OK (success)

opt (10) Repeat Key Open() Operations as many times as required
	note over Cfstore_Client, Cfstore: Repeat operations (1)-(9) as required.
end
--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Open Pre-Existing Key, Read, RSeek, Close(Asynch. Success Case)
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Open Pre-Existing Key, Read, Rseek, Close (Asynch. Success Case) 

Cfstore_Client->Cfstore: (1) Open(key_name, READONLY)
Cfstore->Cfstore_Client: (2) returns OK (pending)
Cfstore->Cfstore: (3) Internal Operations
Cfstore->Cfstore_Client: (4) Cfstore_Client_callback(OPCODE=OPEN, status, client context, key_handle)

Cfstore_Client->Cfstore: (5) Rseek(key_handle, offset)
Cfstore->Cfstore_Client: (6) returns OK (pending)
Cfstore->Cfstore: (7) Internal Operations
Cfstore->Cfstore_Client: (8) Cfstore_Client_callback(OPCODE=RSEEK, status, client context, key_handle)

Cfstore_Client->Cfstore: (9) Read(key_handle, buffer)
Cfstore->Cfstore_Client: (10) returns OK (pending)
Cfstore->Cfstore: (11) Internal Operations
Cfstore->Cfstore_Client: (12) Cfstore_Client_callback(OPCODE=READ, status, client context, key_handle)

note over Cfstore_Client, Cfstore: (13) Perform Other Operations ...

Cfstore_Client->Cfstore: (14) Close(key_handle)
Cfstore->Cfstore_Client: (15) returns OK (pending)
Cfstore->Cfstore: (16) Internal Operations
Cfstore->Cfstore_Client: (17) Cfstore_Client_callback(OPCODE=CLOSE, status, client context)

opt (18) Repeat Key Open() Operations as many times as required
	note over Cfstore_Client, Cfstore: Repeat operations (1)-(17) as required.
end

--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Open Pre-Existing Key, Write, Close, Flush (Synch. Success Case)
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Open Pre-Existing Key, Write, Close, Flush (Synch. Success Case) 

Cfstore_Client->Cfstore: (1) Open(key_name)
Cfstore->Cfstore_Client: (2) returns OK (success), key_handle

Cfstore_Client->Cfstore: (3) Write(key_handle, value_data)
Cfstore->Cfstore_Client: (4) returns OK (success)

note over Cfstore_Client, Cfstore: (5) Perform Other Operations ...

Cfstore_Client->Cfstore: (6) Close(key_handle)
Cfstore->Cfstore_Client: (7) returns OK (success)

opt (8) Repeat Key Open() Operations as many times as required
	note over Cfstore_Client, Cfstore: Repeat operations (1)-(7) as required.
end

Cfstore_Client->Cfstore: (9) Flush()
Cfstore->Cfstore_Client: (10) returns OK (success)

--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Open Pre-Existing Key, Write, Close, Flush (Asynch. Success Case)
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Open Pre-Existing Key, Write, Close, Flush (Asynch. Success Case) 

Cfstore_Client->Cfstore: (1) Open(key_name)
Cfstore->Cfstore_Client: (2) returns OK (pending)
Cfstore->Cfstore: (3) Internal Operations
Cfstore->Cfstore_Client: (4) Cfstore_Client_callback(OPCODE=OPEN, status, client context, key_handle)

Cfstore_Client->Cfstore: (5) Write(key_handle, value_data)
Cfstore->Cfstore_Client: (6) returns OK (pending)
Cfstore->Cfstore: (7) Internal Operations
Cfstore->Cfstore_Client: (8) Cfstore_Client_callback(OPCODE=WRITE, status, client context, key_handle)

note over Cfstore_Client, Cfstore: (9) Perform Other Operations ...

Cfstore_Client->Cfstore: (10) Close(key_handle)
Cfstore->Cfstore_Client: (11) returns OK (pending)
Cfstore->Cfstore: (12) Internal Operations
Cfstore->Cfstore_Client: (13) Cfstore_Client_callback(OPCODE=CLOSE, status, client context)

opt (14) Repeat Key Open() Operations as many times as required
	note over Cfstore_Client, Cfstore: Repeat operations (1)-(13) as required.
end

Cfstore_Client->Cfstore: (15) Flush()
Cfstore->Cfstore_Client: (16) returns OK (pending)
Cfstore->Cfstore: (17) Internal Operations
Cfstore->Cfstore_Client: (18) Cfstore_Client_callback(OPCODE=FLUSH, status, client context)

--- CUT_HERE-----------------------------------------------------------------------

Currently not used:
##################################################################################
###### Open Pre-Existing Key, Read, Rseek, Write, Close, Flush (Synch. Success Case)
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Open Pre-Existing Key, Read, Rseek, Write, Close, Flush (Synch. Success Case) 

Cfstore_Client->Cfstore: (1) Open(key_name)
Cfstore->Cfstore_Client: (2) returns OK (success), key_handle

Cfstore_Client->Cfstore: (3) Rseek(key_handle, offset)
Cfstore->Cfstore_Client: (4) returns OK (success)

Cfstore_Client->Cfstore: (5) Read(key_handle, buffer)
Cfstore->Cfstore_Client: (6) returns OK (success), value data

Cfstore_Client->Cfstore: (7) Write(key_handle, value_data)
Cfstore->Cfstore_Client: (8) returns OK (success)

note over Cfstore_Client, Cfstore: (9) Perform Other Operations ...

Cfstore_Client->Cfstore: (10) Close(key_handle)
Cfstore->Cfstore_Client: (11) returns OK (success)

opt (12) Repeat Key Open() Operations as many times as required
    note over Cfstore_Client, Cfstore: Repeat operations (1)-(11) as required.
end

Cfstore_Client->Cfstore: (13) Flush()
Cfstore->Cfstore_Client: (14) returns OK (success)

--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Finding Keys, (Synch. Full Walk, Success Case)
##################################################################################

--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Finding Keys, (Synch. Full Walk, Success Case) 

Cfstore_Client->Cfstore: (1) Find(key_name_search_string, prev=NULL, &next)
Cfstore->Cfstore_Client: (2) returns OK (success), next

note left of Cfstore_Client: (3) prev = next
Cfstore_Client->Cfstore: (4) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (5) returns OK (success), next

note left of Cfstore_Client: (6) prev = next
Cfstore_Client->Cfstore: (7) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (8) returns OK (success), next

note over Cfstore_Client, Cfstore: (9) Client decides this KV requires changes and performs operations as required.

note over Cfstore_Client, Cfstore: (10) Perform additional calls to Find()

note left of Cfstore_Client: (11) prev = next
Cfstore_Client->Cfstore: (12) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (13) returns OK (success), next=NULL

--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Finding Keys, (Asynch. Full Walk, Success Case)
##################################################################################

--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Finding Keys, (Asynch. Full Walk, Success Case) 

Cfstore_Client->Cfstore: (1) Find(key_name_search_string, &next, prev=NULL)
Cfstore->Cfstore_Client: (2) returns OK (pending)
Cfstore->Cfstore: (3) Internal Operations
Cfstore->Cfstore_Client: (4) Cfstore_Client_callback(OPCODE=FIND, status, client context, next)

note left of Cfstore_Client: (5) prev = next
Cfstore_Client->Cfstore: (6) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (7) returns OK (pending)
Cfstore->Cfstore: (8) Internal Operations
Cfstore->Cfstore_Client: (9) Cfstore_Client_callback(OPCODE=FIND, status, client context, next)

note left of Cfstore_Client: (10) prev = next
Cfstore_Client->Cfstore: (11) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (12) returns OK (pending)
Cfstore->Cfstore: (13) Internal Operations
Cfstore->Cfstore_Client: (14) Cfstore_Client_callback(OPCODE=FIND, status, client context, next)

note over Cfstore_Client, Cfstore: (15) Client decides this KV requires changes and performs operations as required.

note over Cfstore_Client, Cfstore: (16) Perform additional calls to Find()

note left of Cfstore_Client: (17) prev = next
Cfstore_Client->Cfstore: (18) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (19) returns OK (pending)
Cfstore->Cfstore: (20) Internal Operations
Cfstore->Cfstore_Client: (21) Cfstore_Client_callback(OPCODE=FIND, status, client context, next=NULL)

--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Finding Keys, (Synch. Early Termination of Walk, Success Case) 
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Finding Keys, (Synch. Early Termination of Walk, Success Case) 

Cfstore_Client->Cfstore: (1) Find(key_name_search_string, &next, prev=NULL)
Cfstore->Cfstore_Client: (2) returns OK (success), next=key_handle

note left of Cfstore_Client: (3) prev = next
Cfstore_Client->Cfstore: (4) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (5) returns OK (success), next=key_handle

note left of Cfstore_Client: (6) prev = next
Cfstore_Client->Cfstore: (7) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (8) returns OK (success), next=key_handle

note over Cfstore_Client, Cfstore: (9) Continue until find desired item and then stop

note left of Cfstore_Client: (10) prev = next
Cfstore_Client->Cfstore: (11) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (12) returns OK (success), next=key_handle

note over Cfstore_Client, Cfstore: (13) Found Desired item, use item and then close key to terminate walk

Cfstore_Client->Cfstore: (14) Close(key_handle)
Cfstore->Cfstore_Client: (15) returns OK (success)


##################################################################################
###### Finding Keys, (Asynch. Early Termination of Walk, Success Case) 
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Finding Keys, (Asynch. Early Termination of Walk, Success Case) 

Cfstore_Client->Cfstore: (1) Find(key_name_search_string, &next, prev=NULL)
Cfstore->Cfstore_Client: (2) returns OK (pending)
Cfstore->Cfstore: (3) Internal Operations
Cfstore->Cfstore_Client: (4) Cfstore_Client_callback(OPCODE=FIND, status, client context, next)

note left of Cfstore_Client: (5) prev = next
Cfstore_Client->Cfstore: (6) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (7) returns OK (pending)
Cfstore->Cfstore: (8) Internal Operations
Cfstore->Cfstore_Client: (9) Cfstore_Client_callback(OPCODE=FIND, status, client context, next)

note over Cfstore_Client, Cfstore: (10) Perform additional calls to Find()

note left of Cfstore_Client: (11) prev = next
Cfstore_Client->Cfstore: (12) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (13) returns OK (pending)
Cfstore->Cfstore: (14) Internal Operations
Cfstore->Cfstore_Client: (15) Cfstore_Client_callback(OPCODE=FIND, status, client context, next)

note over Cfstore_Client, Cfstore: (16) Found desired item, use item and then close key to terminate walk

Cfstore_Client->Cfstore: (17) Close(next)
Cfstore->Cfstore_Client: (18) returns OK (pending)
Cfstore->Cfstore: (19) Internal Operations
Cfstore->Cfstore_Client: (20) Cfstore_Client_callback(OPCODE=CLOSE, status, client context)
--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Finding Keys, (Synch. Full and Part Walk, Success Case) 
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Finding Keys, (Synch. Full and Part Walk, Success Case) 

Cfstore_Client->Cfstore: (1) Find(key_name_search_string, prev=NULL, &next)
Cfstore->Cfstore_Client: (2) returns OK (success), next

note left of Cfstore_Client: (3) prev = next
Cfstore_Client->Cfstore: (4) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (5) returns OK (success), next

note left of Cfstore_Client: (6) prev = next
Cfstore_Client->Cfstore: (7) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (8) returns OK (success), next


alt Use Case For Full Walk of All Matching Results

    note over Cfstore_Client, Cfstore: (9) Client decides this KV requires changes and performs operations as required.
    
    note over Cfstore_Client, Cfstore: (10) Perform additional calls to Find()

    note left of Cfstore_Client: (11) prev = next
    Cfstore_Client->Cfstore: (12) Find(key_name_search_string, prev, &next)
    Cfstore->Cfstore_Client: (13) returns OK (success), next=NULL

else Use Case For Partial Walk of All Matching Results

    note over Cfstore_Client, Cfstore: (9) Found Desired item, use item and then close key to terminate walk
    
    Cfstore_Client->Cfstore: (10) Close(key_handle)
    Cfstore->Cfstore_Client: (11) returns OK (success)
end
--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Finding Keys, (Asynch. Full and Part Walk, Success Case) 
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Finding Keys, (Asynch. Full and Part Walk, Success Case) 

Cfstore_Client->Cfstore: (1) Find(key_name_search_string, &next, prev=NULL)
Cfstore->Cfstore_Client: (2) returns OK (pending)
Cfstore->Cfstore: (3) Internal Operations
Cfstore->Cfstore_Client: (4) Cfstore_Client_callback(OPCODE=FIND, status, client context, next)

note left of Cfstore_Client: (5) prev = next
Cfstore_Client->Cfstore: (6) Find(key_name_search_string, prev, &next)
Cfstore->Cfstore_Client: (7) returns OK (pending)
Cfstore->Cfstore: (8) Internal Operations
Cfstore->Cfstore_Client: (9) Cfstore_Client_callback(OPCODE=FIND, status, client context, next)

alt Use Case For Full Walk of All Matching Results

    note over Cfstore_Client, Cfstore: (10) Found first desired item. Client performs operations as required.

    note over Cfstore_Client, Cfstore: (11) Perform additional calls to Find()
    
    note left of Cfstore_Client: (12) prev = next
    Cfstore_Client->Cfstore: (13) Find(key_name_search_string, prev, &next)
    Cfstore->Cfstore_Client: (14) returns OK (pending)
    Cfstore->Cfstore: (15) Internal Operations
    Cfstore->Cfstore_Client: (16) Cfstore_Client_callback(OPCODE=FIND, status, client context, next=NULL)
    

else Use Case For Partial Walk of All Matching Results

    note over Cfstore_Client, Cfstore: (10) Found desired item, use item and then close key to terminate walk
    
    Cfstore_Client->Cfstore: (11) Close(next)
    Cfstore->Cfstore_Client: (12) returns OK (pending)
    Cfstore->Cfstore: (13) Internal Operations
    Cfstore->Cfstore_Client: (14) Cfstore_Client_callback(OPCODE=CLOSE, status, client context)

end

--- CUT_HERE-----------------------------------------------------------------------

##################################################################################
###### Setting Power Level with PowerControl() (Synch. Success Case) 
##################################################################################
--- CUT_HERE-----------------------------------------------------------------------
title Configuration-Store Client API: Setting Power Level with PowerControl() (Synch. Success Case) 

Cfstore_Client->Cfstore: (1) PowerControl(level)
Cfstore->Cfstore_Client: (2) returns OK (success)
--- CUT_HERE-----------------------------------------------------------------------

