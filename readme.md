> [!NOTE]
> **9base status: Preserved** · **Lifecycle: archived reference copy.**
>
> This is a historical fork of [TinkerBoard2/debian](https://github.com/TinkerBoard2/debian).
> 9base retains it for provenance and reference and does not actively maintain it.
> Upstream source and inherited authorship remain attributed to their original contributors.

# Tinkerboard2-debian — 9base preservation notes

## Role and verified historical fix

This vendor-derived repository contains shell scripts, overlays, package
patches and build-service material for Rockchip distribution root filesystems,
including Debian 10/Buster for ARM64 and ARMHF. Its retained default branch is
`linux4.19-rk3399-debian10`.

[Commit 0e081e6](https://github.com/9base/Tinkerboard2-debian/commit/0e081e63b98c7d8ddc7dde417a830c0c4f402983),
authored by Suleyman Poyraz (Zaryob) on 22 September 2022, is the one verified
historical local code change. It changes `BASEIMG=linaro-buster-alip` to:

- `linaro-buster-alip-arm64` in `ubuntu-build-service/buster-base-arm64/Makefile`;
- `linaro-buster-alip-armhf` in `ubuntu-build-service/buster-base-armhf/Makefile`.

That precise two-line architecture-specific image-name fix is credited to the
local author. The 8 October 2026 comparison was 1 ahead / 0 behind upstream;
the inspected history does not establish a larger downstream-development
effort or current maintenance. This repository remains **Preserved** under the
conservative board-family classification.

The upstream rootfs guide follows unchanged below. Its Debian/Buster commands
are historical instructions, not newly verified contemporary build guidance.
The existing [LICENSE.txt](LICENSE.txt), source attribution and history remain
in place.

## Tinker Board 2 platform family

| Layer | Preserved 9base repository |
| --- | --- |
| Linux checkout manifests | [Tinkerboard2-manifest](https://github.com/9base/Tinkerboard2-manifest) |
| Linux kernel | [Tinkerboard2-kernel](https://github.com/9base/Tinkerboard2-kernel) |
| U-Boot bootloader | [Tinkerboard2-uboot](https://github.com/9base/Tinkerboard2-uboot) |
| Buildroot build system | [Tinkerboard2-buildroot](https://github.com/9base/Tinkerboard2-buildroot) |
| Debian/rootfs scripts | [Tinkerboard2-debian](https://github.com/9base/Tinkerboard2-debian) |
| Rockchip firmware and loaders | [Tinkerboard2-rkbin](https://github.com/9base/Tinkerboard2-rkbin) |
| Poky/OpenEmbedded/BitBake | [yocto-poky](https://github.com/9base/yocto-poky) |
| Android checkout manifests | [Tinkerboard2Android-manifest](https://github.com/9base/Tinkerboard2Android-manifest) |

The [Linux release manifest](https://github.com/9base/Tinkerboard2-manifest/blob/linux4.19-rk3399-debian10/default.xml) explicitly names the kernel, U-Boot,
Buildroot, Debian, rkbin and yocto-poky components. Its remote still points to
`TinkerBoard2`, not these 9base forks; it does not automatically select 9base's
historical Debian fix. This family is only a retained subset of the vendor's
larger source graph, not a self-contained complete BSP checkout.

The Android manifests concern the same board family but target a separate
`TinkerBoard2-Android` source graph; they do not establish use of these 9base
Linux components.

## Historical note

These preservation notes were reconstructed on **8 October 2026** from the
repository tree, exposed branches, commit history and upstream comparisons.
They are new archival documentation, not evidence that this explanation existed
at the historical fork date. The original reason for retention or any deployment
is not established by the inspected record. No contemporary build, support or
upstream synchronization commitment is implied.

---

## Original upstream README (preserved)

## Introduction

A set of shell scripts that will build GNU/Linux distribution rootfs image
for rockchip platform.

## Available Distro

* Debian 10 (Buster-X11 and Wayland)~~

```
sudo apt-get install binfmt-support qemu-user-static
sudo dpkg -i ubuntu-build-service/packages/*
sudo apt-get install -f
```

## Usage for 32bit Debian 10 (Buster-32)

Building a base debian system by ubuntu-build-service from linaro.

```
	RELEASE=buster TARGET=desktop ARCH=armhf ./mk-base-debian.sh
```

Building the rk-debian rootfs:

```
	RELEASE=buster ARCH=armhf ./mk-rootfs.sh
```

Building the rk-debain rootfs with debug:

```
	VERSION=debug ARCH=armhf ./mk-rootfs-buster.sh
```

Creating the ext4 image(linaro-rootfs.img):

```
	./mk-image.sh
```

---

## Usage for 64bit Debian 10 (Buster-64)

Building a base debian system by ubuntu-build-service from linaro.

```
	RELEASE=buster TARGET=desktop ARCH=arm64 ./mk-base-debian.sh
```

Building the rk-debian rootfs:

```
	RELEASE=buster ARCH=arm64 ./mk-rootfs.sh
```

Building the rk-debain rootfs with debug:

```
	VERSION=debug ARCH=arm64 ./mk-rootfs-buster.sh
```

Creating the ext4 image(linaro-rootfs.img):

```
	./mk-image.sh
```
---

## Cross Compile for ARM Debian

[Docker + Multiarch](http://opensource.rock-chips.com/wiki_Cross_Compile#Docker)

## Package Code Base

Please apply [those patches](https://github.com/rockchip-linux/rk-rootfs-build/tree/master/packages-patches) to release code base before rebuilding!

## FAQ

- noexec or nodev issue
noexec or nodev issue /usr/share/debootstrap/functions: line 1450:
../rootfs/ubuntu-build-service/stretch-desktop-arm64/chroot/test-dev-null:
Permission denied E: Cannot install into target
...
mounted with noexec or nodev

Solution: mount -o remount,exec,dev xxx (xxx is the mount place), then rebuild it.
