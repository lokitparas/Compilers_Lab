	.file	1 "testing.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 1
	.abicalls
	.rdata
	.align	2
$LC0:
	.ascii	"%d\000"
	.text
	.align	2
	.globl	main
	.ent	main
	.type	main, @function
main:
	.set	nomips16
	.frame	$fp,80,$31		# vars= 40, regs= 3/0, args= 16, gp= 8
	.mask	0xc0010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	addiu	$sp,$sp,-80
	sw	$31,76($sp)
	sw	$fp,72($sp)
	sw	$16,68($sp)
	move	$fp,$sp
	lui	$28,%hi(__gnu_local_gp)
	addiu	$28,$28,%lo(__gnu_local_gp)
	.cprestore	16
	move	$2,$sp
	move	$16,$2
	addiu	$3,$fp,28
	lui	$2,%hi($LC0)
	addiu	$4,$2,%lo($LC0)
	move	$5,$3
	lw	$25,%call16(__isoc99_scanf)($28)
	nop
	jalr	$25
	nop

	lw	$28,16($fp)
	lw	$7,28($fp)
	nop
	move	$2,$7
	sw	$2,52($fp)
	move	$2,$0
	sw	$2,48($fp)
	lw	$3,52($fp)
	lw	$2,48($fp)
	sw	$3,52($fp)
	sw	$2,48($fp)
	li	$4,-1			# 0xffffffffffffffff
	lw	$5,48($fp)
	nop
	andi	$2,$5,0xf
	lw	$5,52($fp)
	nop
	and	$3,$5,$4
	sw	$3,52($fp)
	sw	$2,48($fp)
	lw	$3,52($fp)
	nop
	srl	$2,$3,27
	lw	$3,48($fp)
	nop
	sll	$4,$3,5
	or	$4,$2,$4
	lw	$2,52($fp)
	nop
	sll	$5,$2,5
	li	$6,-1			# 0xffffffffffffffff
	andi	$2,$4,0xf
	and	$3,$5,$6
	move	$5,$3
	move	$4,$2
	move	$2,$7
	sw	$2,44($fp)
	move	$3,$0
	sw	$3,40($fp)
	lw	$3,44($fp)
	lw	$2,40($fp)
	sw	$3,44($fp)
	sw	$2,40($fp)
	li	$4,-1			# 0xffffffffffffffff
	lw	$5,40($fp)
	nop
	andi	$2,$5,0xf
	lw	$5,44($fp)
	nop
	and	$3,$5,$4
	sw	$3,44($fp)
	sw	$2,40($fp)
	lw	$3,44($fp)
	nop
	srl	$2,$3,27
	lw	$3,40($fp)
	nop
	sll	$4,$3,5
	or	$4,$2,$4
	lw	$2,44($fp)
	nop
	sll	$5,$2,5
	li	$6,-1			# 0xffffffffffffffff
	andi	$2,$4,0xf
	and	$3,$5,$6
	move	$5,$3
	move	$4,$2
	move	$2,$7
	sll	$2,$2,2
	addiu	$2,$2,7
	addiu	$2,$2,7
	srl	$2,$2,3
	sll	$2,$2,3
	subu	$sp,$sp,$2
	addiu	$3,$sp,16
	sw	$3,32($fp)
	lw	$5,32($fp)
	nop
	addiu	$2,$5,7
	srl	$2,$2,3
	sll	$2,$2,3
	sw	$2,32($fp)
	lw	$2,32($fp)
	nop
	sw	$2,24($fp)
	move	$sp,$16
	move	$sp,$fp
	lw	$31,76($sp)
	lw	$fp,72($sp)
	lw	$16,68($sp)
	addiu	$sp,$sp,80
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	main
	.ident	"GCC: (Debian 4.3.5-4) 4.3.5"
