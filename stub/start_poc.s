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
	// Restore the two words we clobbered on the stack
	ldr	r1, =0x000001c0
	str	r1, [sp]
	ldr	r1, =0x20100869
	str	r1, [sp, #-4]

	bl	main

	// Restore the original LR we clobbered, return with -1337
	ldr	r0, =0xfffffac7	
	ldr	r3, =0x20100869
	mov	lr, r3
	bx	lr
.ltorg

.global boot_new_ios_kernel
__syscall_boot_new_ios_kernel:
	.word	0xe6000870
	bx	lr
