# Known Issues

## About this document

This is the list of known issues for the 15.11 release of [mbed Client](https://github.com/ARMmbed/mbed-client). We publish mbed-client as module in GitHub, and track open issues as tickets in the repository. That makes for an easy development flow, but doesn’t make it very easy for you to get a single view of all the issues that affect the whole release. 

The purpose of this document is to provide that single view of all the key issues we are aware of. It isn’t a complete list; it’s a filtered and reviewed list, focusing on the issues we thought you’d want to be aware of. Each item explains the problem, as well as workarounds if those are possible. For items filed through GitHub, we’ve included a link to the issue so that you can follow the discussion and code - or even suggest a solution.

For more information about an issue, contact us on the [mbed forums](http://forums.mbed.com).

### Other information not in this document

We’re still very actively building mbed Client and the 15.11 release is a technology preview. As such there are some other limitations of the release that you can find described in the [release note](https://www.mbed.com/en/development/software/mbed-client/releases/mbed-client1511).

#### Important Note for Windows Users

If you are using this release on Microsoft Windows, please be advised that because of the way Windows handles filename and paths, you may have problems if you attempt to use this in a deep directory hierarchy with a long path name (e.g. `c:\some\very\long\path`). If you experience problems unpacking or building this release, please try it in a location with a shorter path before filing a bug. Thanks.

***
### mbed Client API may not be fully interoperable with other LWM2M servers.

*Description*: mbed Client API is OMA LWM2M compatible. However, some features may not be fully interoperable against other open source LWM2M servers. This is the first release version and more features will be made interoperable over coming releases.

*Workaround*: mbed Client is compatible with mbed Device Connector Service, which can be tested at [https://connector.mbed.com](https://connector.mbed.com).

*Reported Issue*: ARM internal reference IOTCLT-366

*Priority*: Major

### mbed Client for Linux doesn't support IPv6 connectivity

*Description*: mbed Client for Linux platform currently works only for IPv4-enabled LWM2M server addresses. IPv6 address support is currently broken.

*Workaround*: mbed Client is compatible with mbed Device Connector Service which can be tested from [https://connector.mbed.com](https://connector.mbed.com) over IPv4 Address.

*Reported Issue*: https://github.com/ARMmbed/mbed-client-linux/issues/19

*Priority*: Major

### mbed Client will occasionally disconnect from LWM2M Server because of network errors

*Description*: If mbed Client is kept running for long durations (over 24 hours) with long lifetime values, it occasionally  goes offline due to unstable network conditions - and doesn't send periodic updates to LWM2M server.

*Workaround*: Set the periodic lifetime value to less than 15 minutes if you want to run stability tests. Also, implement a network error handling mechanism on the application side, to handle `error()` callbacks received from the mbed-client library.

*Reported Issue*: ARM internal reference IOTCLT-206

*Priority*: Major

### mbed Client might experience a memory leak when running for long durations

*Description*: mbed Client might have memory leak issues when left running for longer than 12 hours.

*Workaround*: None

*Reported Issue*: ARM internal reference IOTCLT-290

*Priority*: Major

