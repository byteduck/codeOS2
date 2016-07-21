syscall_setup:
	ret

[extern syscallHandler]
syscall_handler:
	call syscallHandler
	ret