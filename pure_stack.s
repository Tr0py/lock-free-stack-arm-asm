	.globl		pop
	.p2align	2
	.type		pop,%function
	.extern		=p_top

pop:
	.fnstart
	@no arg 
	@Pops the top object and returns a pointer to it.
	@ret the address of the poped object.

tryPop:
	ldr		r1, =p_top		@r1 = &p_top
	ldrex	r2, [r1]		@r2 = p_top
	ldr		r3, [r2, #4]	@r3 = p_top+4(p_top->next)
	strex	r0, r3, [r1]	@store next -> *&p_top
	cmp		r0, #0			@success?
	bne		tryPop

	mov		r0, r2
	mov		pc, lr
	.fnend
	
	.globl		push
	.p2align	2
	.type		push,%function
	.extern		=p_top

push:
	.fnstart
	@arg = obj_ptr
	@Pushes the object specified by obj_ptr to stack.
	@no return value

tryPush:

	ldr		r1, =p_top		@r1 = &p_top
	ldrex	r2, [r1]		@r2 = p_top
	str		r2, [r0, #4]	@r0 = obj_ptr, p_top -> obj_ptr->next
	strex	r2, r0, [r1]	@store obj_ptr -> *&p_top
	cmp		r2, #0			@success?
	bne		tryPush

	mov		pc, lr
	.fnend
