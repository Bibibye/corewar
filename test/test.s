.name "Foo"
.comment "Bar"

l1:	sti r1,%:l2,%1
	and r1,%0,r1

l2:	live %1
	zjmp %:l2

# Foobar
# Barfoo
