//
// Copyright (C) 2019 Wayne Campbell
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Wayne Campbell <wcampbell1995@gmail.com>
//
#define _GNU_SOURCE
#include "../include/usb-notify.h"

#include <libudev.h>
#include <libnotify/notify.h>
#include <stdio.h>

void sig_handler()
{
  printf("[#] received signal, exiting...\n");
}

/*
 * Display Notification
 *
 * Display notification of the device informatino if the device is of the
 * 'bind' udev action.
 *
 * @param dev, udev_device from udev
 * @return int of success
 */
int display_notification(struct udev_device* dev)
{
  if (dev)
  {
    if (udev_device_get_devnode(dev))
    {
      /* Check if action is bind */
      const char* action = udev_device_get_action(dev);
      if (!strcmp(action, "add"))
      {
        const char *product  = udev_device_get_sysattr_value(dev, "product");
        const char *vid      = udev_device_get_sysattr_value(dev, "idVendor");
        const char *pid      = udev_device_get_sysattr_value(dev, "idProduct");
        const char *serial   = udev_device_get_sysattr_value(dev, "serial");

        char *message;
        if (0 > asprintf(&message,
            "Product : %s\n"
            "Serial #: %s\n"
            "Vid-Pid: %s-%s",
            product, serial, vid, pid))
        {
            printf("[!] Error allocating char\n");
        }
        printf("[#] Displaying message: \n%s\n", message);

        /* Display notification */
        NotifyNotification * n_usb = notify_notification_new("usb-notify", message, "dialog-information");
        notify_notification_show(n_usb, NULL);
        g_object_unref(G_OBJECT(n_usb));
      }
      else
      {
        return 1;
      }
    }
  }
  udev_device_unref(dev);
  return 0;
}

void monitor_devices(struct udev* udev)
{
  struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");

  udev_monitor_filter_add_match_subsystem_devtype(mon, "usb", NULL);
  udev_monitor_enable_receiving(mon);

  int fd = udev_monitor_get_fd(mon);

  while (1)
  {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    /* wait for device from udev */
    int ret = select(fd+1, &fds, NULL, NULL, NULL);
    if (ret <= 0) {
      break;
    }

    if (FD_ISSET(fd, &fds))
    {
      /* Get pointer from device found */
      struct udev_device* dev = udev_monitor_receive_device(mon);
      display_notification(dev);
    }
  }
}

int main()
{
  signal(SIGINT, sig_handler);

  /* Create libnotify init */
  notify_init("usb-notify");

  /* udev init */
  struct udev *udev;
  udev = udev_new();
  if (!udev)
  {
    printf("[!] udev not created, exiting...\n");
    return 1;
  }

  /* init libnotify */
  notify_init("usb-notify");

  /* startup monitor loop */
  monitor_devices(udev);

  /* cleanup libnotify */
  notify_uninit();
  return 0;
}
