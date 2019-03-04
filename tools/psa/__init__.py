from os.path import basename
from tools.resources import FileType


def find_secure_image(notify, resources, ns_image_path, configured_s_image_filename, image_type):
    """ Find secure image. """

    assert ns_image_path and configured_s_image_filename, 'ns_image_path and configured_s_image_path are mandatory'
    assert image_type in [FileType.BIN, FileType.HEX], 'image_type must be of type BIN or HEX'

    image_files = resources.get_file_paths(image_type)
    assert image_files, 'No image files found for this target'

    secure_image = next((f for f in image_files if basename(f) == configured_s_image_filename), None)
    secure_image = next((f for f in image_files if basename(f) == basename(ns_image_path)), secure_image)

    if secure_image:
        notify.debug("Secure image file found: %s." % secure_image)
    else:
        notify.debug("Secure image file %s not found. Aborting." % configured_s_image_filename)
        raise Exception("Required secure image not found.")

    return secure_image
