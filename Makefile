CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -fsanitize=address -Iinclude

# One test file per feature: tests/test_<name>.c
TESTS = create get_set print add multiply transpose subtract hadamard

# Keep the compiled test programs in build/
.SECONDARY:

# Compile one test program (library + that test file)
build/test_%: tests/test_%.c src/matrix.c include/matrix.h
	@mkdir -p build
	$(CC) $(CFLAGS) src/matrix.c $< -o $@

# Run one test:  make run-add
run-%: build/test_%
	./build/test_$*

# Run all tests:  make test
test:
	@failed=0; \
	for t in $(TESTS); do \
		echo "=== test_$$t ==="; \
		$(MAKE) --no-print-directory run-$$t || failed=$$((failed + 1)); \
	done; \
	echo; \
	if [ $$failed -eq 0 ]; then echo "ALL TEST FILES PASSED"; \
	else echo "$$failed TEST FILE(S) FAILED"; exit 1; fi

clean:
	rm -rf build

.PHONY: test clean
