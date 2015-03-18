#include "UDev.hpp"
#include "Common/Logging.hpp"
#include <sodium.h>
#include <fstream>

namespace usbguard
{
  Firewall::Rule UDevDeviceToDeviceRule(struct udev_device *dev)
  {
    Firewall::Rule rule;

    const char *id_vendor = udev_device_get_sysattr_value(dev, "idVendor");
    if (id_vendor) {
      rule.id_vendor = id_vendor;
    }

    const char *id_product = udev_device_get_sysattr_value(dev, "idProduct");
    if (id_product) {
      rule.id_product = id_product;
    }

    const char *name = udev_device_get_sysattr_value(dev, "product");
    if (name) {
      rule.name = name;
    }

    rule.hash = UDevDeviceHash(dev);

    log->debug("Device hash: {}", rule.hash);

    const char *port = udev_device_get_sysname(dev);
    if (port) {
      rule.id_ports.push_back(port);
    }

    const char *syspath = udev_device_get_syspath(dev);
    if (syspath) {
      rule.ref_syspath = syspath;
    } else {
      throw std::runtime_error("device wihtout syspath");
    }

    const char *usb_class = udev_device_get_sysattr_value(dev, "bDeviceClass");
    if (usb_class) {
      rule.usb_class = usb_class;
    }

    rule.target = Firewall::Target::Unknown;

    return std::move(rule);
  }

  std::string UDevDeviceHash(struct udev_device *dev)
  {
    unsigned char hash[crypto_generichash_BYTES_MIN];
    crypto_generichash_state state;
    std::string hash_string;
    const std::vector<std::string> filenames = { "idProduct", "idVendor", "descriptors",
						 "product", "manufacturer", "bNumConfigurations",
						 "bDeviceClass", "bDeviceSubClass", "bNumInterfaces",
						 "serial" };

    crypto_generichash_init(&state, NULL, 0, sizeof hash);

    for (auto const& filename : filenames) {
      /*
       * Construct the sys path to the file to be read
       */
      char path[PATH_MAX];
      snprintf(path, PATH_MAX, "%s/%s", udev_device_get_syspath(dev), filename.c_str());
      /*
       * Try to open it. If it fails, terminate the whole process by throwing an
       * exception.
       */
      std::ifstream stream(path);
      if (!stream.is_open()) {
	if (filename == "serial") {
	  /*
	   * Some devices (actually a lot of them), do not export the
	   * iSerial field. The iSerial value is probably the only piece
	   * of information available to us to uniquely identify physical
	   * a USB device from others (of the same type).
	   *
	   * NOTE: Yubikeys don't export the iSerial field value by default
	   *       but they can be configured to do so using the ykpersonalize
	   *       tool (see the "serial-usb-visible" flag).
	   */
	  log->warn("Device doesn't export the iSerial value. Physical device identification not possible.");
	  continue;
	}
	else {
	  log->error("Cannot compute device hash: path {} is unreadable", path);
	  throw std::runtime_error("Cannot compute device hash");
	} 
      }
      /*
       * Read blocks of data from the file and update the hash
       */
      char buffer[1024];
      size_t buflen;
      do {
	stream.read(buffer, sizeof buffer);
	buflen = stream.gcount();
	crypto_generichash_update(&state, (const uint8_t *)buffer, buflen);
      } while(stream.good());
    }

    /* Finalize the hash value */
    crypto_generichash_final(&state, hash, sizeof hash);

    /* bin => hex conversion */
    const size_t hexlen = crypto_generichash_BYTES_MIN * 2 + 1;
    char hexval[hexlen];
    sodium_bin2hex(hexval, hexlen, hash, sizeof hash); 

    return std::string(hexval, hexlen - 1);
  }

} /* namespace usbguard */
