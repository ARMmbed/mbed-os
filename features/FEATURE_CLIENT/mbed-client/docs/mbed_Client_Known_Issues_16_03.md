# mbed Client Known Issues

## About this document

This is the list of known issues for the [16.03 release of mbed Client](Release_Note.md).

We publish mbed Client as a collection of modules on GitHub. Issues are raised in the specific repositories and then tracked internally. 

The purpose of this document is to provide a single view of the outstanding key issues that have not been addressed for this release. As such it is a filtered and reviewed list based on priority and potential impact. Each item summarises the problem and includes any known workarounds, along with a link to the GitHub issue (if applicable). We welcome any comments or proposed solutions.

For more information about an issue, contact us on the [forums](http://forums.mbed.com).

## Additional information

For further information regarding this release please refer to the release notes referenced above.

# Known issues

## mbed Client API does not support float data type
* **Description**: mbed Client API doesn't work correctly with float data type, as it is disabled in the FRDM target configuration.
* **Workaround**: No workaround at the moment. There will be a fix for target-mbed-gcc soon, which will enable float data type usage.
* **Reported Issue**: ARM internal reference IOTCLT-649
* **Priority**: Critical

## mbed-trace module has a missing test dependency on Linux and Windows build targets.
* **Description**: The mbed-trace library has a test-dependency on cpputest, which is not yet available in the yotta registry. Since you can't download this dependency, "yt test" for mbed-trace will fail.
* **Workaround**: No workaround available.
* **Reported Issue**: ARM internal reference IOTCLT-650
* **Priority**: Critical

## mbed Client API may not be fully interoperable with other LWM2M servers.
* **Description**: mbed Client API is OMA LWM2M compatible. However, some features may not be fully interoperable against other open source LWM2M servers.
* **Workaround**: mbed Client is compatible with mbed Device Connector Service, which can be tested at [https://connector.mbed.com](https://connector.mbed.com).
* **Reported Issue**: ARM internal reference IOTCLT-366
* **Priority**: Major

## mbed Client example application may drop off from Connector if kept running for a long duration.
* **Description**: mbed Client example application sometimes drops off from Connector after a few hours. This may happen when mbed Client is kept running for long durations (over 24 hours) - it occasionally goes offline due to unstable network conditions, and doesn't send periodic updates to Connector.
* **Workaround**: Set the periodic lifetime value to less than 15 minutes if you want to run stability tests. Also, implement a network error handling mechanism on the application side, to handle error() callbacks received from the mbed-client library.
* **Reported Issue**: [https://github.com/ARMmbed/mbed-client-examples/issues/52](https://github.com/ARMmbed/mbed-client-examples/issues/52)
* **Priority**: Major

