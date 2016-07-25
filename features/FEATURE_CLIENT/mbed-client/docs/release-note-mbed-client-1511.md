# mbed Client 15.11 Technology Preview Release Note

This is the first Technology Preview of mbed Client, which is available for mbed OS as well as Linux. We’re actively working on mbed Client and we expect to make exciting changes in the next six months. We’re making this technology preview available so you can see the trajectory we’re on. Our focus in this release is on laying the foundation for mbed Client development and collaboration, particularly core tools, technology and testing.

We expect mbed Client developers to be able to access, build and run example projects and to explore the underlying code.

## About this release

Note that this is a technology preview release, which means that this release is not yet suitable for volume production use. The software is still maturing, and a number of things may change, including module names, repository URLs, APIs, header file names and configuration parameters. We'll try to mitigate the impact that these changes have on your code where possible.

Note that in this release we're changing our version numbering scheme for mbed Client releases, to a calendar-based (year and month YY.MM) scheme. This release (15.11) has previously been called 3.0 in some communications.

## Collaboration

We’re building mbed Client as a collaborative project, bringing together industry and open source community contributions. If you’d like to work on mbed Client with us, we’d encourage you [to pitch in](https://github.com/ARMmbed/mbed-os/blob/master/CONTRIBUTING.md). With this technology preview we’re ready to start receiving contributions back from the community.

## Documentation

To get started with mbed Client, please visit our [getting started guide](https://docs.mbed.com/docs/mbed-client-guide/en/latest/).

## Changes since the last release

This section documents the changes between this release and the earlier mbed Client Beta (15.09) release.

This release contains the following new features:
Securely connect to mbed Device Server (mbed DS) over TCP connection through TLS. The supported secure connection includes Certificate mode. We still support non-secure connection mode for fast development and debugging.

New LWM2M Firmware Object class preview for application development.

## Known issues

The known issues for this release are described [on GitHub](https://github.com/ARMmbed/mbed-client/blob/master/docs/known-issues-mbed-client1511.md).

## Other ways of accessing this release

We prefer that you access and collaborate with mbed Client online. However, the release may also be downloaded as an archive [on mbed.com](https://www.mbed.com/en/development/software/mbed-client/releases/mbed-client1511/).

# Module versions in this release

This release comprises the following yotta modules and their versions:
 

| Module                           |  Version   |
|----------------------------------|------------|
| `mbed-client`                    |   1.2.1	   |
| `mbed-client-c`                  |   1.1.1	   |
| `mbed-client-libservice`         |   3.0.8	   |
| `mbed-client-linux`              |   1.1.0	   |
| `mbed-client-linux-example`  	   |   1.0.0	   |
| `mbedtls`  	 		                  |   2.2.0	   |
| `mbed-client-mbed-tls`           |   1.0.9	   |
| `target-x86-linux-native`        |   1.0.0	   |
| `target-linux-native`            |   1.0.0	   |



