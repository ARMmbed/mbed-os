Content of this directory is copied from https://github.com/butok/FNET.

Selected FNET services are ported to work in nanostack environment, for example mDNS services. 
Others, like FNET TCP/IP stack functionality, documentation, demos or tools is not copied here.

FNET services will be used by using dedicated nanostack API. Usage of FNET services directly from nanostack is not allowed.
Nanostack adaptation layer and API for using FNET services locates one level above this directory.

This directory contains portions of the FNET source code that is needed to get selected FNET services to work in 
nanostack environment. Below is a directory structure of the original FNET stack:

FNET TCP/IP Stack.

Please read the FNET Quick Start chapter. You can find it in the /fnet_doc/fnet_user_manual.chm file.

FNET project directory structure:
	fnet                        - FNET root directory.
	|-- fnet_stack                - FNET TCP/IP stack.
	|    |-- stack                  - FNET TCP/IP stack platform-independent source code.
	|    |-- services               - FNET Services.
	|    |    |-- autoip              - Auto-IP service.
	|    |    |-- dhcp                - DHCP/BOOTP client service.
	|    |    |-- dns                 - DNS client/resolver service.
	|    |    |-- flash               - Flash  Memory driver.
	|    |    |-- fs                  - File System driver.
	|    |    |-- http                - HTTP Server service.
	|    |    |-- link                - Link-Detection service.
	|    |    |-- llmnr               - LLMNR Server/Responder.
	|    |    |-- mdns                - mDNS Server/Responder.
	|    |    |-- ping                - PING service.
	|    |    |-- poll                - Polling Mechanism library.
	|    |    |-- serial              - Serial Input/Output driver.
	|    |    |-- shell               - Command Shell service.
	|    |    |-- telnet              - TELNET server service.
	|    |    |-- tftp                - TFTP server and client services.
	|    |    |-- tls                 - TLS library.
	|    |-- port                   - FNET port-specific source code.
	|         |-- cpu                 - FNET platform-specific source code.
	|         |    |-- common           - Common platform-specific source code.
	|         |    |-- mcf              - ColdFire-specific source code.
	|         |    |-- mk               - Kinetis-specific source code.
	|         |    |-- mpc              - Power Architecture-specific source code.
	|         |-- compiler            - Compiler-specific source code.
	|-- fnet_doc                  - FNET documentation.
	|-- fnet_demos                - FNET demo projects.
	|    |-- common                 - Common source code used by demos.
	|    |    |-- fnet_application    - Demo application source code.
	|    |    |-- fnet_webpage        - Demo web page source code.
	|    |    |-- startup             - Platform-specific startup source code.
	|    |-- boards                 - Demo projects.
	|         |-- <board_name>        - Demo projects for a <board_name> board.
	|              |-- boot           - FNET TFTP Bootloader.
	|              |-- benchmark      - TCP/UDP Throughput Benchmark application. 
	|              |-- shell          - "Shell" demo is a fully featured shell. It allows
	|              |                    view/change various parameters (e.g. IP address,
	|              |                    subnet mask), explore mounted file systems, run 
	|              |                    DHCP client, HTTP and Telnet servers.
	|              |-- shell_boot     - Example application used for the FNET TFTP Bootloader
	|              |                    demonstration. This demo has the same features as
	|              |                    the "Shell" demo, but it`s modified to be able to
	|              |                    work with the FNET Bootloader.
	|              |-- shell_mbedtls  - Example application used for the HTTPS server demonstration.
	|                                   This demo has the same features as the "Shell" demo,
	|                                   but in addition it includes the mbedTLS library.
	|-- fnet_tools                - FNET tools.
	|-- third_party               - Optional, third-party libraries.
	|    |-- mbedtls-2.4.0          - mbedTLS library, used by FNET TLS.


