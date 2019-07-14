#include "usb-notify.h"

void sig_handler()
{
  printf("[#] received signal, exiting...\n");
}

void display_notification(struct udev_device* dev)
{
  if (dev)
  {
    if (udev_device_get_devnode(dev))
    {
      NotifyNotification * n_usb = notify_notification_new("usb-notify", "Yikes", "dialog-information");
      notify_notification_show(n_usb, NULL);
      g_object_unref(G_OBJECT(n_usb));
    }
  }
  udev_device_unref(dev);
}

void monitor_devices(struct udev* udev)
{

  struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");
  printf("1\n");

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

  /* Check for root */
  //if (geteuid() != 0)
  //{
  //  printf("[!] usb-notify needs root permissions, exiting...\n");
  //  return 1;
  //}

  /* Create libnotify init */
  notify_init("usb-notify");
  NotifyNotification *n = NULL;
  char *message;

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
