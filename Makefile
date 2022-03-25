.PHONY: all blockMatch bugInsert markBBL util varFind

all: blockMatch bugInsert markBBL util varFind

blockMatch:
	make TARGET -C blockMatch

bugInsert:
	make -C bugInsert

markBBL:
	make -C markBBL

util:
	make -C util

varFind:
	make TARGET -C varFind

clean:
	make clean -C blockMatch && \
	make clean -C bugInsert && \
	make clean -C markBBL && \
	make clean -C util && \
	make clean -C varFind && \
	rm -rf lib/*

