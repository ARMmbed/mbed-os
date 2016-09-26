# Change Log

## [v4.0.3](https://github.com/ARMmbed/coap-service/releases/tag/v4.0.3) (15-Sep-2016)
[Full Changelog](https://github.com/ARMmbed/coap-service/compare/mbed-os-5.0-rc1...v4.0.3)

** New feature **

- Updated coap-service to use new structure from mbed-client-c 
- Set link layer security when opening socket.


**Merged pull requests:**

commit 0fcd1d4254ca30c2d96ef51a5df513912b5fb133 (HEAD, tag: v4.0.3, origin/master, origin/HEAD, master)
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Thu Sep 15 15:21:03 2016 +0300

    version v4.0.3

commit 4769b274ece0989cc43de614171bd20cca0b0a27
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Thu Sep 15 15:18:55 2016 +0300

    version v4.0.2

commit 078bd0db248955c7f09bc53fa1de3e1a2f2eac4e
Merge: 70447c3 bc636c0
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Thu Sep 15 15:17:41 2016 +0300

    Merge pull request #32 from ARMmbed/coap-option-tidy
    
    Coap option tidy

commit bc636c0621f0a895e62b6dc23ecd7a261f18ed7e (origin/coap-option-tidy)
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Thu Sep 15 10:42:29 2016 +0300

    mbed-client-c version updated

commit 73d5163e4ef07ee10b494290761cf977ae6a6f28
Author: Antti Kauppila <antti.kauppila@arm.com>
Date:   Thu Sep 8 18:13:26 2016 +0300

    New CoAP changes updated to this library
    
    Unittests updated also

commit 70447c3e35426be19fb1885aaa85e645f2ee144d
Author: Tero Heinonen <tero.heinonen@arm.com>
Date:   Wed Aug 17 14:30:36 2016 +0300

    Set link layer security when opening socket. (#30)
