mbed Client data types
======================

This chapter describes how the different data types are processed in the mbed Client and the REST API.

The following two aspects are considered:
 
- How the different data types should be passed to API on mbed Client.
- How the data is received on REST API side, and how it is converted into right data type.

## Using mbed Client data types

When data is transferred from mbed Client to mbed Device Server, the following need to be kept in mind:

- mbed Client is based on CoAP client. The Resource values are represented as text/plain format. 
- Different data type values have to be converted into corresponding text/plain format.
- For example, an integer value 100 is passed in a text buffer format of 100 and it appears on the web service side as 100 in text format. 
- The service must interpret this data to an appropriate format.

### Transferring integers from mbed Client to mbed Device Server

**Integer (from the application)**:

```
int value = 1;
M2MResource* instance = objinst->create_dynamic_resource(“D", “IntegerType“,M2MResourceInstance::INTEGER, true);
char buffer[20];
int size = sprintf(buffer,"%d",value);
instance->set_value((const uint8_t*)buffer, size);
```

If you want the value of the resource to be stored in the server cache for a certain period of time, you can set the `max_age` parameter. Subsequent requests to the same resource will get the response directly from the cache:

```
instance->set_max_age(15) ; // This is the resource lifetime in the server cache (in seconds).
```

**Response on the REST API**:

```
{"async-response-id":...{ "id" : "1131895768#f4b93d6e-4652-4874-82e4-41a3ced0cd56@b02ac40a-a501-44d9-be29-e6e0e5303f85/Test/0/D", "status" : 200, "payload" : "MQ==", "ct" : "text/plain", "max-age" : 0 }
```

- Base64 encoded value of the payload -> `MQ==`
- Base64 decoded value of the payload -> `1`

<span class="notes">**Note**: If the resource value is stored in the server cache and is still valid, the response comes straight from the cache in **plain text**, neither inside any json object nor encoded in B64.</span>

### Transferring a float value from mbed Client to mbed Device Server

**Float (from the application)**:

```
float value = 1.0232f;
M2MResource* instance = objinst->create_dynamic_resource(“D", “IntegerType“,M2MResourceInstance::FLOAT, true);
char buffer[20];
int size = sprintf(buffer,"%f",_value);
instance->set_value((const uint8_t*)buffer, size);
```

If you want the value of the resource to be stored in the server cache for a certain period of time, you can set the `max_age` parameter. Subsequent requests to the same resource will get the response directly from the cache:

```
instance->set_max_age(15) ; // This is the resource lifetime in the server cache (in seconds).
```

**Response on the REST API**:

```
{"async-response-id":...{ "id" : "1132945328#f4b93d6e-4652-4874-82e4-41a3ced0cd56@b02ac40a-a501-44d9-be29-e6e0e5303f85/Test/0/D", "status" : 200, "payload" : "MS4wMjMyMDA=", 
"ct" : "text/plain", "max-age" : 0 }
```

- Base64 encoded value of the payload -> `MS4wMjMyMDA=`
- Base64 decoded value of the payload -> `1.023200`

<span class="notes">**Note**: If the resource value is stored in the server cache and is still valid, the response comes straight from the cache in **plain text**, neither inside any json object nor encoded in B64.</span>

### Transferring a boolean value from mbed Client to mbed Device Server

**Boolean (from the application)**:

```
bool value = true;
M2MResource* instance = objinst->create_dynamic_resource(“D", “IntegerType“,M2MResourceInstance::BOOLEAN, true);
char buffer[20];
int size = sprintf(buffer,"%d",_value);
instance->set_value((const uint8_t*)buffer, size);
```

If you want the value of the resource to be stored in the server cache for a certain period of time, you can set the `max_age` parameter. Subsequent requests to the same resource will get the response directly from the cache:

```
instance->set_max_age(15) ; // This is the resource lifetime in the server cache (in seconds).
```

**Response on the REST API**:

```
{"async-response-id":….{ "id" : "1131895768#f4b93d6e-4652-4874-82e4-41a3ced0cd56@b02ac40a-a501-44d9-be29-e6e0e5303f85/Test/0/D", "status" : 200, "payload" : "MQ==", "ct" : "text/plain", "max-age" : 0 }
```

- Base64 encoded value of the payload -> `MQ==`
- Base64 decoded value of the payload -> `1`

<span class="notes">**Note**: If the resource value is stored in the server cache and is still valid, the response comes straight from the cache in **plain text**, neither inside any json object nor encoded in B64.</span>

### Transferring a string value from mbed Client to mbed Device Server

**String (from the application)**:

```
M2MResource* instance = objinst->create_dynamic_resource(“D", “IntegerType“,M2MResourceInstance::STRING, true);
char buffer[10] = “Test Data”;
instance->set_value((const uint8_t*)buffer, 10);
```

If you want the value of the resource to be stored in the server cache for a certain period of time, you can set the `max_age` parameter. Subsequent requests to the same resource will get the response directly from the cache:

```
instance->set_max_age(15) ; // This is the resource lifetime in the server cache (in seconds).
```

**Response on the REST API**:

```
{"async-response-id":... { "id" : "1133335779#f4b93d6e-4652-4874-82e4-41a3ced0cd56@b02ac40a-a501-44d9-be29-e6e0e5303f85/Test/0/D", "status" : 200, "payload" : "VGVzdCBEYXRh", 
"ct" : "text/plain", "max-age" : 0 }
```

- Base64 encoded value of the payload -> `VGVzdCBEYXRh`
- Base64 decoded value of the payload -> `Test Data`

<span class="notes">**Note**: If the resource value is stored in the server cache and is still valid, the response comes straight from the cache in **plain text**, neither inside any json object nor encoded in B64.</span>

### Transferring an opaque value from mbed Client to mbed Device Server

**Opaque(from the application)**:

```
M2MResource* instance = objinst->create_dynamic_resource(“D", “IntegerType“,M2MResourceInstance::OPAQUE, true);
uint8_t buffer[12] = “Opaque Data”;
instance->set_value(buffer, 12);
```

If you want the value of the resource to be stored in the server cache for a certain period of time, you can set the `max_age` parameter. Subsequent requests to the same resource will get the response directly from the cache:

```
instance->set_max_age(15) ; // This is the resource lifetime in the server cache (in seconds).
```

**Response on the REST API**:

```
{"async-response-id":... { "id" : "1134045351#f4b93d6e-4652-4874-82e4-41a3ced0cd56@b02ac40a-a501-44d9-be29-e6e0e5303f85/Test/0/D", "status" : 200, "payload" : "T3BhcXVlIERhdGEA", 
"ct" : "application/octet-stream", "max-age" : 0 }
```

- Base64 encoded value of the payload -> `T3BhcXVlIERhdGEA`
- Base64 decoded value of the payload -> `Opaque Data`

<span class="notes">**Note**: If the resource value is stored in the server cache and is still valid, the response comes straight from the cache in **plain text**, neither inside any json object nor encoded in B64.</span>
