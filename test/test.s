.name "Foo"
.comment "Bar"

ld %1, r1
ld %1, r2
ld %5, r4

l1:	add r1, r2, r1
	ld r1, r3
	sub r3, r4, r3
	zjmp %:l1

# Foobar
# Barfoo
