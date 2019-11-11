/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef _LINUX_VHOST_H
#define _LINUX_VHOST_H
/* Userspace interface for in-kernel virtio accelerators. */

/* vhost is used to reduce the number of system calls involved in virtio.
 *
 * Existing virtio net code is used in the guest without modification.
 *
 * This header includes interface used by userspace hypervisor for
 * device configuration.
 */

#include <linux/vhost_types.h>
#include <linux/types.h>
#include <linux/ioctl.h>

/* ioctls */

#define VHOST_VIRTIO 0xAF

/* Features bitmask for forward compatibility.  Transport bits are used for
 * vhost specific features. */
#define VHOST_GET_FEATURES	_IOR(VHOST_VIRTIO, 0x00, __u64)
#define VHOST_SET_FEATURES	_IOW(VHOST_VIRTIO, 0x00, __u64)

/* Set current process as the (exclusive) owner of this file descriptor.  This
 * must be called before any other vhost command.  Further calls to
 * VHOST_OWNER_SET fail until VHOST_OWNER_RESET is called. */
#define VHOST_SET_OWNER _IO(VHOST_VIRTIO, 0x01)
/* Give up ownership, and reset the device to default values.
 * Allows subsequent call to VHOST_OWNER_SET to succeed. */
#define VHOST_RESET_OWNER _IO(VHOST_VIRTIO, 0x02)

/* Set up/modify memory layout */
#define VHOST_SET_MEM_TABLE	_IOW(VHOST_VIRTIO, 0x03, struct vhost_memory)

/* Write logging setup. */
/* Memory writes can optionally be logged by setting bit at an offset
 * (calculated from the physical address) from specified log base.
 * The bit is set using an atomic 32 bit operation. */
/* Set base address for logging. */
#define VHOST_SET_LOG_BASE _IOW(VHOST_VIRTIO, 0x04, __u64)
/* Specify an eventfd file descriptor to signal on log write. */
#define VHOST_SET_LOG_FD _IOW(VHOST_VIRTIO, 0x07, int)

/* Ring setup. */
/* Set number of descriptors in ring. This parameter can not
 * be modified while ring is running (bound to a device). */
#define VHOST_SET_VRING_NUM _IOW(VHOST_VIRTIO, 0x10, struct vhost_vring_state)
/* Set addresses for the ring. */
#define VHOST_SET_VRING_ADDR _IOW(VHOST_VIRTIO, 0x11, struct vhost_vring_addr)
/* Base value where queue looks for available descriptors */
#define VHOST_SET_VRING_BASE _IOW(VHOST_VIRTIO, 0x12, struct vhost_vring_state)
/* Get accessor: reads index, writes value in num */
#define VHOST_GET_VRING_BASE _IOWR(VHOST_VIRTIO, 0x12, struct vhost_vring_state)

/* Set the vring byte order in num. Valid values are VHOST_VRING_LITTLE_ENDIAN
 * or VHOST_VRING_BIG_ENDIAN (other values return -EINVAL).
 * The byte order cannot be changed while the device is active: trying to do so
 * returns -EBUSY.
 * This is a legacy only API that is simply ignored when VIRTIO_F_VERSION_1 is
 * set.
 * Not all kernel configurations support this ioctl, but all configurations that
 * support SET also support GET.
 */
#define VHOST_VRING_LITTLE_ENDIAN 0
#define VHOST_VRING_BIG_ENDIAN 1
#define VHOST_SET_VRING_ENDIAN _IOW(VHOST_VIRTIO, 0x13, struct vhost_vring_state)
#define VHOST_GET_VRING_ENDIAN _IOW(VHOST_VIRTIO, 0x14, struct vhost_vring_state)

/* The following ioctls use eventfd file descriptors to signal and poll
 * for events. */

/* Set eventfd to poll for added buffers */
#define VHOST_SET_VRING_KICK _IOW(VHOST_VIRTIO, 0x20, struct vhost_vring_file)
/* Set eventfd to signal when buffers have beed used */
#define VHOST_SET_VRING_CALL _IOW(VHOST_VIRTIO, 0x21, struct vhost_vring_file)
/* Set eventfd to signal an error */
#define VHOST_SET_VRING_ERR _IOW(VHOST_VIRTIO, 0x22, struct vhost_vring_file)
/* Set busy loop timeout (in us) */
#define VHOST_SET_VRING_BUSYLOOP_TIMEOUT _IOW(VHOST_VIRTIO, 0x23,	\
					 struct vhost_vring_state)
/* Get busy loop timeout (in us) */
#define VHOST_GET_VRING_BUSYLOOP_TIMEOUT _IOW(VHOST_VIRTIO, 0x24,	\
					 struct vhost_vring_state)

/* Set or get vhost backend capability */

/* Use message type V2 */
#define VHOST_BACKEND_F_IOTLB_MSG_V2 0x1

#define VHOST_SET_BACKEND_FEATURES _IOW(VHOST_VIRTIO, 0x25, __u64)
#define VHOST_GET_BACKEND_FEATURES _IOR(VHOST_VIRTIO, 0x26, __u64)

/* Signal the guest running status */
#define VHOST_SET_RUNNING _IOW(VHOST_VIRTIO, 0x61, int)

/* VHOST_NET specific defines */

/* Attach virtio net ring to a raw socket, or tap device.
 * The socket must be already bound to an ethernet device, this device will be
 * used for transmit.  Pass fd -1 to unbind from the socket and the transmit
 * device.  This can be used to stop the ring (e.g. for migration). */
#define VHOST_NET_SET_BACKEND _IOW(VHOST_VIRTIO, 0x30, struct vhost_vring_file)

/* VHOST_SCSI specific defines */

#define VHOST_SCSI_SET_ENDPOINT _IOW(VHOST_VIRTIO, 0x40, struct vhost_scsi_target)
#define VHOST_SCSI_CLEAR_ENDPOINT _IOW(VHOST_VIRTIO, 0x41, struct vhost_scsi_target)
/* Changing this breaks userspace. */
#define VHOST_SCSI_GET_ABI_VERSION _IOW(VHOST_VIRTIO, 0x42, int)
/* Set and get the events missed flag */
#define VHOST_SCSI_SET_EVENTS_MISSED _IOW(VHOST_VIRTIO, 0x43, __u32)
#define VHOST_SCSI_GET_EVENTS_MISSED _IOW(VHOST_VIRTIO, 0x44, __u32)

/* VHOST_VSOCK specific defines */

#define VHOST_VSOCK_SET_GUEST_CID	_IOW(VHOST_VIRTIO, 0x60, __u64)
#define VHOST_VSOCK_SET_RUNNING		VHOST_SET_RUNNING

/* VHOST_DSP specific defines */

#define VHOST_DSP_SET_GUEST_TPLG	_IOW(VHOST_VIRTIO, 0x80,	\
					struct vhost_dsp_topology)

#endif
