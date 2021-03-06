.extern main

.thumb
.thumb_func
.global _start
_start:
	// We enter this code from Thumb mode, so switch back to ARM
	ldr	r1, =__arm_start
	bx	r1
.ltorg

.arm
__arm_start:
	// Restore the two words we clobbered on the stack, just in case?
	ldr	r1, =0x000001c0
	str	r1, [sp]
	ldr	r1, =0x20100869
	str	r1, [sp, #-4]

	// Jump into the new kernel at 0x10100000. 
	// Execution shouldn't resume after this point.
	ldr	r0, =0x10100000
	ldr	r1, =0x00bad105
	bl	__syscall_boot_new_ios_kernel
__stub_panic:
	b	__stub_panic
.ltorg

.global boot_new_ios_kernel
__syscall_boot_new_ios_kernel:
	.word	0xe6000870
	bx	lr
