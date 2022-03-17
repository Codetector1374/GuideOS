set $lastcs = -1
set disassembly-flavor intel
define hook-stop
  x/i $pc
  set $lastcs = $cs
end

echo + target remote localhost:1234\n
target remote localhost:1234

echo + symbol-file kernel/kernel\n
symbol-file kernel/kernel.obj

