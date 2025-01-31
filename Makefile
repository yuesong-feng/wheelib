.PHONY: test

test: wheelib
	$(foreach item, $(wildcard test/*.c), \
		gcc -g $(item) -Isrc -L./ -lwheelib -o $(addsuffix .out, $(notdir $(basename $(item)))); \
	)

wheelib:
	gcc -g -shared -fPIC -o libwheelib.so $(wildcard src/*.c)

clean:
	rm -rf *.out *.so *.dSYM