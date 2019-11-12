	.globl		pop
	.p2align	2
	.type		pop,%function
	.extern		=p_top

pop:
	.fnstart
	@arg = tid
	@Pops the top object and returns a pointer to it.
	@ret the address of the poped object.
	push	{r0, lr}

try:
@printf start info
	ldr		r0, =sStart
	@pop		{r1}
	@push	{r1}
	ldr		r1, [sp]
	ldr		r2, =p_top
	ldr 	r2, [r2]		@r2 = p_top
	ldr		r3, [r2, #4]	@next
	blx		printf


	ldr		r1, =p_top
	ldrex	r2, [r1]
	ldr		r3, [r2, #4]
	strex	r0, r3, [r1]	@store next -> [p_top]
	cmp		r0, #0
	bne		try

suc:
	pop		{r1}
	push	{r2}			@save old_p_top
	ldr		r0, =sSuc
	ldr		r2, =p_top
	ldr		r2, [r2]
	blx		printf


	pop		{r0, pc}		@ret = old_p_top
	.fnend
	
	.globl		push
	.p2align	2
	.type		push,%function
	.extern		=p_top

push:
	.fnstart
	@arg = tid, obj_ptr
	@Pushes the object specified by obj_ptr to stack.
	@no return value
	push	{r0, r1, lr}

tryPush:
@print start info
	ldr		r0, =sPushStart
	pop		{r1, r3}
	push	{r1, r3}		@r1 = id, r3 = obj_ptr
	ldr		r2, =p_top
	ldr 	r2, [r2]		@r2 = p_top
	blx		printf



	ldr		r1, =p_top
	ldrex	r2, [r1]		@r2 = p_top
	ldr		r3, [sp, #4]
	str		r2, [r3, #4]	@obj_ptr->next = p_top
	strex	r0, r3, [r1]	@store next -> [p_top]
	cmp		r0, #0
	bne		try

	pop		{r1, r3}		@r1 = id
	ldr		r0, =sPushSuc
	ldr		r2, =p_top
	ldr		r2, [r2]
	blx		printf


	pop		{pc}		@ret = old_p_top
	nop
	.fnend
	
sStart:
	.asciz		"thread %d pop begins, top = %p, next = %p\n"
sFail:
	.asciz		"thread %d pop fails, retry\n"
sSuc:
	.asciz		"thread %d pop suc, top = %p\n"
sPushStart:
	.asciz		"thread %d push begins, top = %p, obj = %p\n"
sPushSuc:
	.asciz		"thread %d push suc, top = %p\n"


