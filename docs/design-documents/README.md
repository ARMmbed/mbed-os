## Design process for Mbed OS

This document defines the software design process for Mbed OS. To developing for Mbed OS, use this process to submit a design to have the broader Mbed OS developer community review it.

### Design documents location and organization

- The design document follows the format represented in [the design document template](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/design_template.md).
- For each design submission, create a folder under *https://github.com/ARMmbed/mbed-os/docs/design-documents/* at the appropriate level. We have organized the directory structure under design-documents to align with the `mbed-os` directory structure.
  - It is acceptable to create new subdirectories to organize similar items together.
  - If your implementation is creating a new API, match the folder name directly to the new API. Otherwise, use the title of the feature as the folder name. For example, if your implementation is adding a new EMAC API, you may name the folder **New_EMAC_API**, or if you are creating a new serial driver for Mbed OS, you may name the folder **Serial_driver_for_Mbed_OS**.
- Give your design document the same name as the folder, so it serves as your master design document, if you happen to use other .md files under the same folder as reference materials.
- Place any diagrams you are using in the subfolder *diagrams* under the new folder you have created.

Please see the [example design document](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/example_feature_design/example_feature_design.md) for an example.

#### Using diagrams in design documents

We highly encourage creating diagrams to depict your software design. The [design document template](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/design_template.md) uses diagrams generated using draw.io. draw.io generates XMLs for each diagram, and the diagrams folder also provides those corresponding XMLs. You are free to use that as your reference for generating your own diagrams. When generating your diagrams, please provide the metadata and other files (for example, if you are using draw.io, provide XMLs from draw.io) required for future editing of those diagrams.

### Design process

1. Generate a design document under *https://github.com/ARMmbed/mbed-os/docs/design-documents/*, and generate a pull request to Mbed OS.
   1. Decide whether the initial implementation arrives with the design document or as separate PRs. 
   - If you are planning a large change, we encourage you to start the design review early before the implementation. This helps to avoid major rework of your implementation if required by the design review.
   - The initial submission of the design document doesn't need to be complete or refined in terms of capturing all the details, as you may use the design document itself to brainstorm and discuss on your idea or new feature being proposed.
1. Design review follows the same process as code review. Please refer to the contribution process as outlined in [Contributing to Mbed OS](https://os.mbed.com/contributing/) for more information.
1. Once the design is approved, implementation is completed or adjusted according to the latest design document. You can create the pull request with the final implementation, if it's arriving separately.

### Capturing future changes to design

If your implementation changes in future, update the corresponding design documents to capture the new design. If a new implementation requires changes to some previous implementations, capture the new changes in the original design document, as well. 

### General guidelines for creating design documents

It's up to you to make a judgment on whether a change requires a design document. Small localized changes may not require design documents. However, larger changes, such as the addition of new features and functionality or changes with code turmoil across multiple components, require a backing design document.

It's not necessary to start implementation only after design document review. The caveat is if the proposed design changes significantly in the process of review, it affects your implementation, as well. Therefore, for large features for which you expect a lot of design feedback, we advise you start your implementation after major feedback is captured. For example, API designs can invite a lot of feedback. In such cases, capture that feedback before starting the implementation cycle. For very localized implementations that don't affect other parts of the system or other APIs, it may be acceptable to provide an initial implementation (in the same PR) as a reference along with design document.

### Reference documents

- [Design document template](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/design_template.md).
- [Example design document](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/example_feature_design/example_feature_design.md).
- [Design process](https://github.com/ARMmbed/mbed-os/blob/master/docs/design-documents/README.md).

