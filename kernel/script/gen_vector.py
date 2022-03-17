#!/usr/bin/python3

print('# Generated by gen_vector.py')
print('''
.intel_syntax noprefix
.section .text.vectors
.code64
''')

print('.extern trap_enter_all')
for i in range(256):
    print(f'''
    vector_{i}:''')
    if i not in [8, 10, 11, 12, 13, 14, 17]:
        print('    push 0')
    print(f'    push {i}')
    print(f'    jmp trap_enter_all')

print('''
# Vector Pointer Table
.section .rodata.vectors
.align 8
vectors:
''')

for i in range(256):
    print(f'    .quad vector_{i}')