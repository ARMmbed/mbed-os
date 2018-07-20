# Design Process for Mbed-OS
This document defines the software design process for Mbed-OS. Anyone developing for Mbed-OS should use this process to submit the design to get it reviewed from the broader Mbed-OS developer community.

## Design documents location and organization
* It's suggested to write one design document per change, which could be a new feature, defect fixes, tools changes etc. Do not try to address multiple changes in the same document.
* It's up to the judgement of developer to decide whether a change warrant a design document to go with it. For example, minor changes/small localized defect fixes may not need a design document.
* The design document will follow the format represented in [Design Document Template](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/design_template.md)

* For each design submission, create a folder under *https://github.com/ARMmbed/mbed-os/docs/design-documents/{AREA OR COMPONENT}*. The area/component would be Drivers, Platform, Connectivity, Security, Peripherals etc. 
  If your implementation is creating new APIs, you should try to match the folder name directly to the new API, otherwise 
  use the title of the feature as the folder name. 
  For example, if your implementation is adding new EMAC API, you may name the folder as **New_EMAC_API** or if you are creating a new Serial Driver for Mbed-OS you may name the folder as **Serial_driver_for_Mbed-os**.
* Your design document itself should also be named with the same name of the as the folder, so that it serves
  as your master design document, if you happen to use other .md files under same folder as annexures or reference.
* The diagrams you are using may exist in the sub-folder *diagrams* under the new folder you have created.
  See [Example Design Document](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/example_feature_design/example_feature_design.md) for example design document.

### Using diagrams in Design documents
Its highly encouraged to create diagrams to depict your software design. The [Design Document Template]((https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/design_template.md) provided here uses diagrams generated using draw.io. draw.io generates XMLs for each diagram and those corresponding XMLs are also provided under the diagrams folder, and you are free to use that as your reference for generating your own diagrams. And, when you are generating your own diagrams it would be better to provide the metadata/other files(e.g, if you are using draw.io provide XMLs from draw.io) required for future editing of those diagrams.

## Design Process
* Developer generates a design document located under *https://github.com/ARMmbed/mbed-os/docs/design-documents/{AREA OR COMPONENT}* and generates a pull-request to Mbed-OS.
  * NOTE: If you are part of an organization and would like to do design review within your organization before landing the pull-request, its ok to do so.
  * The pull request may also include the initial code implementation. It's upto to the develper to decide if the initial implementation comes along with the design document or later.
  * Although, if you are planning a large change, its encoraged to start the design review early before the implementation. This also help to avoid major re-work of your implementation if required by the design review.
* Design review follows the same process as code review. Please refer contribution process as outlined in [Contributing to Mbed-OS](https://os.mbed.com/contributing/) for more information.
* Once the design is approved, implementation is completed/adjusted according to the latest design document. Developer can create the pull-request with the final implementation, if its coming separately.

## Capturing future changes to design
If your implementation changes in future, corresponsding design documents may be updated to capture the new design.
It's possible that a new implementation may require changes to some previous implementations. In that case, capture
the new changes in the original design document as well. Also create references to other(or older) design documents in the
References section for the new design document.

## General guidelines for creating design documents
Its up to the developer to make a judgement on whether a design docucment is required for a change. It may not be required for minor fixes and small localized changes. But for changes with lot of code turmoil across multiple components or if its new feature or functionality being added its best to have a backing design document. Its also ok for developers to gather initial feedback on some idea before they decide to implement. Its also not absolutely necessary that implementation should be started only after design document review. The caveat here is if the proposed design is changed significantly in the process of review it will impact your implementation as well. Thus for large features where lot of design feedbacks are expected it's advisable to start your implementation once major feedbacks are captured. For example, API designs can invite lot of feedbacks. In such cases, its best to capture those feedbacks before starting the implementation cycle. But for very localized implementations not impacting other parts of the system/APIs it may be ok to provide an initial implementation(in the same PR) as a reference along with design document.

## Reference documents
[Design Document Template](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/design_template.md)

[Example Design Document](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/example_feature_design/example_feature_design.md)

[Design process](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/README.md)

