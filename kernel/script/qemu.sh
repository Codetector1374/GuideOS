#!/bin/bash

qemu-system-x86_64 -m 512M -drive file=guideos.img,format=raw -serial stdio $@
