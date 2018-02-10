#!bin/bash
echo "制作启动软盘"
echo "dd if=/dev/zero of=a.img bs=512 count=2880"
dd if=/dev/zero of=output/a.img bs=512 count=2880


echo "dd if=output/start16.bin of=a.img bs=512 count=1"
dd if=output/start16.bin of=output/a.img bs=512 count=1

echo "sudo dd if=output/myOS.bin of=a.img bs=512 seek=1"
dd if=output/myOS.bin of=output/a.img bs=512 seek=1

qemu-system-i386 -boot a -fda output/a.img -m 32

