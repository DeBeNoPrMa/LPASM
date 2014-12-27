.data 0x12
vars:
	0x05
	0x07
.text 0x01
main:
	ldr1 vars
	ldr2 vars + 1
	add
	str1 vars
	cmp
	bre l1
	subs
l1:
	str1 vars + 1
	fi
