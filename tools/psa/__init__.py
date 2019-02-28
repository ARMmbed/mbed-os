from os.path import basename


def find_secure_image(notify, resources, ns_image_path, configured_s_image_path, image_type):
    """ Find secure image. """
    image_files = resources.get_file_paths(image_type)
    secure_image = next((f for f in image_files if basename(f) == configured_s_image_path), None)
    secure_image = next((f for f in image_files if basename(f) == basename(ns_image_path)), secure_image)

    if secure_image:
        notify.debug("Secure image file found: %s." % secure_image)
    else:
        notify.debug("Secure image file %s not found. Aborting." % configured_s_image_path)
        raise Exception("Required secure image not found.")

    return secure_image
